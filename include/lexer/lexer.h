#pragma once
#include <string>
#include <vector>
#include <fstream>

/**
 * @enum Token_Kind
 * @brief This defines type of Token (if it is a number, variable or some keywords)
 */
enum Token_Kind{
    INT_TOKEN,              //!< Integer Type
    NUMBER_TOKEN,           //!< Number Type
    IDENTIFIER_TOKEN,       //!< Variable Name or function name
    EQUALS_TOKEN,           //!< == Type
    LESS_TOKEN,             //!< < Type
    GREATER_TOKEN,          //!< > Type
    LESS_EQUALS_TOKEN,      //!< <= Type
    GREATER_EQUALS_TOKEN,   //!< >= Type
    NOT_EQUALS_TOKEN,       //!< != Type
    ASSIGN_TOKEN,           //!< = Type
    PLUS_TOKEN,             //!< + Type
    MINUS_TOKEN,            //!< - Type
    SEMICOLON_TOKEN,        //!< ; Type
    IF_TOKEN,               //!< If Type
    ELSE_TOKEN,             //!< Else Type
    OPENPAREN_TOKEN,        //!< ( Type
    CLOSEPAREN_TOKEN,       //!< ) Type
    OPENBRAC_TOKEN,         //!< { Type
    CLOSEBRAC_TOKEN,        //!< } Type
    EOF_TOKEN               //!< EOF Type
};

/**
 * @brief Struct for storing Token details.
 */
typedef struct {
    std::string value;      //!< Token value in string.
    Token_Kind token_kind;  //!< Field of Token_Kind Enum.
    int line;               //!< Line number of Token in Source file.
    int column;             //!< Column number of Token in Source file.
} Token;

/**
 * @brief Struct for converting Token_Kind Enum to String.
 */
typedef struct{
    std::string token; //!< Token_Kind Enum field in string.
    std::string value; //!< Token value in string. 
} Token_String;


/**
 * @brief Simple Lexer for our compiler.
 * This class contains all methods we need for converting our source code into Tokens.
 */
class Lexer{
    private:
        std::fstream source_file;           //!< File stream for reading our source code 
        std::vector<Token> token_vector;    //!< Stores tokens, created by tokenizer  

        /**
         * @brief Checks if the character is Alphabaet or not (a-z, A-Z).
         * @param char
         * @return bool
         */
        bool is_alpha(char c);

        /**
         * @brief Checks if the character is Numebr or not (0-9).
         * @param char
         * @return bool
         */
        bool is_number(char c);

        /**
         * @brief Checks if the token (word) is Keyword or not. 
         * @param word of string type. 
         * @return bool
         */
        Token_Kind identifier_or_keyword_check(std::string word);

    public:
        /**
         * @brief Constructor for Lexer.
         * @param file_url of source code.
         */
        Lexer(std::string &file_url);

        /**
         * @brief Create tokens from Stream of sourcer file.
         * @return vector of tokens.
         */
        std::vector<Token> tokenize();

        /**
         * @brief Returns String value of Token field from Enum. 
         * @param token_kind. Field of Token_Kind Enum.
         * @return Token_String struct. 
         */
        static Token_String token_to_string(Token_Kind token_kind);

        /**
         * @brief Printing Tokens. 
         * @return void. 
         */
        void print_tokens();

        /**
         * @brief Printing File contents by Characters. 
         * @return void. 
         */
        void print_file_content();
};
