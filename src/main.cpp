#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <lexer/lexer.h>
#include <parser/parser.h>

int main(){

    std::string file_url = "test/source_file.simpl";

    std::vector<Token> token_vector;
    lexer l(file_url);
    token_vector = l.tokenize();

    parser p(token_vector);
    p.parse();


    return 0;
}
