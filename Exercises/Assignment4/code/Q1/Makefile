#
# example Makefile to build an executable, taken from Assignment 1 
#

all: 
	mpic++ -Wall -std=c++0x ${PROG}.cpp -o ${PROG}

run: 
	mpiexec --hostfile hosts ${PROG}         

run-local: 
	mpiexec -n 32 ${PROG}

clean:
	$(RM) ${PROG}
