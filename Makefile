
fpArithmetic.out: MyFloat.o main.o
	g++ -g -Wall -std=c++11 -o fpArithmetic.out MyFloat.o main.o

test.out: MyFloat.o mytest.o
	g++ -g -Wall -std=c++11 -o test.out MyFloat.o mytest.o

mytest.o: mytest.cpp MyFloat.h
	g++ -g -Wall -std=c++11 -c -o mytest.o mytest.cpp
	
main.o: main.cpp MyFloat.h
	g++ -g -Wall -std=c++11 -c -o main.o main.cpp
	
MyFloat.o: MyFloat.h MyFloat.cpp
	g++ -g -Wall -std=c++11 -c -o MyFloat.o MyFloat.cpp
	
clean:
	rm main.o MyFloat.o fpArithmetic.out test.out *.o
