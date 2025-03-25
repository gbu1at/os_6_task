
gen_sample: util.o gen.o
	gcc -o gen_sample gen.o util.o -O0 -fno-omit-frame-pointer -no-pie
	chmod u+x gen_sample

util.o: util.asm
	nasm -felf64 util.asm -o util.o -O0

gen.o: gen.c
	gcc -O0 -fno-omit-frame-pointer -no-pie -o gen.o -c gen.c

clean:
	rm *.o gen_sample

