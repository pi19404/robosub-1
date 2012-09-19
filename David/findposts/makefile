CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

findposts2: findposts2.o
	g++  findposts2.o -o findposts2 $(CFLAGS)

findposts2.o: findposts2.cpp
	g++ -c  findposts2.cpp $(CFLAGS)
