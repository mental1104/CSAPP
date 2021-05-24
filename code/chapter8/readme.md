# Chapter 8 Exceptional Control Flow  

## 8.6 Nonlocal Jumps  

[restart.c](./restart.c)  
Initially, I don't quite understand the importance of installing the handler after we call sigsetjmp:  
```c
if (!sigsetjmp(buf, 1)) {
    Signal(SIGINT, handler);
```
The author said it's to avoid a race, but I still cannot make it out.  

Race means the signal installation take place before sigsetjmp, so we let it before the sigsetjmp:  
```c
Signal(SIGINT, handler);
if (!sigsetjmp(buf, 1)) {        
	Sio_puts("starting\n");
}
```
Ok, let's compile and run it.  
`gcc restart.c -lpthread`  
`./a.out`  

```c
➜  chapter8 git:(master) ✗ ./a.out
starting
processing...
processing...
^Crestarting
processing...
processing...
processing...
processing...
^Crestarting
processing...
processing...
processing...
processing...
processing...
processing...
```
It works correctly, so here Signal installation doesn't win the race.  
But if we sleep 5 sec after Signal installation it will give us the chance to win the race:  
```c
Signal(SIGINT, handler);
sleep(5);
if (!sigsetjmp(buf, 1)) {        
	Sio_puts("starting\n");
}
```
compile and run:
```c
➜  chapter8 git:(master) ✗ ./a.out
^C[1]    7023 segmentation fault (core dumped)  ./a.out
```
It's because we use the longjump before we actually setjmp the environment variable, which results in a segmentation fault.  
 