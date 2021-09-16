# Q1 results

- Graphs:
  - lowTrap.png
  - medTrap.png
  - highTrap.png
- Output table - table.png
- Results can be found at results.txt

# b)

As threads increased for low amount of trapezes, the time to compute actually increases. This is due to the overhead that parallelisation adds to the application (thread-creation, waiting on threads to complete and lock-waits). 
However as we reach medium amount of trapezes (1000000) or high (1000000000) we see that the time decreases significantly as at this point the cost to parallelize is less than the benefit it offers.
While the number of trapezes increase, so too did the accuracy of the result.

# c)

This solution takes a very naive approach to parallelizing the program. It attempts to split the number of trapezes by the number of threads
(with the surplus going to the last thread). This means that:

- the work load is not evenly split (in terms of the data partitioning)
- the time taken for each thread to complete varies based on the effort required to complete the calculation for a given trapeze.

An improvement to speed can occur if a thread queueing system was implemented instead, where each thread worked on a single trapeze (until max threads).
Thereafter as a thread completed, a new thread would spin up and compute the next trapeze.
This would ensure that no thread would be wasted, as until the sum was complete, all threads would be working (in a more equal and more efficient manner).
However the queuing solution could potentially have a mutex related bottle neck as result would need to be written after computing a single trapeze area (vs in our solution, it is only written at the end of a set of trapezes).
