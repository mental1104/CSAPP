#include<csapp.h>
#include<time.h>

#define MAXTHREADS 32

void *sum_mutex(void *vargp);
void *sum_array(void *vargp);
void *sum_local(void *vargp);


long gsum;
long psum[MAXTHREADS] = {};

long nelems_per_thread;
sem_t mutex;

int main(int argc, char** argv){
    if(argc!=2){
        printf("Two parameter.\n");
        return 0;
    }

    void*(*f)(void*) = NULL;
    int method = atoi(argv[1]);
    if(method == 0){
        f = sum_mutex;
    }
    else if(method == 1){
        f = sum_array;
    } 
    else 
        f = sum_local;

    long i, nelems, log_nelems = 31, nthreads, myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];

    time_t begin, end;

    sem_init(&mutex, 0, 1);

    nelems = (1L << log_nelems);

    for(i = 0; i < MAXTHREADS; i++)
        myid[i] = i;

    //mutex
    for(nthreads = 1; nthreads<=MAXTHREADS; nthreads = nthreads * 2){
        gsum = 0;
        nelems_per_thread = nelems / nthreads;
        begin = clock();

        for(i = 0; i < nthreads; i++)
            Pthread_create(&tid[i], NULL, f, &myid[i]);
        for(i = 0; i < nthreads; i++)
            Pthread_join(tid[i], NULL);

        end = clock(); 
        printf("%lf ", (double)(end-begin)/CLOCKS_PER_SEC);
        if(gsum != (nelems * (nelems-1))/2)
            printf("\nError: result=%ld\n",gsum);
    }

    printf("\n");

    /*
    if(gsum != (nelems * (nelems-1))/2)
        printf("Error: result=%ld\n",gsum);
    else    
        printf("Success. result=%ld, time for mutex: %lf\n", gsum, (double)(end-begin)/CLOCKS_PER_SEC);
    */

    exit(0);
}

void *sum_mutex(void *vargp){

    long myid = *((long *)vargp);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;
    long i;

    for(i = start; i < end; i++){
        P(&mutex);
        gsum += i;
        V(&mutex);
    }

    return NULL;
}

void *sum_array(void *vargp){
    long myid = *((long *)vargp);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;
    long i;

    for (i = start; i < end; i++)
        psum[myid]+=i;
    return NULL;
}

void *sum_local(void *vargp){
    long myid = *((long *)vargp);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;
    long i, sum = 0;

    for(i = start; i < end; i++)
        sum += i;
    
    psum[myid] = sum;
}