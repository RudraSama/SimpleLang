#include <lexer/lexer.h>
#include <iostream>
#include <fstream>


lexer::lexer(std::string &file_url){
    this->source_file.open(file_url, std::fstream::in);
}


bool lexer::is_alpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool lexer::is_number(char c){
    return c >= '1' && c <= '9';
}

Token_Kind lexer::identifier_or_keyword_check(std::string word){
    if(word.compare("int") == 0)
        return Token_Kind::INT_TOKEN;
    else if(word.compare("if") == 0)
        return Token_Kind::IF_TOKEN;
    else if(word.compare("else") == 0)
        return Token_Kind::ELSE_TOKEN;
    else 
        return Token_Kind::IDENTIFIER_TOKEN;
}

std::string lexer::token_kind_string(Token_Kind token_kind){
    switch(token_kind){
        case INT_TOKEN:
            return "INT_TOKEN";
        case NUMBER_TOKEN:
            return "NUMBER_TOKEN";
        case IDENTIFIER_TOKEN:
            return "IDENTIFIER_TOKEN";
        case EQUALS_TOKEN:
            return "EQUALS_TOKEN";
        case ASSIGN_TOKEN:
            return "ASSIGN_TOKEN";
        case PLUS_TOKEN:
            return "PLUS_TOKEN";
        case MINUS_TOKEN:
            return "MINUS_TOKEN";
        case SEMICOLON_TOKEN:
            return "SEMICOLON_TOKEN";
        case IF_TOKEN:
            return "IF_TOKEN";
        case ELSE_TOKEN:
            return "ELSE_TOKEN";
        case OPENPAREN_TOKEN:
            return "OPENPAREN_TOKEN";
        case CLOSEPAREN_TOKEN:
            return "CLOSEPAREN_TOKEN";
        case OPENBRAC_TOKEN:
            return "OPENBRAC_TOKEN";
        case CLOSEBRAC_TOKEN:
            return "CLOSEBRAC_TOKEN";
        case EOF_TOKEN:
            return "EOF_TOKEN";
        default:
            return "Unknown Token";
    }
}

void lexer::print_tokens(){
    for(int i = 0; i < (int)token_vector.size(); i++){
        std::cout<<"Token : "<<token_kind_string(token_vector[i].token_kind)<<" | Value : "<<token_vector[i].value<<std::endl;
    }
}

std::vector<Token> lexer::tokenize(){
    char c;
    std::string word;
    Token token;

    while((c = source_file.get()) && c != EOF){
        //if first char is alpha, then whole word must be keyword or identifier
        if(is_alpha(c)){
            word = word + c;
            while((c = source_file.get()) && (is_alpha(c) || (is_number(c)))){
                word = word + c;
            }

            token.value = word;
            token.token_kind = identifier_or_keyword_check(word);

            token_vector.push_back(token);
            word = "";
            source_file.unget(); //return to previous character
        }

        else if(is_number(c)){
            word = word + c;
            while((c = source_file.get()) && is_number(c)){
                word = word + c;
            }

            token.value = word;
            token.token_kind = Token_Kind::NUMBER_TOKEN;
            token_vector.push_back(token);
            word = "";
            source_file.unget();
        }

        else if(c == '='){
            word = word + c;

            while((c = source_file.get()) && c == '='){
                word = word + c;
            }

            token.value = word;
            token.token_kind = (word.size() < 2)?Token_Kind::ASSIGN_TOKEN:Token_Kind::EQUALS_TOKEN; //if word contains two '=' sign, then it equals symbol
            token_vector.push_back(token);
            word = "";
            source_file.unget();
        }

        else if(c == '+' || c == '-' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}'){
            word = word + c;
            
            token.value = word;
            switch(c){
                case '+':
                   token.token_kind = Token_Kind::PLUS_TOKEN; 
                   break;
                case '-':
                   token.token_kind = Token_Kind::MINUS_TOKEN; 
                   break;
                case ';':
                   token.token_kind = Token_Kind::SEMICOLON_TOKEN; 
                   break;
                case '(':
                   token.token_kind = Token_Kind::OPENPAREN_TOKEN; 
                   break;
                case ')':
                   token.token_kind = Token_Kind::CLOSEPAREN_TOKEN; 
                   break;
                case '{':
                   token.token_kind = Token_Kind::OPENBRAC_TOKEN; 
                   break;
                case '}':
                   token.token_kind = Token_Kind::CLOSEBRAC_TOKEN; 
                   break;
            }

            token.value = word;
            token_vector.push_back(token);
            word = "";
        }

        else {
            //Handling if character is something else
        }



    }
    
    token.value = "EOF";
    token.token_kind = EOF_TOKEN;
    token_vector.push_back(token);
    
    return token_vector;

}

void lexer::print_file_content(){
    char c;
    while((c = source_file.get()) && c != EOF){
        std::cout<<c<<std::endl;
    }
}
