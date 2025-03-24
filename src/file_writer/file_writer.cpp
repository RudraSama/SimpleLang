#include <file_writer/file_writer.h>

void File_Writer::open_file(std::string &output_file_url){
    output_file.open(output_file_url, std::fstream::out);
    output_file<<".text\n";
    output_file<<"\n";
    output_file<<"\n";
}

void File_Writer::append(std::string s){
    position++;
    file_vector.insert(file_vector.begin() + position, (s+"\n"));
}

int File_Writer::get_position(){
    return position;
}

void File_Writer::set_position(int p){
    this->position = p;
}
void File_Writer::close_file(){

    for(std::string s : file_vector){
        output_file<<s;
    }
    output_file.close();
}
