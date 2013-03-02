CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

orange: orange.o
	g++  orange.o -o orange $(CFLAGS)

orange.o: orange.cpp
	g++ -c  orange.cpp $(CFLAGS)

clean:
	rm *.o
