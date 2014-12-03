/******
 Group members:
 - Huiyu Ma (RIN: 661133289)
 - Teng Liu (RIN: 661088506)
 ******/

#include <math.h>
#include <iostream>
#include <iomanip>
#include "memory.h"


Memory::Memory(int s) {
    size = s;
    size_map.insert(std::make_pair(size-80, 80));
    addr_map.insert(std::make_pair(80, size-80));
    current_process_addr = 79;
    for (int i = 0; i < 80; i++) {
        table.push_back('#');
    }
    for (int i = 80; i < size; i++) {
        table.push_back('.');
    }
}
// add process to the memory
void
Memory::addProcess(Process& p) {
    processes.push_back(p);
}

// run One second
int Memory::runOneSecond(int time_elapse) {
    int event = 0;
    // deal with process leave
    for(unsigned int i = 0; i < processes.size(); i ++){
        std::map<int, int>::const_iterator process_it;
        std::map<int, int> process_time = processes[i].getTime();        
        for(process_it = process_time.begin(); process_it != process_time.end(); ++ process_it){
            if(process_it -> second == time_elapse){
                // std::cout << "run leave" << std::endl;
                leave(processes[i]);
                printMemory(1, processes[i], time_elapse);
            }
        }
    }

    // current_process_addr = 0;
    // deal with process arrive
    bool enter = 0;
    for(unsigned int i = 0; i < processes.size(); i ++){
        std::map<int, int>::const_iterator process_it;
        std::map<int, int> process_time = processes[i].getTime();
        for(process_it = process_time.begin(); process_it != process_time.end(); ++ process_it){
            if(process_it -> first == time_elapse && time_elapse != 0) {
                enter = 1;
                std::cout << "\nProcess \'"<<processes[i].getName()<<"\' enters at time "<<time_elapse<<"\n";
            }
            if(process_it -> first == time_elapse){
                if(algorithm == "first"){
                    event = first(processes[i]);
                }
                else if(algorithm == "next"){
                    event = next(processes[i]);
                }
                else if(algorithm == "best"){
                    event = best(processes[i]);
                }
                else if(algorithm == "worst"){
                    event = worst(processes[i]);
                }
                else if(algorithm == "noncontig"){
                    event = noncontig(processes[i]);
                }
                
                if (event==3) {//out of memory
                    if (algorithm != "non") {
                        std::cout << "OUT-OF-MEMORY\n" << std::endl;
                        defrag(time_elapse);
                        printMemory(0, time_elapse);
                        if(algorithm == "first"){
                            event = first(processes[i]);
                        }
                        else if(algorithm == "next"){
                            event = next(processes[i]);
                        }
                        else if(algorithm == "best"){
                            event = best(processes[i]);
                        }
                        else if(algorithm == "worst"){
                            event = worst(processes[i]);
                        }
                        if (event!=3) printMemory(2,time_elapse);
                    }
                } else if (event == 3) return 3;//out of memory
            }
        }
    }
    if (enter) return 4;
    else return event;
}

// use first algorithm
int Memory::first(Process p) {
    std::map<int, int>::const_iterator addr_it;
    // go throung addr_map from begining to end
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        // found the first empty space bigger than process space
        if(addr_it -> second > p.getSize()){
            int i = 0;
            // set table for coming process
            for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                table[i] = p.getName();
            }
            // record old and new addr, old and new size
            int old_addr = addr_it -> first;
            int new_addr = i;
            int new_size = (addr_it -> second) - p.getSize();
            // erase and insert to addr_map
            addr_map.erase(addr_it -> first);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            // break;
            return 0;
        }
        // found the first empty space equal to process space
        else if(addr_it -> second == p.getSize()){
            // set table for coming process
            for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old addr
            int old_addr = addr_it -> first;
            // erase old from addr_map and size_map
            addr_map.erase(addr_it -> first);
            // erase from size_map
            removeFromSizeMap(old_addr);
            // break;
            return 0;
        }
    }
    // return out of memory
    return 3;
}

