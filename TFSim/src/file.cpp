#include "file.hpp"

FileOut::FileOut(){
    try{
        string path = string(get_current_dir_name()) + "/out/saida.txt";
        file.open(path, std::ios_base::app);
    }catch(...){
        cout << "\nErro ao abrir o arquivo\n";
    }
}

void FileOut::add_str(string line){
    try{
        file << line;
        file << endl;    
    }catch(...){
        cout << "\nErro ao salvar no arquivo\n";
    }
}

FileOut::~FileOut(){
    try{
        if(file.is_open()){
            file.close();
        }
    }catch(...){
        cout << "\nErro ao fechar o arquivo\n";
    }
}