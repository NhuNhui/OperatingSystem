CFLAGS = -Wno-implicit-function-declaration

all: final

final: seqlock.o
	g++ seqlock.o -o final
	@chmod +x final
	./final

seqlock.o: seqlock.c
	g++ -c seqlock.c

clean:
	@echo "Removing everything but the source files"
	rm *.o final