int Memory::best(Process p) {
    std::map<float, int>::const_iterator size_it;
    // go throung size_map from small to big
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        // found the first empty space equal to process space
        if(floor(size_it -> first) == p.getSize()){
            // set table for coming process
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old addr
            int old_addr = size_it -> second;
            // erase old from addr_map and size_map
            addr_map.erase(old_addr);
            // erase from size_map
            removeFromSizeMap(old_addr);            
            // break;
            return 0;
        }
        // found the first empty space bigger than process space
        else if(floor(size_it -> first) > p.getSize()){
            // set table for coming process
            for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
                table[i] = p.getName();
            }
            // record old and new addr, old and new size
            int old_addr = size_it -> second;
            int old_size = floor(size_it -> first);
            int new_addr = old_addr + p.getSize();
            int new_size = old_size - p.getSize();
            // erase and insert to addr_map
            addr_map.erase(old_addr);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            // break;
            return 0;
        }
    }
    // return out of memory
    return 3;
}

int Memory::next(Process p) {
    // whether reach the last slot
    bool got = false;
    std::map<int, int>::const_iterator addr_it;
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        if(addr_it -> first <= current_process_addr){
            continue;
        }
        else if(addr_it -> first > current_process_addr){
            // found the first empty space bigger than process space
            if(addr_it -> second > p.getSize()){
                // std::cout << "bigger" << std::endl;
                int i = 0;
            // set table for coming process
                for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                    table[i] = p.getName();
                }
            // record old and new addr, old and new size
                int old_addr = addr_it -> first;
                int new_addr = i;
                int new_size = (addr_it -> second) - p.getSize();
            // erase and insert to addr_map
                addr_map.erase(addr_it -> first);
                addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
                // erase from size_map
                removeFromSizeMap(old_addr);
                // add new pair to size_map
                addSizeMap(new_addr, new_size);
                current_process_addr = old_addr;
                got = true;
                // break;
                return 0;
            }
            // found the first empty space equal to process space
            else if(addr_it -> second == p.getSize()){
                std::cout << "equal" << std::endl;
            // set table for coming process
                for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                    table[i] = p.getName();
                }
            // record old addr
                int old_addr = addr_it -> first;
            // erase old from addr_map and size_map
                addr_map.erase(addr_it -> first);
                // erase from size_map
                removeFromSizeMap(old_addr);
                current_process_addr = old_addr;
                got = true;
                // break;
                return 0;
            }
        }
    }
    // if reach the last slot
    if(got == false){
        current_process_addr = 79;
        // next(p);
        // std::map<int, int>::const_iterator addr_it;
        for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
            if(addr_it -> first <= current_process_addr){
                continue;
            }
            else if(addr_it -> first > current_process_addr){
                // found the first empty space bigger than process space
                if(addr_it -> second > p.getSize()){
                    // std::cout << "bigger" << std::endl;
                    int i = 0;
                // set table for coming process
                    for(i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){                // else{
                        table[i] = p.getName();
                    }
                // record old and new addr, old and new size
                    int old_addr = addr_it -> first;
                    int new_addr = i;
                    int new_size = (addr_it -> second) - p.getSize();
                // erase and insert to addr_map
                    addr_map.erase(addr_it -> first);
                    addr_map.insert(std::make_pair(new_addr, new_size));
                // erase and insert to size_map
                    // erase from size_map
                    removeFromSizeMap(old_addr);
                    // add new pair to size_map
                    addSizeMap(new_addr, new_size);
                    current_process_addr = old_addr;
                    got = true;
                    // break;
                    return 0;
                }
                // found the first empty space equal to process space
                else if(addr_it -> second == p.getSize()){
                    std::cout << "equal" << std::endl;
                // set table for coming process
                    for(int i = addr_it -> first; i < addr_it -> first + p.getSize(); ++ i){
                        table[i] = p.getName();
                    }
                // record old addr
                    int old_addr = addr_it -> first;
                // erase old from addr_map and size_map
                    addr_map.erase(addr_it -> first);
                    // erase from size_map
                    removeFromSizeMap(old_addr);
                    current_process_addr = old_addr;
                    got = true;
                    // break;
                    return 0;
                }
            }
        }
    }
    // return out of memory
    return 3;
}

int Memory::worst(Process p) {
    std::map<float, int>::const_iterator size_it;
    size_it = size_map.end();
    -- size_it;
    if(floor(size_it -> first) > p.getSize()){
            // set table for coming process
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
            // record old and new addr, old and new size
        int old_addr = size_it -> second;
        int old_size = floor(size_it -> first);
        int new_addr = old_addr + p.getSize();
        int new_size = old_size - p.getSize();
            // erase and insert to addr_map
        addr_map.erase(old_addr);
        addr_map.insert(std::make_pair(new_addr, new_size));
            // erase and insert to size_map
        // erase from size_map
        removeFromSizeMap(old_addr);
        // add new pair to size_map
        addSizeMap(new_addr, new_size);
    }
    else if(floor(size_it -> first) == p.getSize()){
            // set table for coming process
        for(int i = size_it -> second; i < size_it -> second + p.getSize(); ++ i){
            table[i] = p.getName();
        }
            // record old addr
        int old_addr = size_it -> second;
            // erase old from addr_map and size_map
        addr_map.erase(old_addr);
        // erase from size_map
        removeFromSizeMap(old_addr);            
    }
    else if(floor(size_it -> first) < p.getSize()){
        // return out of memory
        return 3;
    }
    return 0;    
}

