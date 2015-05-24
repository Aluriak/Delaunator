FLAGD=-DDEBUG -g
FLAGS=$(FLAGD) -DVERBOSE
#OPTMS=-O3 -flto

all: build v
build: compile link 
br: build r
rp: 
	dot DT_representation.dot -Tpng -o repr.png
	ristretto repr.png

compile: clearwraps
	g++ -lpython3 -I/usr/include/python3.2/ -std=c++0x -c *.cpp $(FLAGS) $(OPTMS)

link:
	g++ *.o  -I/usr/include/python3.2/ -std=c++0x $(FLAGS) $(OPTMS)


r:
	./a.out

v:
	valgrind --leak-check=full ./a.out

vt:
	valgrind --leak-check=full --track-origins=yes  ./a.out

vv:
	valgrind --leak-check=full -v ./a.out


clean:
	rm *.o  
clearwraps:
	- rm *_wrap.cpp
