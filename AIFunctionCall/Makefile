CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

checkImage: checkImage.o
	clang checkImage.o -ggdb -o checkImage $(CFLAGS)
	#g++ checkImage.o -o checkImage $(CFLAGS)

checkImage.o: checkImage.cpp
	clang -ggdb -c checkImage.cpp $(CFLAGS)
	#g++ -c checkImage.cpp $(CFLAGS)

debug:
	g++ -c -D DEBUG checkImage.cpp $(CFLAGS)
	g++ checkImage.o -o checkImage $(CFLAGS)

clean:
	rm *.o
	rm checkImage