int Memory::noncontig(Process p) {
    std::map<int, int>::iterator addr_itr = addr_map.begin();
    int size_needed = p.getSize();
    while (size_needed > 0) {
        if (addr_map.size()==0||
            (addr_map.size()==1&&addr_itr->second < size_needed)) {
            //out of memory
            return 3;
        }
        int begin_addr = addr_itr->first;
        int free_addr_size = addr_itr->second;
        int end_addr = 0;
        if (size_needed<free_addr_size) {
            end_addr = begin_addr + size_needed;
            //allocate memory table (p, begin_addr, end_addr);
            for (int i = begin_addr; i < end_addr; i++) {
                table[i] = p.getName();
            }
            addr_map.erase(addr_itr);
            addr_itr = addr_map.insert(std::make_pair(end_addr, free_addr_size-size_needed)).first;
            return 0;
        } else {
            end_addr = begin_addr+free_addr_size;
            //allocate memory table (p, begin_addr, end_addr);
            for (int i = begin_addr; i < end_addr; i++) {
                table[i] = p.getName();
            }
	    std::map<int, int>::iterator addr_tmp = addr_itr;
	    addr_tmp++;
            addr_map.erase(addr_itr);
	    addr_itr = addr_tmp;
            size_needed -=free_addr_size;
        }
    }
    //update size_map
    addr_itr = addr_map.begin();
    size_map.clear();
    for (; addr_itr != addr_map.end(); addr_itr++) {
        float size_tmp = float(addr_itr->second);
        int addr_tmp = addr_itr->first;
        while (size_map.insert(std::make_pair(size_tmp, addr_tmp)).second == 0) {
            size_tmp += 1/size;
        }
    }
    return 0;
}

