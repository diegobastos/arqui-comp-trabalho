#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
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
        ~FileOut();
        void add_str(string line);
    private:
        ofstream file;
};