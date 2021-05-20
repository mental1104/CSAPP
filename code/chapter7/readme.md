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