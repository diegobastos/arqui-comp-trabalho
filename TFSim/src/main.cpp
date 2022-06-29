#include<systemc.h>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<nana/gui.hpp>
#include<nana/gui/widgets/button.hpp>
#include<nana/gui/widgets/menubar.hpp>
#include<nana/gui/widgets/group.hpp>
#include<nana/gui/widgets/textbox.hpp>
#include<nana/gui/filebox.hpp>
#include "top.hpp"
#include "gui.hpp"
#include "file.hpp"

using std::string;
using std::vector;
using std::map;
using std::fstream;


int sc_main(int argc, char *argv[])
{
    using namespace nana;
    vector<string> instruction_queue;
    string program_selected = "/out/value_register_input";
    int nadd,nmul,nls;
    nadd = 3;
    nmul = nls = 2;
    std::vector<int> sizes;
    bool spec = false;
    bool fila = false;
    ifstream inFile;
    form fm(API::make_center(1024,700));
    place plc(fm);
    place upper(fm);
    place lower(fm);
    listbox table(fm);
    listbox reg(fm);
    listbox instruct(fm);
    listbox rob(fm);
    menubar mnbar(fm);
    button botao(fm);
    button running_all(fm);
    button clock_control(fm);
    button exit(fm);
    group clock_group(fm);
    label clock_count(clock_group);
    fm.caption("TFSim");
    clock_group.caption("Ciclo de Clock");
    clock_group.div("count");
    grid memory(fm,rectangle(),10,50);
    // Tempo de latencia de uma instrucao
    // Novas instrucoes devem ser inseridas manualmente aqui
    map<string,int> instruct_time{{"DADD",4},{"DADDI",4},{"DSUB",6},{"DSUBI",6},{"DMUL",10},{"DDIV",16},{"MEM",2},
    {"AND",1},{"OR",1},{"DSLL",1},{"DSRL",1},{"DSLT",1}};
    top top1("top");
    botao.caption("START");
    running_all.caption("Running All");
    clock_control.caption("NEXT CYCLE");
    exit.caption("EXIT");
    plc["rst"] << table;
    plc["btns"] << botao << clock_control << exit << running_all;
    plc["memor"] << memory;
    plc["regs"] << reg;
    plc["rob"] << rob;
    plc["instr"] << instruct;
    plc["clk_c"] << clock_group;
    clock_group["count"] << clock_count;
    clock_group.collocate();

    spec = false;
    set_spec(plc,spec);
    plc.collocate();

    mnbar.push_back("Opções");
    menu &op = mnbar.at(0);
    menu::item_proxy spec_ip = op.append("Especulação",[&](menu::item_proxy &ip)
    {
        if(ip.checked())
            spec = true;
        else
            spec = false;
        set_spec(plc,spec);
    });

    op.check_style(0,menu::checks::highlight);
    op.append("Modificar valores...");
    auto sub = op.create_sub_menu(1);
    sub->append("Número de Estações de Reserva",[&](menu::item_proxy ip)
    {
        inputbox ibox(fm,"","Quantidade de Estações de Reserva");
        inputbox::integer add("ADD/SUB",nadd,1,10,1);
        inputbox::integer mul("MUL/DIV",nmul,1,10,1);
        inputbox::integer sl("LOAD/STORE",nls,1,10,1);
        if(ibox.show_modal(add,mul,sl))
        {
            nadd = add.value();
            nmul = mul.value();
            nls = sl.value();
        }
    });
    // Menu de ajuste dos tempos de latencia na interface
    // Novas instrucoes devem ser adcionadas manualmente aqui
    sub->append("Tempos de latência", [&](menu::item_proxy &ip)
    {
        inputbox ibox(fm,"","Tempos de latência para instruções");
        inputbox::text dadd_t("DADD",std::to_string(instruct_time["DADD"]));
        inputbox::text daddi_t("DADDI",std::to_string(instruct_time["DADDI"]));
        inputbox::text dsub_t("DSUB",std::to_string(instruct_time["DSUB"]));
        inputbox::text dsubi_t("DSUBI",std::to_string(instruct_time["DSUBI"]));
        inputbox::text dmul_t("DMUL",std::to_string(instruct_time["DMUL"]));
        inputbox::text ddiv_t("DDIV",std::to_string(instruct_time["DDIV"]));

        inputbox::text dand_t("AND",std::to_string(instruct_time["AND"]));
        inputbox::text dor_t("OR",std::to_string(instruct_time["OR"]));
        inputbox::text dsll_t("DSLL",std::to_string(instruct_time["DSLL"]));
        inputbox::text dsrl_t("DSRL",std::to_string(instruct_time["DSRL"]));
        inputbox::text dslt_t("DSLT",std::to_string(instruct_time["DSLT"]));

        inputbox::text mem_t("Load/Store",std::to_string(instruct_time["MEM"]));
        if(ibox.show_modal(dadd_t,daddi_t,dsub_t,dsubi_t,dand_t,dor_t,dsll_t,dsrl_t,dslt_t,dmul_t,ddiv_t,mem_t))
        {
            instruct_time["DADD"] = std::stoi(dadd_t.value());
            instruct_time["DADDI"] = std::stoi(daddi_t.value());
            instruct_time["DSUB"] = std::stoi(dsub_t.value());
            instruct_time["DSUBI"] = std::stoi(dsubi_t.value());
            
            instruct_time["AND"] = std::stoi(dand_t.value());
            instruct_time["OR"] = std::stoi(dor_t.value());
            instruct_time["DSLL"] = std::stoi(dsll_t.value());
            instruct_time["DSRL"] = std::stoi(dsrl_t.value());
            instruct_time["DSLT"] = std::stoi(dslt_t.value());
            
            instruct_time["DMUL"] = std::stoi(dmul_t.value());
            instruct_time["DDIV"] = std::stoi(ddiv_t.value());
            instruct_time["MEM"] = std::stoi(mem_t.value());
        }
    });
    sub->append("Fila de instruções", [&](menu::item_proxy &ip)
    {
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo com a lista de instruções:");
        inputbox::path caminho("",fb);
        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
                fila = true;
        }
    });
    sub->append("Valores de registradores inteiros",[&](menu::item_proxy &ip)
    {
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo de valores de registradores inteiros:");
        inputbox::path caminho("",fb);
        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!inFile.is_open())
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
            {
                auto reg_gui = reg.at(0);
                int value,i = 0;
                while(inFile >> value && i < 32)
                {
                    reg_gui.at(i).text(1,std::to_string(value));
                    i++;
                }
                for(; i < 32 ; i++)
                    reg_gui.at(i).text(1,"0");
                inFile.close();
            }
        }
    });
    sub->append("Valores de registradores PF",[&](menu::item_proxy &ip)
    {
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo de valores de registradores PF:");
        inputbox::path caminho("",fb);
        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!inFile.is_open())
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
            {
                auto reg_gui = reg.at(0);
                int i = 0;
                float value;
                while(inFile >> value && i < 32)
                {
                    reg_gui.at(i).text(4,std::to_string(value));
                    i++;
                }
                for(; i < 32 ; i++)
                    reg_gui.at(i).text(4,"0");
                inFile.close();
            }
        }
    });
    sub->append("Valores de memória",[&](menu::item_proxy &ip)
    {
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo de valores de memória:");
        inputbox::path caminho("",fb);
        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!inFile.is_open())
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
            {
                int i = 0;
                int value;
                while(inFile >> value && i < 500)
                {
                    memory.Set(i,std::to_string(value));
                    i++;
                }
                for(; i < 500 ; i++)
                {
                    memory.Set(i,"0");
                }
                inFile.close();
            }
        }
    });
    op.append("Verificar conteúdo...");
    auto new_sub = op.create_sub_menu(2);
    new_sub->append("Valores de registradores",[&](menu::item_proxy &ip) {
        
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo de valores de registradores:");
        inputbox::path caminho(string(get_current_dir_name()),fb);

        show_message("Status", caminho.value());

        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!inFile.is_open())
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
            {
                string str;
                int reg_pos,i;
                bool is_float, ok; 
                ok = true;
                while(inFile >> str)
                {
                    if(str[0] == '$')
                    {
                        if(str[1] == 'f')
                        { 
                            i = 2;
                            is_float = true; 
                        }
                            else 
                        { 
                            i = 1; 
                            is_float = false;
                        }
                        reg_pos = std::stoi(str.substr(i,str.size()-i));
                        
                        auto reg_gui = reg.at(0);
                        string value;
                        inFile >> value;
                        if(!is_float)
                        {
                            if(reg_gui.at(reg_pos).text(1) != value)
                            {
                                ok = false;
                                break;
                            }
                        }
                        else
                        {
                            if(std::stof(reg_gui.at(reg_pos).text(4)) != std::stof(value))
                            {
                                ok = false;
                                break;
                            }   
                        }
                    }
                }   
                inFile.close();
                msgbox msg("Verificação de registradores");
                if(ok)
                {
                    msg << "Registradores especificados apresentam valores idênticos!";
                    msg.icon(msgbox::icon_information);
                }
                else
                {
                    msg << "Registradores especificados apresentam valores distintos!";
                    msg.icon(msgbox::icon_error);
                }
                msg.show();
            }
        }
    });
    new_sub->append("Valores de memória",[&](menu::item_proxy &ip) {
        filebox fb(0,true);
        inputbox ibox(fm,"Localização do arquivo de valores de memória:");
        inputbox::path caminho("",fb);
        if(ibox.show_modal(caminho))
        {
            auto path = caminho.value();
            inFile.open(path);
            if(!inFile.is_open())
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
            {
                string value;
                bool ok; 
                ok = true;
                for(int i = 0 ; i < 500 ; i++)
                {
                    inFile >> value;
                    if(std::stoi(memory.Get(i)) != (int)std::stol(value,nullptr,16))
                    {
                        ok = false;
                        break;
                    }
                }
                inFile.close();
                msgbox msg("Verificação de memória");
                if(ok)
                {
                    msg << "Endereços de memória especificados apresentam valores idênticos!";
                    msg.icon(msgbox::icon_information);
                }
                else
                {
                    msg << "Endereços de memória especificados apresentam valores distintos!";
                    msg.icon(msgbox::icon_error);
                }
                msg.show();
            }
        }
    });
    op.append("Benchmarks");
    auto reg_gui = reg.at(0);
    auto bench_sub = op.create_sub_menu(3);
    bench_sub->append("Fibonacci",[&](menu::item_proxy &ip){
        string path = "in/benchmarks/fibonacci.txt";        
        inFile.open(path);
        if(!add_instructions(inFile,instruction_queue,instruct))
            show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
        else
            fila = true;
        FileOut saveObj;
        saveObj.add_program("Fibonacci");
    });
    bench_sub->append("Busca em Vetor",[&](menu::item_proxy &ip){
        string path = "in/benchmarks/vector_search.txt";        
        inFile.open(path);
        if(!add_instructions(inFile,instruction_queue,instruct))
            show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
        else
            fila = true;

        FileOut saveObj;
        saveObj.add_program("Vector_search");
    });
    bench_sub->append("Stall por Divisão",[&](menu::item_proxy &ip){
        string path = "in/benchmarks/division_stall.txt";       
        inFile.open(path);
        if(!add_instructions(inFile,instruction_queue,instruct))
            show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
        else
            fila = true;

        FileOut saveObj;
        saveObj.add_program("Stall por Divisao");
    });
    bench_sub->append("Stress de Memória (Stores)",[&](menu::item_proxy &ip){
        string path = "in/benchmarks/store_stress.txt";     
        inFile.open(path);
        if(!add_instructions(inFile,instruction_queue,instruct))
            show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
        else
            fila = true;
        FileOut saveObj;
        saveObj.add_program("Store_stress");
    });
    bench_sub->append("Stall por hazard estrutural (Adds)",[&](menu::item_proxy &ip){
        string path = "in/benchmarks/res_stations_stall.txt";       
        inFile.open(path);
        if(!add_instructions(inFile,instruction_queue,instruct))
            show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
        else
            fila = true;

        FileOut saveObj;
        saveObj.add_program("Res_stations_stall");
    });

    //vector<string> files = {"and","or","show_pares","solu_populacao"};
    bench_sub->append("and",[&](menu::item_proxy &ip){
            string path = "in/benchmarks/and/and.txt";
            program_selected = "/in/benchmarks/and/and_input"; 
            cout << "Reading the file ...: " << path;       
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else{
                fila = true;
                FileOut testFile;
                string local_file = string(get_current_dir_name())+program_selected+".csv";
                if( testFile.check_file_exist(local_file )){
                    cout  << "Carregando Banco de Registradores[ " << local_file << "] !" << endl;
                    std::vector<string> lines = testFile.read_file_csv(program_selected+".csv");
                    int ct = 4;
                    for(int i = 0 ; i < 32 ; i++){
                        reg_gui.at(i).text(1, lines[ct+1]);
                        reg_gui.at(i).text(2,"0");
                        reg_gui.at(i).text(4, lines[ct+3]);
                        reg_gui.at(i).text(5,"0");
                        ct += 4;
                    }
                }
                FileOut testF;
                string fileMemory = string(get_current_dir_name())+"/in/benchmarks/and/and_memory_input.csv";
                if( testF.check_file_exist(fileMemory)){
                    cout << "Load values of memory " << endl;
                    std::vector<string> lines = testF.read_file_csv("/in/benchmarks/and/and_memory_input.csv");
                    for(int i = 0; i < 500; i++){
                        memory.Push(lines[i]);
                    }
                }
            }
            FileOut saveObj;
            saveObj.add_program("and");
    });
    bench_sub->append("or",[&](menu::item_proxy &ip){
            string path = "in/benchmarks/or/or.txt";
            program_selected = "/in/benchmarks/or/or_input"; 
            cout << "Reading the file ...: " << path;       
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else{
                fila = true;
                FileOut testFile;
                string local_file = string(get_current_dir_name())+program_selected+".csv";
    
                if( testFile.check_file_exist(local_file )){
                    cout  << "Carregando Banco de Registradores[ " << local_file << "] !" << endl;
                    std::vector<string> lines = testFile.read_file_csv(program_selected+".csv");
                    int ct = 4;
                    for(int i = 0 ; i < 32 ; i++){
                        reg_gui.at(i).text(1, lines[ct+1]);
                        reg_gui.at(i).text(2,"0");
                        reg_gui.at(i).text(4, lines[ct+3]);
                        reg_gui.at(i).text(5,"0");
                        ct += 4;
                    }
                }
                FileOut testF;
                string fileMemory = string(get_current_dir_name())+"/in/benchmarks/or/or_memory_input.csv";
                if( testF.check_file_exist(fileMemory)){
                    cout << "Load values of memory " << endl;
                    std::vector<string> lines = testF.read_file_csv("/in/benchmarks/or/or_memory_input.csv");
                    for(int i = 0; i < 500; i++){
                        memory.Push(lines[i]);
                    }
                }
            }
            FileOut saveObj;
            saveObj.add_program("or");
    });
    bench_sub->append("show_pares",[&](menu::item_proxy &ip){
            string path = "in/benchmarks/show_pares/show_pares.txt"; 
            program_selected = "/in/benchmarks/show_pares/show_pares_input";
            cout << "Reading the file ...: " << path;       
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else{
                fila = true;
                FileOut testFile;
                string local_file = string(get_current_dir_name())+program_selected+".csv";
    
                if( testFile.check_file_exist(local_file )){
                    cout  << "Carregando Banco de Registradores[ " << local_file << "] !" << endl;
                    std::vector<string> lines = testFile.read_file_csv(program_selected+".csv");
                    int ct = 4;
                    for(int i = 0 ; i < 32 ; i++){
                        reg_gui.at(i).text(1, lines[ct+1]);
                        reg_gui.at(i).text(2,"0");
                        reg_gui.at(i).text(4, lines[ct+3]);
                        reg_gui.at(i).text(5,"0");
                        ct += 4;
                    }
                }
                FileOut testF;
                string fileMemory = string(get_current_dir_name())+"/in/benchmarks/show_pares/show_pares_memory_input.csv";
                if( testF.check_file_exist(fileMemory)){
                    cout << "Load values of memory " << endl;
                    std::vector<string> lines = testF.read_file_csv("/in/benchmarks/show_pares/show_pares_memory_input.csv");
                    for(int i = 0; i < 500; i++){
                        memory.Push(lines[i]);
                    }
                }
            }
            FileOut saveObj;
            saveObj.add_program("show_pares");
    });
    bench_sub->append("solu_populacao",[&](menu::item_proxy &ip){
            string path = "in/benchmarks/solu_populacao/solu_populacao.txt";
            program_selected = "/in/benchmarks/solu_populacao/solu_populacao_input"; 
            cout << "Reading the file ...: " << path;       
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else{
                fila = true;
                FileOut testFile;
                string local_file = string(get_current_dir_name())+program_selected+".csv";
    
                if( testFile.check_file_exist(local_file )){
                    cout  << "Carregando Banco de Registradores[ " << local_file << "] !" << endl;
                    std::vector<string> lines = testFile.read_file_csv(program_selected+".csv");
                    int ct = 4;
                    for(int i = 0 ; i < 32 ; i++){
                        reg_gui.at(i).text(1, lines[ct+1]);
                        reg_gui.at(i).text(2,"0");
                        reg_gui.at(i).text(4, lines[ct+3]);
                        reg_gui.at(i).text(5,"0");
                        ct += 4;
                    }
                }
                FileOut testF;
                string fileMemory = string(get_current_dir_name())+"/in/benchmarks/solu_populacao/solu_populacao_memory_input.csv";
                if( testF.check_file_exist(fileMemory)){
                    cout << "Load values of memory " << endl;
                    std::vector<string> lines = testF.read_file_csv("/in/benchmarks/solu_populacao/solu_populacao_memory_input.csv");
                    for(int i = 0; i < 500; i++){
                        memory.Push(lines[i]);
                    }
                }
            }
            FileOut saveObj;
            saveObj.add_program("solu_populacao");
    });
    bench_sub->append("bubbleSort",[&](menu::item_proxy &ip){
            string path = "in/benchmarks/buble_sort.txt";
            cout << "Reading the file ...: " << path;       
            inFile.open(path);
            if(!add_instructions(inFile,instruction_queue,instruct))
                show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
            else
                fila = true;
            FileOut saveObj;
            saveObj.add_program("bubbleSort");
    });


    vector<string> columns = {"#","Name","Busy","Op","Vj","Vk","Qj","Qk","A"}; 
    for(unsigned int i = 0 ; i < columns.size() ; i++)
    {
        table.append_header(columns[i].c_str());
        if(i && i != 3)
            table.column_at(i).width(45);
        else if (i == 3)
            table.column_at(i).width(60);
        else
            table.column_at(i).width(30);
    }
    columns = {"","Value","Qi"};
    sizes = {30,75,40};
    for(unsigned int k = 0 ; k < 2 ; k++)
        for(unsigned int i = 0 ; i < columns.size() ; i++)
        {
            reg.append_header(columns[i]);
            reg.column_at(k*columns.size() + i).width(sizes[i]);
        }

  
    for(int i = 0 ; i < 32 ;i++)
    {
        string index = std::to_string(i);
        reg_gui.append("R" + index);
        reg_gui.at(i).text(3,"F" + index);
    }

    columns = {"Instruction","Issue","Execute","Write Result"};
    sizes = {140,60,70,95};
    for(unsigned int i = 0 ; i < columns.size() ; i++)
    {
        instruct.append_header(columns[i]);
        instruct.column_at(i).width(sizes[i]);
    }
    columns = {"Entry","Busy","Instruction","State","Destination","Value"};
    sizes = {45,45,120,120,90,60};
    for(unsigned int i = 0 ; i < columns.size() ; i++)
    {
        rob.append_header(columns[i]);
        rob.column_at(i).width(sizes[i]);
    }

    srand(static_cast <unsigned> (time(0)));

    FileOut testFile;
    string local_file = string(get_current_dir_name())+program_selected+".csv";
    if( testFile.check_file_exist(local_file )){
        cout  << "Carregando Banco de Registradores[ " << local_file << "] !" << endl;
        std::vector<string> lines = testFile.read_file_csv(program_selected+".csv");
        int ct = 4;
        for(int i = 0 ; i < 32 ; i++){
                reg_gui.at(i).text(1, lines[ct+1]);
                reg_gui.at(i).text(2,"0");
                reg_gui.at(i).text(4, lines[ct+3]);
                reg_gui.at(i).text(5,"0");
                ct += 4;
        }
       
    }else{
        cout << endl << "Criando Banco de Registradores !" << endl;
        string lineRegister = "Register-R,Value-R,Register-F,Value-F\n";
        for(int i = 0 ; i < 32 ; i++)
        {
            if(i){
                string val = std::to_string(rand()%100);
                lineRegister +=  "R" + std::to_string(i) + "," + val;
                reg_gui.at(i).text(1,val);
            }else{
                reg_gui.at(i).text(1,std::to_string(0));
                lineRegister += "R" + std::to_string(i) + "," + "0";
            }
            reg_gui.at(i).text(2,"0");
            string val2 = std::to_string(static_cast <float> (rand()) / static_cast <float> (RAND_MAX/100.0));
            lineRegister += ",F" + std::to_string(i) + "," + val2;
            reg_gui.at(i).text(4,val2);
            reg_gui.at(i).text(5,"0");
            lineRegister += "\n";
        }
        FileOut saveRegister("/out/value_register_input.csv");
        saveRegister.add_str(lineRegister);
    }
    
    FileOut testF;
    string fileMemory = string(get_current_dir_name())+"/out/value_memory_input.csv";
    if( testF.check_file_exist(fileMemory)){
        cout << "Load values of memory " << endl;
        std::vector<string> lines = testF.read_file_csv("/out/value_memory_input.csv");
        for(int i = 0; i < 500; i++){
            memory.Push(lines[i]);
        }
    }else{
        string lineMemory = "";
        for(int i = 0 ; i < 500 ; i++){
            string strM = std::to_string(rand()%100);
            memory.Push(strM);
            if(i < (500-1) )
                lineMemory += strM + ",";
            else
                lineMemory += strM;
        }
        FileOut saveRegister("/out/value_memory_input.csv");
        saveRegister.add_str(lineMemory);
    }
    for(int k = 1; k < argc; k+=2)
    {
        int i;
        if (strlen(argv[k]) > 2)
            show_message("Opção inválida",string("Opção \"") + string(argv[k]) + string("\" inválida"));
        else
        {
            char c = argv[k][1];
            switch(c)
            {
                case 'q':
                    inFile.open(argv[k+1]);
                    if(!add_instructions(inFile,instruction_queue,instruct))
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                        fila = true;
                    break;
                case 'i':
                    inFile.open(argv[k+1]);
                    int value;
                    i = 0;
                    if(!inFile.is_open())
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                    {
                        while(inFile >> value && i < 32)
                        {
                            reg_gui.at(i).text(1,std::to_string(value));
                            i++;
                        }
                        for(; i < 32 ; i++)
                            reg_gui.at(i).text(1,"0");
                        inFile.close();
                    }
                    break;
                case 'f':
                    float value_fp;
                    i = 0;
                    inFile.open(argv[k+1]);
                    if(!inFile.is_open())
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                    {
                        while(inFile >> value_fp && i < 32)
                        {
                            reg_gui.at(i).text(4,std::to_string(value_fp));
                            i++;
                        }
                        for(; i < 32 ; i++)
                            reg_gui.at(i).text(4,"0");
                        inFile.close();
                    }
                    break;
                case 'm':
                    inFile.open(argv[k+1]);
                    if(!inFile.is_open())
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                    {
                        int value;
                        i = 0;
                        while(inFile >> value && i < 500)
                        {
                            memory.Set(i,std::to_string(value));
                            i++;
                        }
                        for(; i < 500 ; i++)
                        {
                            memory.Set(i,"0");
                        }
                        inFile.close();
                    }
                    break;
                case 'r':
                    inFile.open(argv[k+1]);
                    if(!inFile.is_open())
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                    {
                        int value;
                        if(inFile >> value && value <= 10 && value > 0)
                            nadd = value;
                        if(inFile >> value && value <= 10 && value > 0)
                            nmul = value;
                        if(inFile >> value && value <= 10 && value > 0)
                            nls = value;
                        inFile.close();
                    }
                    break;
                case 's':
                    spec = true;
                    set_spec(plc,spec); 
                    spec_ip.checked(true);
                    k--;
                    break;
                case 'l':
                    inFile.open(argv[k+1]);
                    if(!inFile.is_open())
                        show_message("Arquivo inválido","Não foi possível abrir o arquivo!");
                    else
                    {
                        string inst;
                        int value;
                        while(inFile >> inst)
                        {
                            if(inFile >> value && instruct_time.count(inst))
                                instruct_time[inst] = value;
                        }
                    }
                    inFile.close();
                    break;
                default:
                    show_message("Opção inválida",string("Opção \"") + string(argv[k]) + string("\" inválida"));
                    break;
            }
        }
    }
    clock_control.enabled(false);
    botao.events().click([&]
    {
        if(fila)
        {
            botao.enabled(false);
            clock_control.enabled(true);
            //Desativa os menus apos inicio da execucao
            op.enabled(0,false);
            op.enabled(1,false);
            op.enabled(3,false);
            for(int i = 0 ; i < 6 ; i++)
                sub->enabled(i,false);
            for(int i = 0 ; i < 5 ; i++)
                bench_sub->enabled(i,false);
            if(spec)
                top1.rob_mode(nadd,nmul,nls,instruct_time,instruction_queue,table,memory,reg,instruct,clock_count,rob);
            else
                top1.simple_mode(nadd,nmul,nls,instruct_time,instruction_queue,table,memory,reg,instruct,clock_count);
            sc_start();
        }
        else{
                show_message("Fila de instruções vazia","A fila de instruções está vazia. Insira um conjunto de instruções para iniciar.");
        }
    });
    clock_control.events().click([]
    {
        if(sc_is_running())
            sc_start();
    });
    exit.events().click([]
    {
        sc_stop();
        API::exit();
    });

    running_all.events().click([&]{
        while(sc_is_running()){
            sc_start();
        }
        show_message("Status de execucao","Execucao concluida com sucesso !");
        string lineRegister = "Register-R,Value-R,Register-F,Value-F\n";
        for(int i = 0 ; i < 32 ; i++){
           
            lineRegister += "R" +std::to_string(i)+ "," +reg_gui.at(i).text(1);
            lineRegister += ",F" +std::to_string(i)+ "," +reg_gui.at(i).text(4);
            lineRegister += "\n";
        }
        FileOut saveRegister("/out/value_register_output.csv");
        saveRegister.add_str(lineRegister);
    });

    fm.show();
    exec();
    return 0;
}
