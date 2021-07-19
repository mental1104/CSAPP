## Chapter 12 Concurrent Programming  

Usage:  
`gcc -o server echoserver(p|s|t).c -pthread`  
`./server 1024`  
`./client localhost 1024`  

### 0. Doesn't support concurrency  

[see the version of chapter11:](../chapter11/echoserver.c)  
![](./server_no_concurrency.png)


### 1. With processes  

[echoserverp.c](./echoserverp.c)  
![](./serverp.png)  

### 2. With I/O multiplexing  

[echoservers.c](./echoservers.c)  
![](./servers.png)  

### 3. With threads  

[echoservert.c](./echoservert.c)  
![](./servert.png)  

### 4. Using semaphore to sychronize  

[badcnt.c](./badcnt.c)

```shell
➜  chapter12 git:(master) ✗ gcc -o badcnt badcnt.c -pthread
➜  chapter12 git:(master) ✗ ./badcnt
Usage: ./badcnt <niters>
➜  chapter12 git:(master) ✗ ./badcnt 1999
OK cnt=3998
➜  chapter12 git:(master) ✗ ./badcnt 1000
OK cnt=2000
➜  chapter12 git:(master) ✗ ./badcnt 10000
BOOM! cnt=15835
➜  chapter12 git:(master) ✗ ./badcnt 5000
OK cnt=10000
➜  chapter12 git:(master) ✗ ./badcnt 7000
OK cnt=14000
➜  chapter12 git:(master) ✗ ./badcnt 8000
BOOM! cnt=11724
```

Using semaphore:

`sem_t mutex;`  

`Sem_init(&mutex, 0 ,1);`   with P, V;

[goodcnt.c](./goodcnt.c)  
```shell
➜  chapter12 git:(master) ✗ ./goodcnt 1000000
OK cnt=2000000
```

### 5. Prethreading  

Relative file: 

+ [echo_cnt.c](./echo_cnt.c)
+ [echoservert-pre.c](./echoservert-pre.c)
+ [sbuf.c](./sbuf.c)
+ [sbuf.h](./sbuf.h)

However, there's a multiple definition problem that haunts me...

### 6. psum with mutex, array and local variable

+ [psum.c](./psum.c)  

Usage: `gcc -o psum psum.c -O0 -pthread`  

result:   
36.541364 399.397656 1494.803441 2620.637289 2713.644104 2733.520022   
4.797576 9.801893 19.810466 29.753811 22.701177 14.509155   
4.855713 4.924726 4.985449 5.245657 5.460248 5.542412    

Conclusion: local variable > memory reference > mutex lock  

