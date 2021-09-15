# Q1 results

- Table can be found at table.png
- Results can be found at results.txt

# b)

As threads increase, time to calculate decreased for the same amount of trapezes.
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
