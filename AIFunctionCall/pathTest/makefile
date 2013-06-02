CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

orange: orange.o
	g++  orange.o -o orange.out $(CFLAGS)

orange.o: orange.cpp
	g++ -c  orange.cpp $(CFLAGS)

clean:
	rm *.o
