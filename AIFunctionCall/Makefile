CFLAGS = `pkg-config opencv --libs --cflags` -Wall -g

checkImage: checkImage.o
	clang checkImage.o -ggdb -o checkImage.out $(CFLAGS)

checkImage.o: checkImage.cpp
	clang -ggdb -c -D TEST checkImage.cpp $(CFLAGS)

debug:
	clang -c -D TEST -D DEBUG checkImage.cpp $(CFLAGS)
	clang checkImage.o -o checkImage $(CFLAGS)

clean:
	rm *.o
	rm checkImage
