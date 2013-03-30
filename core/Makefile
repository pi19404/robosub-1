CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g
all:
	clang core.cpp image_kb.cpp kb.cpp -o ai $(CFLAGS)

clean:
	rm -rf *.o
