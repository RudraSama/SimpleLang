#include <lexer/lexer.h>
#include <parser/parser.h>
#include <iostream>

parser::parser(std::vector<Token> token_vector){
    this->token_vector = std::move(token_vector);
}

Token parser::peek(){
    return token_vector[index];
}

void parser::next_token(){
    index++;
}

bool parser::expect(Token_Kind token_kind){
    if(token_kind == peek().token_kind){
        return true;
    }
    std::cout<<"Expected "<<lexer::token_kind_string(token_kind)<<", got "<<peek().value<<std::endl;
    return false;
}

bool parser::is_literal(TreeNode *node){
    return typeid(*node).name() == typeid(Literal).name();
}

int parser::literal_evaluation(TreeNode *left, TreeNode *right, Token_Kind token_kind){
    //We need to dynamically caste TreeNodes to Literal
    Literal *l = dynamic_cast<Literal*>(left);
    Literal *r = dynamic_cast<Literal*>(right);
    
    switch(token_kind){
        case PLUS_TOKEN:
            return l->get_value() + r->get_value();
        case MINUS_TOKEN:
            return l->get_value() - r->get_value();
        default:
            return 0;
    }
    return 0;
}

void parser::parse(){

    TreeNode *node = program();

    if(node == nullptr){
        std::cout<<"Node is null";
    }
    else{
        node->print();
    }
    std::cout<<std::endl;
}

TreeNode *parser::program(){
    if(peek().token_kind == Token_Kind::INT_TOKEN) {
        //TODO need to add Variable in symbol table before going to next token
        next_token();
        if(!expect(Token_Kind::IDENTIFIER_TOKEN)){
            return nullptr;
        }
        TreeNode *node = parse_statement();
        return node;
    }
    else if(peek().token_kind == Token_Kind::IDENTIFIER_TOKEN){
        TreeNode *node = parse_statement();
        return node;
    }
    else if(peek().token_kind == Token_Kind::IF_TOKEN){
        next_token();
        if(!expect(Token_Kind::OPENPAREN_TOKEN)){
            return nullptr;
        }
        next_token();
        TreeNode *left = parse_expression();
        left->print();
        next_token();
        std::cout<<std::endl;
        TreeNode *right = parse_expression();
        right->print();
        std::cout<<std::endl;
        if(!expect(Token_Kind::CLOSEPAREN_TOKEN)){
            return nullptr;
        }
        next_token();
        if(!expect(Token_Kind::OPENBRAC_TOKEN)){
            return nullptr;
        }
        next_token();
        if(!expect(Token_Kind::CLOSEBRAC_TOKEN)){
            return nullptr;
        }
    }
    return nullptr;
}

TreeNode *parser::parse_statement(){
    Token t = peek(); //Storing Identifier
    //skip Identifier
    next_token();
    //looking for '=' symbol, if symbol is not there, it means it was just variable initialization
    if(expect(Token_Kind::ASSIGN_TOKEN)){
        //TODO look in symbol table before assigning value
        next_token();
        TreeNode *right = parse_expression();
        TreeNode *node = new Assign(new Identifier(t.value), "=",  right);

        if(!expect(Token_Kind::SEMICOLON_TOKEN)){
            return nullptr;
        }

        return node;
    }

    return nullptr;
}

TreeNode *parser::parse_term(){
    if(peek().token_kind == Token_Kind::NUMBER_TOKEN){
        int num = std::atoi(peek().value.c_str());
        next_token();
        return new Literal(num);
    }
    else if(peek().token_kind == Token_Kind::IDENTIFIER_TOKEN){
        TreeNode *node = new Identifier(peek().value);
        next_token();
        return node;
    }
    else if(peek().token_kind == Token_Kind::OPENPAREN_TOKEN){
        next_token(); //skipping '('
        TreeNode *node = parse_expression(); 
        if(!expect(Token_Kind::CLOSEPAREN_TOKEN)){
            return nullptr;
        }
        next_token(); //skipping ')'
        return node;
    }
    else{
        //need to return more detailed error
        std::cout<<"Error in parse_term "<<peek().value<<std::endl;
    }
    return nullptr;
}

TreeNode *parser::parse_expression(){
    TreeNode *left = parse_term();
    //This loop helps in handling associativity
    while(peek().token_kind != Token_Kind::EOF_TOKEN){
        if(peek().token_kind == Token_Kind::PLUS_TOKEN){
            next_token();
            TreeNode *right = parse_term();

            //if both left and right are Literal, just add them
            if(is_literal(left) && is_literal(right)){
                //creating new literal with addition
                left = new Literal(literal_evaluation(left, right, Token_Kind::PLUS_TOKEN));
            }
            else{
                left = new BinaryExpr(left, "+", right);
            }
        }
        else if(peek().token_kind == Token_Kind::MINUS_TOKEN){
            next_token();
            TreeNode *right = parse_term();

            //if both left and right are Literal, just sub them
            if(is_literal(left) && is_literal(right)){
                //creating new literal with subtraction 
                left = new Literal(literal_evaluation(left, right, Token_Kind::MINUS_TOKEN));
            }
            else{
                left = new BinaryExpr(left, "-", right);
            }
        }
        else{
            return left;
        }
    }
    return nullptr;
}

void parser::print_tokens(){
    for(auto t : token_vector){
        std::cout<<t.value;
    }
}
