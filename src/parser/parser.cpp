#include <parser/parser.h>

Parser::Parser(std::vector<Token> token_vector, std::string file_name, SymbolTable *ST){
    this->token_vector = std::move(token_vector);
    this->file_name = file_name;
    this->ST = ST;
}


Token Parser::peek(){
    return token_vector[index];
}


void Parser::next_token(){
    index++;
}


bool Parser::expect(Token_Kind token_kind){
    if(token_kind == peek().token_kind){
        return true;
    }
    if(peek().token_kind != Token_Kind::EOF_TOKEN){
        report_unexpected_syntax_error(token_kind, peek().token_kind);
    }
    return false;
}


void Parser::report_unexpected_syntax_error(Token_Kind expected, Token_Kind got){
    int line = peek().line;
    int column = peek().column;

    std::string expect = Lexer::token_to_string(expected).value;
    std::string _got = Lexer::token_to_string(got).value;
    std::string message = "Expected " + expect + " , instead got " + _got;

    std::cerr<<file_name<<":"<<line<<":"<<column<<" "<<message<<std::endl;
}

void Parser::report_undefined_variable_error(std::string s){
    int line = peek().line;
    int column = peek().column;
    
    std::string message = "Undefined Variable "+s;

    std::cerr<<file_name<<":"<<line<<":"<<column<<" "<<message<<std::endl;
   
}

void Parser::report_already_defined_variable_error(std::string s){
    int line = peek().line;
    int column = peek().column;
    
    std::string message = "Variable is already defined "+s;

    std::cerr<<file_name<<":"<<line<<":"<<column<<" "<<message<<std::endl;
}

void Parser::report_memory_limit_exceed_error(std::string s){
    int line = peek().line;
    int column = peek().column;
    
    std::string message = "Memory Limit Exceeds 128 Bytes while declaring variable "+s;

    std::cerr<<file_name<<":"<<line<<":"<<column<<" "<<message<<std::endl;
}


bool Parser::is_literal(TreeNode *node){
    return typeid(*node).name() == typeid(Literal).name();
}


bool Parser::is_if_statement(TreeNode *node){
    return typeid(*node).name() == typeid(IFStatement).name();
}


bool Parser::is_assign(TreeNode *node){
    return typeid(*node).name() == typeid(Assign).name();
}


bool Parser::is_identifier(TreeNode *node){
    return typeid(*node).name() == typeid(Identifier).name();
}


bool Parser::is_binary_expression(TreeNode *node){
    return typeid(*node).name() == typeid(BinaryExpr).name();
}


int Parser::literal_evaluation(TreeNode *left, TreeNode *right, Token_Kind token_kind){
    Literal *l = dynamic_cast<Literal*>(left);
    Literal *r = dynamic_cast<Literal*>(right);
    
    switch(token_kind){
        case PLUS_TOKEN: return (int)l->get_value() + (int)r->get_value();
        case MINUS_TOKEN: return (int)l->get_value() - (int)r->get_value();
        default: return 0;
    }
}


TreeNode *Parser::parse(){
    TreeNode *node = program();
    return node;
}

TreeNode *Parser::program(){
    TreeNode *statements = parse_statements();
    return statements;
}

TreeNode *Parser::parse_statements(){
    TreeNode *statements_head = new Statements();
    TreeNode *statements = statements_head;
    
    //Loop till we get Closed Bracket (IF Block) or EOF(Without IF Block)
    while(peek().token_kind != Token_Kind::EOF_TOKEN && peek().token_kind != Token_Kind::CLOSEBRAC_TOKEN){
        TreeNode *node = nullptr;
        if(peek().token_kind == Token_Kind::INT_TOKEN) {
            next_token();
            if(!expect(Token_Kind::IDENTIFIER_TOKEN)) return nullptr;

            if(ST->exist(peek().value)){
                report_already_defined_variable_error(peek().value);
                delete statements_head;
                statements_head = nullptr;
                return nullptr;
            }

            if(!ST->insert_symbol(peek().value, "int")){
                report_memory_limit_exceed_error(peek().value);
                delete statements_head;
                statements_head = nullptr;
                return nullptr;
            }

            node = parse_statement();
            if(!node && peek().token_kind == Token_Kind::SEMICOLON_TOKEN){
                next_token();
                continue;
            } 
            if(!node){
                delete statements_head;
                statements_head = nullptr;
                break;
            }
        }
        else if(peek().token_kind == Token_Kind::IDENTIFIER_TOKEN){
            node = parse_statement();
            if(!node){
                delete statements_head;
                statements_head = nullptr;
                break;
            }
        }
        else if(peek().token_kind == Token_Kind::IF_TOKEN){
            next_token();
            node = parse_ifstatement();
            if(!node){
                delete statements_head;
                statements_head = nullptr;
                break;
            }
        }
        statements->add_node(node);
        statements->add_next(new Statements());
        statements = statements->get_next();
    }

    return statements_head;
}


