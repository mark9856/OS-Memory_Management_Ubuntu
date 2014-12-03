/******
 Group members:
 - Huiyu Ma (RIN: 661133289)
 - Teng Liu (RIN: 661088506)
 ******/

#include <map>
#include <vector>
#include <string>

class Process {
public:
    Process() {}
    Process(char n, int s):name(n),size(s) {}
    bool addTime(int arrive, int leave);
    int getSize() const {return size;}
    char getName() const {return name;}
    std::map<int, int> getTime() const {return time;}
private:
    char name;
    int size;
    std::map<int, int> time;
};
