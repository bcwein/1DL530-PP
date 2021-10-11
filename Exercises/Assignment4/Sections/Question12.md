# Question 1.2 - Sieve of Eratosthenes - MPI (Multi-Machine)

In this question we were asked to rewrite our earlier implementation of the Sieve of Eratosthenes using MPI, while utilizing `MPI_send`
and `MPI_Recv`. Once complete we were then asked to run the code on three server and note the speed-up/slow-down when compared to previous implementations.

## Results

As seen in {mpi_MULTI}, running the MPI implementation on multi-machines far outstripped our initial PThread version and OpenMP implementations.
This makes sense as we are now exploiting 3x the amount of cores we previously had.
Interestingly even though we now utilized 3x the amount of cores, we only saw a 70% improvement in terms of speed.

This can be attributed to a number of factors:

- Network Traffic and Latency
- Overhead the MPI adds when running on multiple-machines
- Time spent sending and receiving from the different machines
- The code which is still being run serially in the application
