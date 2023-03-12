#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "lib.h"
#include "mm.h"

struct Flow {
    bool alloc;
    int index;
    int size;
    Flow():alloc(false), index(-1), size(0){}
};

struct Trace {
    void* ptr;
    int size;
};



int main(int argc, char**argv){
    if(argc!=3){
        printf("You should pass one extra variable to indicate the <testfile>\n");
        printf("Usage: %s <1/2> <time(0)/fragment(1)>\n", argv[0]);
        return 0;
    }
    
    int file = atoi(argv[1]);
    int kind = atoi(argv[2]);

    std::ifstream is;
    if(file == 1)
        is.open("./trace1.txt");
    else if(file == 2)
        is.open("./trace2.txt");
    else    
        return 0;
    int operation, allocTimes;
    is >> allocTimes >> operation ;
    Trace* trace = new Trace[allocTimes];
    Flow* flow = new Flow[operation]();
    int sum = 0;
    std::vector<int> frag;

    char allo;
    int index;
    int size;
    
    /* 集中处理文件读写 */
    for(int i = 0; i < operation; i++){
        is >> allo >> index;
        flow[i].index = index;
        if(allo == 'a'){
            is >> size;
            flow[i].alloc = true;
            flow[i].size = size;
        } else {
            flow[i].alloc = false;
        }
    }

    if(kind == 0){
    mem_init();
    mm_init();
    time_t begin, end;
    begin = clock();
    for(int i = 0; i < operation; i++){
        if(flow[i].alloc){
            trace[flow[i].index].ptr = mm_malloc(flow[i].size);
            //sum += temp;
            //std::cout << "Malloc at " << flow[i].index << " with size " << flow[i].size << std::endl;
        } else {
            mm_free(trace[flow[i].index].ptr);
            //sum -= trace[flow[i].index].size;
            //std::cout << "Free at " << flow[i].index << std::endl;
        }
    }
    end = clock();
    std::cout << double(end-begin)/CLOCKS_PER_SEC << " "; 
    mem_deinit();
    }

    else {
    mem_init();
    mm_init();

    int step = operation/100;
    for(int i = 0; i < operation; i++){
        if(flow[i].alloc){
            trace[flow[i].index].ptr = mm_malloc(flow[i].size);
        } else {
            mm_free(trace[flow[i].index].ptr);
        }
        
        if(i % step == 0){
            frag.push_back(get_fragment());
        }
    }
    frag.push_back(get_fragment());
    for(auto i : frag)
        std::cout << i << " ";

    std::cout << std::endl;
    }
    
    //std::cout << sum << std::endl;
    delete[] trace;
    delete[] flow;
    

    return 0;
}