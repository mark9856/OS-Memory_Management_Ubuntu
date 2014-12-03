/******
 Group members:
 - Huiyu Ma (RIN: 661133289)
 - Teng Liu (RIN: 661088506)
 ******/

#include <string>
#include <map>
#include <list>
#include <vector>
#include "process.h"

class Memory {
public:
    Memory(int s);
    void setAlgorithm(std::string algo) {algorithm = algo;}
    void addProcess(Process& p);
    int runOneSecond(int time_elapse);
    int first(Process p);
    int best(Process p);
    int next(Process p);
    int worst(Process p);
    int noncontig(Process p);
    int defrag(int time_elapse);
    void printMemory(int event, int time_elapse);
    void printMemory(int event, Process& p, int time_elapse);
    int leave(Process p);
    bool removeFromSizeMap(int old_addr);
    void printMap();
    bool addSizeMap(int new_addr, int new_size);
private:
    int size;
    std::string algorithm;
    std::map<float, int> size_map;
    std::map<int, int> addr_map;
    std::vector<Process> processes;
    std::vector<char> table;
    int current_process_addr;
};