int Memory::defrag(int time_elapse) {
    std::cout<<"Performing defragmentation...\n";
    int relocate = 0;
    //modify addr_map
    int free_memory = 0;
    std::map<int, int>::iterator itr = addr_map.begin();
    for (; itr != addr_map.end(); itr++) {
        free_memory += itr->second;
    }
    //free_memory -= (--itr)->second;
    addr_map.clear();
    addr_map.insert(std::make_pair(size-free_memory, free_memory));
    //modify size_map
    size_map.clear();
    size_map.insert(std::make_pair(free_memory, size-free_memory));
    
    //modify table
    int base = 80;
    for (unsigned int i = 0; i < processes.size(); i++) {
        //check if the process need to be relocated
        /*
        if (table[base] == processes[i].getName() &&
            table[base+processes[i].getSize()-1] ==
            processes[i].getName()) {
            base += processes[i].getSize();
            std::cout<<"HERE1\n";
            continue;//no need to relocate
        }
        */
        std::map<int, int> time_map = processes[i].getTime();
        std::map<int, int>::iterator itr_t = time_map.begin();
        for (; itr_t != time_map.end(); itr_t++) {
            if (itr_t->first<time_elapse &&
                itr_t->second>time_elapse) {
                break;
            }
        }
        //this process is running
        if (itr_t != time_map.end()) {
            if (table[base] == processes[i].getName() &&
                table[base+processes[i].getSize()-1] ==
                processes[i].getName()) {
                base += processes[i].getSize();
                continue;//no need to relocate
            }
            relocate++;
            for (int offset = 0; offset < processes[i].getSize(); offset++) {
                table[base+offset] = processes[i].getName();
            }
            base += processes[i].getSize();
        }
    }
    
    for (int i = (--addr_map.end())->first; i < size; i++) {
        table[i]='.';
    }
    
    std::cout<<"Defragmentation completed.\n";
    std::cout<<"Relocated "<<relocate
    <<" processes to create a free memory block of "
    <<free_memory<<" units ("
    <<std::setprecision(4)
    <<float(free_memory)/float(size)*100
    <<"\% of total memory).\n\n";
    return 1;

}
// when the process leave return current address
int Memory::leave(Process p){
    // if algorithm is not equal to noncontig
    //if(algorithm != "noncontig"){
    // record current address
    int current_addr;
    for(int i = 0; i < size; ++ i){
        if(table[i] == p.getName()){
            current_addr = i;
            // std::cout << "current_addr is " << current_addr << std::endl;
            break;
        }
    }
    // release the memory space after the process leave
    for(int i = 0; i < size; ++ i){
        if(table[i] == p.getName()){
            table[i] = '.';
        }
    }

    // create new empty space in both maps
    addr_map.insert(std::make_pair(current_addr, p.getSize()));

    int new_addr = current_addr;

    int new_size = p.getSize();
    // while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
    //     temp_size += 0.01;
    //     // std::cout << temp_size << std::endl;
    //     // std::cout << "Wooo" << std::endl;
    // }
    // add new pair to size_map
    addSizeMap(new_addr, new_size);


    // attach the empty space together
    std::map<int, int>::const_iterator addr_it;
    int space = p.getSize();
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        // attach the empty space before
        if(addr_it -> first + addr_it -> second == current_addr){
        // record old and new addr, old and new size
            int old_addr = addr_it -> first;
            int new_addr = addr_it -> first;
            int new_size = addr_it -> second + space;
            // remove the empty space before current space
            addr_map.erase(addr_it -> first);
            // remove current empty space
            addr_map.erase(current_addr);
            // erase from size_map
            removeFromSizeMap(current_addr);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            current_addr = new_addr;
            space = new_size;
            break;
        }

    }// print table
    // printMap();
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        // attach the empty space behide
        if(addr_it -> first == current_addr + space){
        // record old and new addr, old and new size
            int old_addr = addr_it -> first;
            int new_addr = addr_it -> first - space;
            int new_size = addr_it -> second + space;
            addr_map.erase(addr_it -> first);
            // remove current empty space
            addr_map.erase(current_addr);
            // erase from size_map
            removeFromSizeMap(current_addr);
            addr_map.insert(std::make_pair(new_addr, new_size));
            // erase from size_map
            removeFromSizeMap(old_addr);
            // add new pair to size_map
            addSizeMap(new_addr, new_size);
            break;
        }
    }// print table
    return 0;
}

// add new pair to size_map
bool Memory::addSizeMap(int new_addr, int new_size){
    float temp_size = new_size;
    while(size_map.insert(std::make_pair(temp_size, new_addr)).second != 1) {
        temp_size += float(1)/float(size);
    }
    return true;
}

// erase from size_map
bool Memory::removeFromSizeMap(int old_addr){
    std::map<float, int>::const_iterator size_it;
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        if(size_it -> second == old_addr){
            size_map.erase(size_it -> first);
            return true;
        }
    }
    return false;
}

// print table
void Memory::printMap(){
    std::map<float, int>::const_iterator size_it;
    std::cout << "size_map is " << std::endl;
    for(size_it = size_map.begin(); size_it != size_map.end(); ++ size_it){
        std::cout << size_it -> first << ", " << size_it -> second << std::endl;
    }
    std::cout << std::endl;
    std::map<int, int>::const_iterator addr_it;
    std::cout << "addr_map is " << std::endl;
    for(addr_it = addr_map.begin(); addr_it != addr_map.end(); ++ addr_it){
        std::cout << addr_it -> first << ", " << addr_it -> second << std::endl;
    }
    std::cout << std::endl;
}

void Memory::printMemory(int event, int time_elapse) {
    if (event == 0 || event == 2||event==4) {//normal print
        std::cout << "Memory at time "<<time_elapse<<":";
        for (int i = 0; i < size; i++) {
            if (i%80==0) std::cout << "\n";
            std::cout << table[i];
        }
    } else if (event == 3) {//out of memory
        std::cout << "OUT-OF-MEMORY" << std::endl;
    }
    std::cout << "\n";
}

void Memory::printMemory(int event, Process& p, int time_elapse) {
    if (event == 1) {//process exit
        std::cout << "\nProcess \'"<<p.getName()<<"\' exits at time "<<time_elapse<<"\n";
        printMemory(0, time_elapse);
    } else if (event == 4) {//process enter
        std::cout << "\nProcess \'"<<p.getName()<<"\' enters at time "<<time_elapse<<"\n";
        printMemory(0, time_elapse);
    }
}



