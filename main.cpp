/******
 Group members:
 - Huiyu Ma (RIN: 661133289)
 - Teng Liu (RIN: 661088506)
 ******/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <cstdlib>

#include "memory.h"

#define MEMORY_SIZE 1600

//function prototype
void readIn(Memory& memory, const char* input_file);
void usage();

//global variable
int time_elapse = 0;

int main(int argc, char* argv[]) {
    
    if (argc != 3 && argc != 4) {
        std::cerr<<"USAGE: memsim [-q] <input-file> { first | best | next | worst }\n";
        return 1;
    }
    
    std::string algorithm;
    std::ifstream input;
    Memory M(MEMORY_SIZE);
    bool q = 0;//quiet-user flag
    int event = 0;//denote if an event happened in the last second
    //event: 0(nothing),1(process exit), 2(defragmentation), 3(outof memory), 4(process enter)
    int time = -1;//user input time
    
    if (argc == 3) {
        algorithm = argv[2];
        M.setAlgorithm(algorithm);
        readIn(M, argv[1]);
    } else {
        q = 1;
        algorithm = argv[3];
        M.setAlgorithm(algorithm);
        readIn(M, argv[2]);
    }


    //print memory at time 0
    // M.printMemory(0, 0);
    
    while (1) {
        if (!q) {//if not quiet-user mode
            if (time == 0) {//user need to type in a time
                if (time_elapse!=0&&time_elapse!=1)
                    M.printMemory(0, time_elapse-1);
                std::cout<<"\nPlease enter integer t:\n";
                std::cin >> time;
                if (time==0) {
                    break;
                }
            }
            //proceed one second for all processes
            //in the memory
            event = M.runOneSecond(time_elapse);
            
            if (time_elapse==0) {
                M.printMemory(0,time_elapse);
                time=1;
            }
            
            if (event != 0) {//if an event happens
                M.printMemory(event, time_elapse);
            }
            time_elapse++;
            time--;

        } else {//if quiet-user mode
            //proceed one second for all processes
            //in the memory
            event = M.runOneSecond(time_elapse);
            if (event != 0) {//if an event happens
                M.printMemory(event, time_elapse);
            }
            if (time_elapse==0) M.printMemory(event, time_elapse);
            time_elapse++;
        }
        if (event == 3) {
            break;
        }
    }
    
    return 0;
}

void readIn(Memory & memory, const char* input_file){
    std::ifstream file(input_file);
    std::string line;
    
    if(file.is_open()){
        int num_processes;
        getline(file, line);
        //num_processes = std::stoi(line);
        num_processes = atoi(line.c_str());
        
        while(getline(file, line)){
	    num_processes--;
            char process_name;
            int process_size;
            int arrive_time;
            int leave_time;
            
            std::istringstream iss(line);
            
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss),
                 std::istream_iterator<std::string>(),
                 back_inserter(tokens));
            process_name = tokens[0][0];
            //process_size = std::stoi(tokens[1]);
	    process_size = atoi(tokens[1].c_str());
            Process p(process_name, process_size);
            for (unsigned int i = 2; i < tokens.size(); i = i + 2)
            {
                /* code */
                //arrive_time = std::stoi(tokens[i]);
                //leave_time = std::stoi(tokens[i + 1]);
		arrive_time = atoi(tokens[i].c_str());
		leave_time = atoi(tokens[i+1].c_str());
                p.addTime(arrive_time, leave_time);
            }
            memory.addProcess(p);
            // std::cout << "Process Name: " << process_name << " process_size: " << process_size
            // 	<< " arrive_time: " << arrive_time << " leave_time: " << leave_time << std::endl;
        }
	if (num_processes!=0) {
	  std::cerr<<"ERROR: Process number incorrect!\n";
	  exit(1);
	}
        file.close();
    }
    else {
        std::cerr << "ERROR: Could not open " << input_file << " for reading." << std::endl;
        usage(); 
    }
}

void usage() {
    // std::cerr << "Usage: " << program_name << " <input_file> " << std::endl;
    std::cerr << "USAGE: memsim [-q] <input_file> { first | best | next | worst}" << std::endl;
    exit(1);
}
