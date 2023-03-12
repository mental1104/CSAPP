all: segregated implicit explicit

segregated: main.o seg_list.o lib.o
	g++ -m32 -o $@ $^ -w

implicit: main.o first_fit.o lib.o
	g++ -m32 -o $@ $^ -w

explicit: main.o exp_list.o lib.o
	g++ -m32 -o $@ $^ -w

main.o: main.cpp mm.h
	g++ -m32 -o $@ -c $< -w

seg_list.o: seg_list.c lib.h
	gcc -m32 -o $@ -c $< -w

first_fit.o: first_fit.c lib.h
	gcc -m32 -o $@ -c $< -w

exp_list.o: exp_list.c lib.h
	gcc -m32 -o $@ -c $< -w

lib.o: lib.c lib.h
	gcc -m32 -o $@ -c $< -w

.PHONY: clean
clean:
	rm segregated implicit explicit seg_list.o first_fit.o exp_list.o main.o lib.o
