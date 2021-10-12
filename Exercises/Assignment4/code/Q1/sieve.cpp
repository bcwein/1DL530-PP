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
int *seeds;

long long sqrtNumber;
long long maxNumber = 90000000;
int *results = (int *)malloc(sizeof(int) * maxNumber + 1);

void getSeeds(int startNumber, long maxNumber)
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
    countPrimes++;
    // cout << i << "\n";
    tempSeeds[numSeeds++] = i;
  }

  seeds = (int *)malloc(sizeof(int) * numSeeds);
  for (int k = 0; k < numSeeds; k++)
  {
    seeds[k] = tempSeeds[k];
  }
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
    getSeeds(2, sqrtNumber);
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
  if (myID == 0)
  {
    int count = 0;
    int msg = 0;
    countPrimes += localCount;
    while (count < numProcs - 1)
    {
      MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      countPrimes += msg;
      count++;
    }
  }
  else
  {
    MPI_Send(&localCount, 1, MPI_INT, 0, myID, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  auto end_time = std::chrono::high_resolution_clock::now();

  if (myID == 0)
  {
    cout << "\ntotal number of primes < " << maxNumber << " : " << countPrimes;
    cout << "\ntime taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms";
    cout << "\nnumber of processes: " << numProcs << "\n";
  }
  return 0;
}
