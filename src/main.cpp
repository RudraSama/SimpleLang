#include <iostream>
#include <fstream>
#include <string>

#include <lexer/lexer.h>


int main(){

    std::string file_url = "source_file.simpl";
    lexer l(file_url);
    l.tokenize();
    l.print_tokens();

    return 0;
}
