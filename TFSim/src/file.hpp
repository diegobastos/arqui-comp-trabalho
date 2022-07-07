#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <unistd.h>

using namespace std;
using std::ofstream;

/*
* This class is responsable for create the file and save the results in it.
*/
class FileOut{

    public:
         static int n_instruction;
         FileOut();
         FileOut(string local);
        ~FileOut();
        void add_str(string line);
        void add_program(string attribute);
        void add_clock(string attribute);
        void add_n_instruction(string attribute);
        void add_cpi(string attribute);
        void save_file();
        vector<string> read_file_csv(string local);
        bool check_file_exist(string path);
    private:
        ofstream file;
        string attributes[3];
};