# Question 3

## 1.

Yes (in a dangerously fragile manner).
An example of this can be see with two threads.

- Thread 1 runs the `lock` method. This in turn sets `turn` to its ID and sets `busy` to true. The lock method then returns and Thread 1 goes into the critical section.
- Thread 2 now attempts to go into the critical section. Thread 2 runs the `lock` method. `turn` is now set to Thread2's ID, and it loops as it fails the `busy=true` test.
- Thread 1 now completes the critical section, and thus runs the `unlock` method. This sets `busy` to false.
- Thread 2 now fails the `busy=true` test and thus exits the loop. It sets `busy=true` and fails the `while (turn != me)` test, thus exiting the lock method.
- Thread 2 now enters the critical section.

## 2.

Yes in the sense that no priority is given to any of the waiting threads. And thus the quickest thread to set `turn=me` when `busy = false` will get the lock (could this leave one thread waiting for longer than others? Yes, but that would be due to the thread being slow in the race.)
An example of this can be seen when looking at three threads (and ignores deadlocks).

- Thread 1 arrives to the critical section first, it runs the method `lock`, sets `busy` to true and returns from the method.
- Thread 2 arrives at the critical section and attempts to run the method `lock`. It sets `turn=me` and passes the `busy=false` check thus looping and waiting for the lock
- Thread 3 arrives at the critical section and it too attempts to run the method `lock`. It sets `turn=me` and passes the `busy=false` check thus looping and waiting for the lock.
  - Therefore Thread 2 and 3 are both waiting on the lock to release (i.e `busy=false`) and are equally setting `turn` to their IDs.
- Thread 1 completes the critical section and sets `busy=false`
- Thread 3 wins the race and fails the `busy=false` check before thread 2 can change the `turn` variable. Thus Thread 3 exits the loop and holds the lock.
- Thread 1 now attempts to enter the critical section once again and it too attempts to run the method `lock`. It sets `turn=me` and passes the `busy=false` check thus looping and waiting for the lock.
  - At this point we now have Thread 1 and Thread 2 waiting on the lock to release and are equally setting `turn` to their IDs.
- Thread 3 completes the critical section and releases the lock
- The quicker thread between 1 and 2 grabs the lock and the other lock waits.
- This continues as more threads attempt to enter the critical section.

## 3.

No. Due to the do while approached used, the `turn` variable can be changed between setting `busy=true` and the check `turn != me`.
This will occur when two threads try to execute the `lock` method simultaneously

- Thread 1 and Thread 2 attempt to enter the critical section.
- Thread 1 is slighlty quicker and sets `turn=me` and `busy=true`
- At this point Thread 2 enters the second do-while loop and sets `turn = me` .
  - Thread 2 will now constantly pass the `while (busy)` check (leaving it to loop).
  - While Thread 1 will constantly pass the `while ( turn !=me )` check (leaving it to loop).
- Thus both threads will never return from the `lock` as each depends on the other, thus leading to a deadlock
