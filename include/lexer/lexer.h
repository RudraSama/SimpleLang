#pragma once
#include <string>
#include <vector>
#include <fstream>

//this defines type of Token (if it is a number, variable or some keywords)
enum Token_Kind{
    INT_TOKEN,
    NUMBER_TOKEN,
    IDENTIFIER_TOKEN,
    EQUALS_TOKEN,
    ASSIGN_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    SEMICOLON_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    OPENPARA_TOKEN,
    CLOSEPARA_TOKEN,
    OPENBRAC_TOKEN,
    CLOSEBRAC_TOKEN,
};

typedef struct {
    std::string value;
    Token_Kind token_kind;
} Token;


class lexer{
    private:
        std::fstream source_file;
        std::vector<Token> token_vector;
        bool is_alpha(char c);
        bool is_number(char c);
        Token_Kind identifier_or_keyword_check(std::string word);
    public:
        lexer(std::string &file_url);
        std::vector<Token> tokenize();
        static std::string token_kind_string(Token_Kind token_kind);
        void print_tokens();
        void print_file_content();
};
