# Question 2

For this exercise, we were tasked to use Posix threads to implement a parallel version of the Sieve of Eratosthenes (via the algorithm given in the original task.). The coded solution can be found in [.question2.cpp].

This implementation attempted to stay as true to the original algorithm as possible and thus first computes the primes from 2 to sqrt(max) in a serial fashion.
Once complete these "seed" primes are stored in an array.
The remaining numbers (that being from sqrt(max) to max) are then split amongst the number of threads. This is done by dividing the amount of remaining numbers by the number of threads and flooring the result. If the aforementioned remaining list is divisible by numberOfThreads without remainder, work is distributed equally (and therefore fully _balanced_). If however the result is not fully divisible, the final thread picks up the extra cycles (thus leaving it in an almost equal split for all but one of the threads) and completes the calculation.
Mathematically given the fact the partition size is constant (for almost all cases) and the seed values are constant, the work for each thread is almost equal (as the checks would equate to `for each number in range, check if number is divisible by any number in seeds`). Therefore the max-checks for each thread is `rangeLength * numberOfSeeds`.
A potential inequality argument may be made that the actual checks done may be slightly quicker in one thread vs another (as one thread may hold a range with many numbers divisible by 2, which would result in far quicker tests). This argument is valid, however testing has not shown this to be impactful in any meaningful way.

Furthermore no synchronization between threads were utilized in this solution. The simple reasoning behind this is that no thread operates on the same block of memory that another thread operates on. As the results array is a contiguous block of memory, with each element getting a distinct memory address, it therefore follows that each element can be interacted with independently of each other.
Since the algorithm partitions the remaining numbers into distinct chunks with no overlap, no thread contains or interacts with numbers out of its chunk, which therefore follows that no thread interacts with elements being touched by another thread (in terms of the `results` array.)
One _could_ add a mutex lock on writes to results, however the overhead of the lock checks would provide no real benefit as the algorithm ensures that each block works independently.
The only piece of common-shared memory between the threads is the `seeds` array. This common interaction is only done in a **read-only** fashion. And since the seeds array is computed once in a serial fashion _before_ the threads spin-up, we know that its value is written once and never altered after. Therefore there is no issue with multiple threads reading its value at the same time. For safety's sake we could add a read-write lock at this point, however the overhead of the lock will slow down the program for a case that is impossible given the design of the algorithm.

As mentioned above, there is no real communication between sub-threads of the main thread (as they operate independently), with the only real communication occuring between the main thread and individual sub-threads, where sub-threads would update distinct shared-memory locations and for the main-thread to wait for sub-threads to complete and join to the main thread.

## Results

- Graphs:

  - maxNumLow.png
  - maxNumHigh.png
  - maxNumVHigh.png

- Results can be found at results.txt

As seen in the graphs, at lower maxNums (that being 10 or 100), we see that the time actually increase as we add more threads. This seems to be due to the system overhead of managing threads (creation, tracking and freeing) and the design of the algorithm (which for smaller numbers creates many more partitions that is actually required, thus threads are barely doing any work).
However as we increase maxNum into the millions (in our testing 1000000 and 10000000), we see that the time taken to compute the primes scales linearly as we increase the amount of threads (that is 2 threads takes about 50% of the time that 1 thread takes, with 4 threads taking 25% of the time).This result is further demonstrated when looking at the computational results when maxNumber is 90000000.
In conclusion, this exercise demonstrates the impact that an algorithm and chosen parameters ultimately decides when parallelism can make a difference (and by how much) to your application.
