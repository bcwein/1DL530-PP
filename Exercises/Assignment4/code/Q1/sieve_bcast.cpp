#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <math.h>
#include "mpi.h"

using namespace std;

int myID, numProcs;

int numSeeds = 0;
int countPrimes = 0;

long long sqrtNumber;
long long maxNumber = 90000000;
int *results = (int *)malloc(sizeof(int) * maxNumber + 1);
int *seeds;

int *getSeeds(int startNumber, long maxNumber)
{
  int *tempSeeds = (int *)malloc(sizeof(int) * maxNumber);

  for (int i = startNumber; i <= maxNumber; i++)
  {
    if (results[i] == 1)
    {
      continue;
    }

    for (int j = 2; i * j <= maxNumber; j++)
    {
      results[j * i] = 1;
    }
    // cout << i << "\n";
    tempSeeds[numSeeds++] = i;
  }

  return tempSeeds;
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  auto begin_time = chrono::high_resolution_clock::now();
  if (myID == 0)
  {
    // mark initials
    results[1] = 1;
    sqrtNumber = (long long)(floor(sqrt(maxNumber)));
    seeds = getSeeds(2, sqrtNumber);
    MPI_Bcast(seeds, numSeeds, MPI_INT, myID, MPI_COMM_WORLD);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &myID);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  long long checkNumbers = maxNumber - sqrtNumber;

  int startNumber = (sqrtNumber + 1) + (myID * checkNumbers) / numProcs;
  int range = checkNumbers / numProcs;
  if (myID == numProcs - 1)
  {
    range = (checkNumbers / numProcs) + (checkNumbers % numProcs);
  }

  int localCount = 0;
  // cout << "My startnumber" << startNumber << " and range " << range << "\n";
  for (int i = startNumber; i <= startNumber + range && i < maxNumber; i++)
  {
    if (results[i] == 1)
    {
      continue;
    }

    bool isNotPrime = false;
    for (int j = 0; j < numSeeds; j++)
    {
      if (i % seeds[j] == 0)
      {
        isNotPrime = true;
        break;
      }
    }
    if (!isNotPrime)
    {
      // cout << i << "\n";
      localCount++;
    }
  }
  MPI_Reduce(&localCount, &countPrimes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Finalize();
  auto end_time = std::chrono::high_resolution_clock::now();

  if (myID == 0)
  {
    cout << "\ntotal number of primes < " << maxNumber << " : " << countPrimes + numSeeds;
    cout << "\ntime taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms";
    cout << "\nnumber of processes: " << numProcs << "\n";
  }
  return 0;
}
