main: main.c
	gcc -g main.c -o main -lpthread

run: main
	./main

gdb: main
	gdb ./main

clean:
	rm -rf tmp.txt ./main
