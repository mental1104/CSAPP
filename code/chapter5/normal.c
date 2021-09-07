#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int main(int argc, char **argv){
    if(argc!=2){
        printf("Usage: %s <limit>\n", argv[0]);
        return 0;
    }

    long limit = atol(argv[1]);

    unsigned long sum = 0;

    time_t start, end;

    start = clock();
    for(int i = 0; i < limit; i++){
        sum += i;
    }
    end = clock();
    printf("Time: %lf\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}