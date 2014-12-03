Group members: 
  - Huiyu Ma (RIN: 661133289)
  - Teng Liu (RIN: 661088506)

We defined a Process class and a Memory class. 
Process class stores a process’s name, arrival times, leaving times, etc. 
Memory class implements all the algorithms including "first","next","best","worst", and "noncontiguous". 
main.cpp file reads in a txt file and prepare it for program operation.
There are two types of operations, one is quite model and the other is user model.
In our Memory class, we created two maps, one is size map, whose key is size, and the other map is addr map, whose key is address in the memory. the way we deal with the same size process is to set up the key of size map to be float. Both maps are used to storage the available address and size of the empty space.
Memory includes functions of runOneSecond, which is used for running each time_elapse; first, best, next, worst and noncotig totally 5 algorithms. In the runOneSecond function, each process will run the function of each algorithm when a process enter into the memory. and when a process leave the memory, the runOneSecond function will execute the leave function.