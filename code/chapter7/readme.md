# Chapter 7 Linking  

## 7.6 Symbol Resolution  

### 7.6.2 Linking with Static Libraries  

File list:  
+ [addvec.c](./addvec.c)
+ [multvec.c](./multvec.c)
+ [vector.h](./vector.h)
+ [main2.c](./main2.c)  

`gcc -c addvec.c multvec.c`  
`ar rcs libvector.a addvec.o multvec.o`  
`gcc -c main2.c`  
`gcc -static main2.o ./libvector.a`  
`./a.out`  
As expected, the result is `z = [4 6]`  

Or equivalently,  
`gcc -static main2.o -L. -lvector`  

## 7.13 Library Interpositioning  

File list: 
+ [int.c](./int.c)  
+ [malloc.h](./malloc.h)  
+ [mymalloc.c](./mymalloc.c)  

### 7.13.1 Compile-Time Interpositioning  

`gcc -DCOMPILETIME -c mymalloc.c`  
`gcc -I. -o intc int.c mymalloc.o`  

```bash
➜  chapter7 git:(master) ✗ ./intc
malloc(32) = 0x560fff47e2a0
free(0x560fff47e2a0)
```

### 7.13.2 Link-Time Interpositioning  

`gcc -DLINKTIME -c mymalloc.c`  
`gcc -c int.c`  
`gcc -Wl,--wrap,malloc -Wl,--wrap,free -o intl int.o mymalloc.o`  

### 7.13.3 Run-Time Interpositioning  

`gcc -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl`  
`gcc -o intr int.c`  
`LD_PRELOAD="./mymalloc.so" ./intr`  

Notice that I intentionally annotated the `printf`. see [this](https://blog.csdn.net/baidu_30073577/article/details/89453567) to find the reason why.  


### Exercise 7.13  

A. 
```zsh
➜  ~ ar -t /usr/lib/x86_64-linux-gnu/libc.a | wc -l
1740
```  
B. 
```zsh
➜  ~ touch test.c
➜  ~ vim test.c
➜  ~ gcc -Og -o a.out test.c
➜  ~ gcc -Og -g -o b.out test.c
➜  ~ sha256sum a.out b.out
aa89cc84b4979bf558436a24c26925832a2a6f15086d2697d2cfb9e6bdf28fb7  a.out
f423cfb6f244c93e004fcd43b99aee28cca0d941869c2335a71455ff2485ad4a  b.out
```
C.  
```
➜  ~ ldd /usr/bin/gcc
	linux-vdso.so.1 (0x00007ffd4dcf7000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007eff351a9000)
	/lib64/ld-linux-x86-64.so.2 (0x00007eff353b3000)
```
