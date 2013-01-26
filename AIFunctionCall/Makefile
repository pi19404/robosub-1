CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

checkImage: checkImage.o
	g++ checkImage.o -o checkImage $(CFLAGS)

checkImage.o: checkImage.cpp
	g++ -c checkImage.cpp $(CFLAGS)

debug:
	g++ -c -D DEBUG checkImage.cpp $(CFLAGS)
	g++ checkImage.o -o checkImage $(CFLAGS)
