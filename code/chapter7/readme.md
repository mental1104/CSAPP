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

