#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <lexer/lexer.h>
#include <parser/parser.h>

int main(int argc, char *argv[]){

    std::string file_url = "test/source_file.simpl";

//    if(argc < 2){
//        std::cout<<"Input file not found\n";
//        return 0;
//    }
//
//    file_url = argv[1];

    std::vector<Token> token_vector;
    lexer l(file_url);
    token_vector = l.tokenize();
    parser p(token_vector);
    p.parse();


    return 0;
}
