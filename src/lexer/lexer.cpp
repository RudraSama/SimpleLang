#include <lexer/lexer.h>
#include <iostream>
#include <fstream>


Lexer::Lexer(std::string &file_url){
    this->source_file.open(file_url, std::fstream::in);
}


bool Lexer::is_alpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Lexer::is_number(char c){
    return c >= '0' && c <= '9';
}

Token_Kind Lexer::identifier_or_keyword_check(std::string word){
    if(word.compare("int") == 0)
        return Token_Kind::INT_TOKEN;
    else if(word.compare("if") == 0)
        return Token_Kind::IF_TOKEN;
    else if(word.compare("else") == 0)
        return Token_Kind::ELSE_TOKEN;
    else 
        return Token_Kind::IDENTIFIER_TOKEN;
}

Token_String Lexer::token_to_string(Token_Kind token_kind){
    Token_String token;
    switch(token_kind){
        case INT_TOKEN:
            token.token = "INT_TOKEN";
            token.value = "int";
            return token;
        case NUMBER_TOKEN:
            token.token = "NUMBER_TOKEN";
            token.value = "Integer";
            return token;
        case IDENTIFIER_TOKEN:
            token.token = "IDENTIFIER_TOKEN";
            token.value = "Identifier";
            return token;
        case EQUALS_TOKEN:
            token.token = "EQUALS_TOKEN";
            token.value = "==";
            return token;
        case LESS_TOKEN:
            token.token = "LESS_TOKEN";
            token.value = "<";
            return token;
        case GREATER_TOKEN:
            token.token = "GREATER_TOKEN";
            token.value = ">";
            return token;
        case LESS_EQUALS_TOKEN:
            token.token = "LESS_EQUALS_TOKEN";
            token.value = "<=";
            return token;
        case GREATER_EQUALS_TOKEN:
            token.token = "GREATER_EQUALS_TOKEN";
            token.value = ">=";
            return token;
        case NOT_EQUALS_TOKEN:
            token.token = "NOT_EQUALS_TOKEN";
            token.value = "!=";
            return token;
        case ASSIGN_TOKEN:
            token.token = "ASSIGN_TOKEN";
            token.value = "=";
            return token;
        case PLUS_TOKEN:
            token.token = "PLUS_TOKEN";
            token.value = "+";
            return token;
        case MINUS_TOKEN:
            token.token = "MINUS_TOKEN";
            token.value = "-";
            return token;
        case SEMICOLON_TOKEN:
            token.token = "SEMICOLON_TOKEN";
            token.value = ";";
            return token;
        case IF_TOKEN:
            token.token = "IF_TOKEN";
            token.value = "if";
            return token;
        case ELSE_TOKEN:
            token.token = "ELSE_TOKEN";
            token.value = "else";
            return token;
        case OPENPAREN_TOKEN:
            token.token = "OPENPAREN_TOKEN";
            token.value = "(";
            return token;
        case CLOSEPAREN_TOKEN:
            token.token = "CLOSEPAREN_TOKEN";
            token.value = ")";
            return token;
        case OPENBRAC_TOKEN:
            token.token = "OPENBRAC_TOKEN";
            token.value = "{";
            return token;
        case CLOSEBRAC_TOKEN:
            token.token = "CLOSEBRAC_TOKEN";
            token.value = "}";
            return token;
        case EOF_TOKEN:
            token.token = "EOF_TOKEN";
            token.value = "EOF";
            return token;
        default:
            token.token = "UNKNOWN_TOKEN";
            token.value = "unknown";
            return token;
    }
}

void Lexer::print_tokens(){
    for(int i = 0; i < (int)token_vector.size(); i++){
        std::cout<<"Token : "<<token_to_string(token_vector[i].token_kind).token<<" | Value : "<<token_vector[i].value<<std::endl;
    }
}

std::vector<Token> Lexer::tokenize(){
    char c;
    std::string word;
    Token token;
    int line_index = 1;
    int column_index = 0;

    while((c = source_file.get()) && c != EOF){
        //increment column_index until u reach new line char
        column_index++;
        //if first char is alpha, then whole word must be keyword or identifier
        if(is_alpha(c)){
            word = word + c;
            while((c = source_file.get()) && (is_alpha(c) || (is_number(c)))){
                word = word + c;
            }

            token.value = word;
            token.line = line_index;
            token.column = column_index;
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
            token.line = line_index;
            token.column = column_index;
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
        else if(c == '<' || c == '>' || c == '!'){
            word = word + c;

            while((c = source_file.get()) && c == '='){
                word = word + c;
            }
            
            token.value = word;
            token.line = line_index;
            token.column = column_index;
            if(word.compare("<"))
                token.token_kind = Token_Kind::LESS_TOKEN;
            else if(word.compare(">"))
                token.token_kind = Token_Kind::GREATER_TOKEN;
            else if(word.compare("<="))
                token.token_kind = Token_Kind::LESS_EQUALS_TOKEN;
            else if(word.compare(">="))
                token.token_kind = Token_Kind::GREATER_EQUALS_TOKEN;
            else
                token.token_kind = Token_Kind::NOT_EQUALS_TOKEN;
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
            token.line = line_index;
            token.column = column_index;
            token_vector.push_back(token);
            word = "";
        }
        else if(c == '\n'){
            column_index = 0;
            line_index++;
        }
    }
    
    token.value = "EOF";
    token.token_kind = EOF_TOKEN;
    token_vector.push_back(token);
    
    return token_vector;

}

void Lexer::print_file_content(){
    char c;
    while((c = source_file.get()) && c != EOF){
        std::cout<<c<<std::endl;
    }
}
