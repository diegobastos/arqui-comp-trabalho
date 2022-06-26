#include "file.hpp"

FileOut::FileOut(string local){
    try{
        string path = string(get_current_dir_name()) + local;
        file.open(path);
    }catch(...){
         cout << "\nErro ao abrir o arquivo\n";
    }
}

FileOut::FileOut(){
    try{
        string path = string(get_current_dir_name()) + "/out/experimenter.csv";
        bool check  = check_file_exist(path);
        file.open(path, std::ios_base::app);
        if(!check ){
            file << "data,programa,n_instrucao,n_clock,cpi,ipc,mips,t_cpu" << endl;
        }
    }catch(...){
        cout << "\nErro ao abrir o arquivo\n";
    }
}

bool FileOut::check_file_exist(string path){
    try{
        ifstream f(path);
        if(!f){
            return false;
        }
        f.close();
        return true;
    }catch(...){
        cout << "Erro ao tentar abrir o arquivo";
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
    }catch(...){
        cout << "\nErro ao salvar no arquivo\n";
    }
    file.close();
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

vector<string> FileOut::read_file_csv(string local){
    try{
        string path = string(get_current_dir_name()) + local;
        vector<string> lines;
        fstream input;
        input.open(path, ios::in);
        string line;
        while(input >> line){
            string word = "";
            for(int i=0; i < (int)line.size(); i++){
                if(line[i] != ','){
                    word += line[i];
                }else{
                    lines.push_back(word);
                    word = "";
                }
            }
            lines.push_back(word);
        }
        input.close();
        return lines;
    }catch(...){
        cout << "\nErro ao abrir o arquivo \n";
    }
}