
# Standard variables.
CXX     = g++ 
CFLAGS += -Wall -Wextra -std=c++0x 
OUT     = run
LDFLAGS = -lIrrlicht 

# Custom variables.
obj	    = .Input.o .Actor.o .Engine.o
tmp     = ${CXX} ${CFLAGS} ${LDFLAGS}
link    = ${tmp} -o ${OUT} ${obj}
compile = ${tmp} -c

run: main.cpp ${obj}
	${link} main.cpp 

.Input.o : Input.*
	${compile} Input.cpp -c -o .Input.o

.Actor.o : Actor.*
	${compile} Actor.cpp -c -o .Actor.o

.Engine.o : Engine.*
	${compile} Engine.cpp -c -o .Engine.o
