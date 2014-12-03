/******
 Group members:
 - Huiyu Ma (RIN: 661133289)
 - Teng Liu (RIN: 661088506)
 ******/

#include "process.h"

bool
Process::addTime(int arrive, int leave) {
    return (time.insert(std::make_pair(arrive, leave)).second);
}