TreeNode *Parser::parse_ifstatement(){
    if(!expect(Token_Kind::OPENPAREN_TOKEN)) return nullptr;
    next_token();

    TreeNode *left = parse_expression();
    if(!left) return nullptr;

    Token_Kind token_kind = peek().token_kind;
    next_token();

    TreeNode *right = parse_expression();
    if(!right) return nullptr;
    if(!expect(Token_Kind::CLOSEPAREN_TOKEN)) return nullptr;
    next_token();
    if(!expect(Token_Kind::OPENBRAC_TOKEN)) return nullptr;
    next_token();

    //Parsing all statements inside if block
    TreeNode *block = parse_statements();
    if(!block) return nullptr;

    if(!expect(Token_Kind::CLOSEBRAC_TOKEN)) return nullptr;
    
    return new IFStatement(left, token_kind, right, block);
}


TreeNode *Parser::parse_statement(){
    //Storing Identifier
    Token t = peek(); 
    next_token();

    if(peek().token_kind == Token_Kind::SEMICOLON_TOKEN) return nullptr;

    if(!expect(Token_Kind::ASSIGN_TOKEN)) return nullptr;
    if(!ST->exist(t.value)){
        report_undefined_variable_error(t.value);
        return nullptr;
    }

    next_token();

    TreeNode *right = parse_expression();
    TreeNode *node = new Assign(new Identifier(t.value), "=",  right);


    if(!expect(Token_Kind::SEMICOLON_TOKEN)) return nullptr;
    next_token();
    return node;
}


TreeNode *Parser::parse_term(){
    TreeNode *node = nullptr;

    if(peek().token_kind == Token_Kind::NUMBER_TOKEN){
        int num = std::atoi(peek().value.c_str());
        next_token();
        node = new Literal(num);
    }
    else if(peek().token_kind == Token_Kind::IDENTIFIER_TOKEN){
        node = new Identifier(peek().value);
        if(!ST->exist(peek().value)){
            report_undefined_variable_error(peek().value);
            node = nullptr;
        }
        next_token();
    }
    else if(peek().token_kind == Token_Kind::MINUS_TOKEN){
        next_token();
        if(!expect(Token_Kind::NUMBER_TOKEN)) return nullptr;
        int num = std::atoi(peek().value.c_str()) * -1;
        node = new Literal(num);
        next_token();
    }
    else if(peek().token_kind == Token_Kind::OPENPAREN_TOKEN){
        next_token(); //skipping '('
        node = parse_expression(); 
        if(!expect(Token_Kind::CLOSEPAREN_TOKEN)) return nullptr;
        next_token(); //skipping ')'
    }
    else{
        report_unexpected_syntax_error(Token_Kind::IDENTIFIER_TOKEN, peek().token_kind);
    }
    return node;
}

TreeNode *Parser::parse_expression(){
    TreeNode *left = parse_term();
    if(!left) return nullptr;

    //This loop helps in handling associativity
    while(peek().token_kind != Token_Kind::EOF_TOKEN){
        Token token = peek();
        if(token.token_kind == Token_Kind::PLUS_TOKEN || token.token_kind == Token_Kind::MINUS_TOKEN){
            next_token();
            TreeNode *right = parse_term();
            if(!right) return nullptr;

            if(is_literal(left) && is_literal(right)){
                left = new Literal(literal_evaluation(left, right, token.token_kind));
            }
            else{
                left = new BinaryExpr(left, token.token_kind, right);
            }
        }
        else{
            return left;
        }
    }
    
    //This statement will only run when peek() reached at EOF and didn't find any valid syntax
    report_unexpected_syntax_error(Token_Kind::SEMICOLON_TOKEN, peek().token_kind);
    
    return nullptr;
}
