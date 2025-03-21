#include <lexer/lexer.h>
#include <parser/parser.h>
#include <iostream>

Parser::Parser(std::vector<Token> token_vector, std::string file_name){
    this->token_vector = std::move(token_vector);
    this->file_name = file_name;
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
        std::string message = "Expected " + Lexer::token_to_string(token_kind).value + " Got " + peek().value;
        report_syntax_error(message);
    }
    return false;
}


void Parser::report_syntax_error(std::string message){
    int line = peek().line;
    int column = peek().column;
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
        case PLUS_TOKEN: return l->get_value() + r->get_value();
        case MINUS_TOKEN: return l->get_value() - r->get_value();
        default: return 0;
    }
}


void Parser::parse(){
    TreeNode *node = program();
    TreeNode *temp = node;
    
    while(temp->get_next() != nullptr){
        temp->get_node()->print();
        std::cout<<std::endl;
        temp = temp->get_next();
    }

    std::cout<<std::endl;
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

            //TODO need to add Variable in symbol table before going to next token
            node = parse_statement();
            if(!node && peek().token_kind == Token_Kind::SEMICOLON_TOKEN){
                next_token();
                continue;
            } 
            if(!node) break;
        }
        else if(peek().token_kind == Token_Kind::IDENTIFIER_TOKEN){
            node = parse_statement();
            if(!node) break;
        }
        else if(peek().token_kind == Token_Kind::IF_TOKEN){
            next_token();
            node = parse_ifstatement();
            if(!node) break;
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

    std::string value = peek().value;
    next_token();

    TreeNode *right = parse_expression();
    if(!right) return nullptr;
    if(!expect(Token_Kind::CLOSEPAREN_TOKEN)) return nullptr;
    next_token();
    if(!expect(Token_Kind::OPENBRAC_TOKEN)) return nullptr;
    next_token();

    //Parsing all statements inside if block
    TreeNode *block = parse_statements();
    if(!block->get_node()) return nullptr;

    if(!expect(Token_Kind::CLOSEBRAC_TOKEN)) return nullptr;
    
    return new IFStatement(left, value, right, block);
}


TreeNode *Parser::parse_statement(){
    //Storing Identifier
    Token t = peek(); 
    next_token();

    if(peek().token_kind == Token_Kind::SEMICOLON_TOKEN) return nullptr;

    if(!expect(Token_Kind::ASSIGN_TOKEN)) return nullptr;
    //TODO look in symbol table before assigning value
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
        next_token();
    }
    else if(peek().token_kind == Token_Kind::OPENPAREN_TOKEN){
        next_token(); //skipping '('
        node = parse_expression(); 
        if(!expect(Token_Kind::CLOSEPAREN_TOKEN)) return nullptr;
        next_token(); //skipping ')'
    }
    else{
        report_syntax_error("Expected Identifer or Number");
    }
    return node;
}

TreeNode *Parser::parse_expression(){
    TreeNode *left = parse_term();

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
                std::string symbol = token.token_kind == (Token_Kind::PLUS_TOKEN)?"+":"-";
                left = new BinaryExpr(left, symbol, right);
            }
        }
        else{
            return left;
        }
    }
    
    //This statement will only run when peek() reached at EOF and didn't find any valid syntax
    std::string message = "Expected " + Lexer::token_to_string(Token_Kind::SEMICOLON_TOKEN).value;
    report_syntax_error(message);
    
    return nullptr;
}

void Parser::print_tokens(){
    for(auto t : token_vector){
        std::cout<<t.value;
    }
}
