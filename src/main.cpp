#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <symbol_table/symbol_table.h>
#include <code_generator/code_generator.h>

int main(int argc, char *argv[]){

    std::string input_file_url;
    std::string output_file_url;



    if(argc < 3 || argc > 3){
        std::cout<<"Usage : \n";
        std::cout<<"./simplelang input_file output_file\n";
        return 0;
    }

    input_file_url = argv[1];
    output_file_url = argv[2];


    SymbolTable *ST = new SymbolTable;

    std::vector<Token> token_vector;
    Lexer l(input_file_url);
    token_vector = l.tokenize();

    Parser p(token_vector, input_file_url, ST);
    TreeNode *ast_head = p.parse();

    
    Code_Generator cg(output_file_url , ST);
    cg.generate(ast_head);

    delete ST;


    return 0;
}
