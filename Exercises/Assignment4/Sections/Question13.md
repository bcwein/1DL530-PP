# Question 1.3 - Sieve of Eratosthenes - MPI (Multi-Machine using MPI_Reduce)

In this question we were asked to rewrite our earlier implementation of the Sieve of Eratosthenes using MPI, while utilizing `MPI_Bcast`
and `MPI_Reduce`. Once complete we were then asked to run the code on three server and note the speed-up/slow-down when compared to previous implementations.

## Process

- We firstly removed any mentions of `MPI_Send` and `MPI_Recv` from our implementation
- We then refactored the code to use `MPI_Bcast` and `MPI_Reduce`

## Difficulty

When comparing to the our initial MPI implementation, using Broadcast and Reduce was far easier to write.
However using OpenMP still far exceeds MPI in ease-of-use, readability and code-length.

## Results

As seen in {mpi_BCAST}, running the MPI implementation with `Broadcast` and `Reduce`, we see no noticeable slow-down or speed-up between the two implementations.
This is most likely as our application does very little at send/receive or reduce time, therefore there would be no noticeable speed-up between the two algorithms.
Therefore due to the slightly cleaner code and easier implementation, we would suggest using `MPI_Bcast` and `MPI_Reduce` to replace `MPI_Send` and `MPI_Recv` where possible.
