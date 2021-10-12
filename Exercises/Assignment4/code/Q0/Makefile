#
# example Makefile to build an executable, taken from Assignment 1 
#

all: 
	mpicc -Wall ${PROG}.cpp -o ${PROG}

run: 
	mpiexec --hostfile hosts ${PROG}         

run-local: 
	mpiexec ${PROG}

clean:
	$(RM) ${PROG}
