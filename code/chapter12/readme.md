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







