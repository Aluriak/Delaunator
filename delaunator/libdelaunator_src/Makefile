#FLAGS=-DDEBUG -DVERBOSE
OPTMS=-O3 -flto

all: compile link v

compile: clearwraps
	g++ -lpython3 -I/usr/include/python3.2/ -std=c++0x -c *.cpp $(FLAGS) $(OPTMS)

link:
	g++ *.o  -I/usr/include/python3.2/ -std=c++0x $(FLAGS) $(OPTMS)


r:
	./a.out

v:
	valgrind --leak-check=full ./a.out

vv:
	valgrind --leak-check=full -v ./a.out


clean:
	rm *.o  
clearwraps:
	- rm *_wrap.cpp
