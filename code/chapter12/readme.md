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



