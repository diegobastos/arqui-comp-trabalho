#include "file.hpp"

FileOut::FileOut(){
    try{
        string path = string(get_current_dir_name()) + "/out/experimenter.csv";
        ifstream f(path);
        bool fileExist = true;
        if(!f){
            fileExist = false;
        }
        file.open(path, std::ios_base::app);
        if(!fileExist){
            file << "data,programa,n_instrucao,n_clock,cpi,ipc,mips,t_cpu" << endl;
        }
    }catch(...){
        cout << "\nErro ao abrir o arquivo\n";
    }
}

void FileOut::add_program(string attribute){
    //attributes[0] = attribute;
     file << __TIMESTAMP__ << "," << attribute << ",";
}
void FileOut::add_clock(string attribute){
    //attributes[2] = attribute;
    file << attribute << ",";
}
void FileOut::add_n_instruction(string attribute){
    //attributes[1] = attribute;
    file  << attribute << ",";
}
void FileOut::add_cpi(string attribute){
    file << attribute;
}

void FileOut::save_file(){
    try{
       /* file << __TIMESTAMP__ ;
        for(int i = 0; i < 3; i++){
            file << "," << attributes[i];
        } */
        file << endl;
    }catch(...){
        cout << "\n Erro ao salvar no arquivo \n";
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