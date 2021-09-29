#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;

int numThreads, numSeeds = 0;
int *results;
int *seeds;
long long sqrtNumber, maxNumber;

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
    tempSeeds[numSeeds++] = i;
  }

  seeds = (int *)malloc(sizeof(int) * numSeeds);
  for (int k = 0; k < numSeeds; k++)
  {
    seeds[k] = tempSeeds[k];
  }
}

int getAnswer(int numThreads, long long maxNumber)
{
  results = (int *)malloc(sizeof(int) * maxNumber + 1);

  auto begin_time = chrono::high_resolution_clock::now();
#pragma omp parallel num_threads(numThreads)
  {
#pragma omp single
    {
      // mark initials
      results[1] = 1;
      sqrtNumber = (long long)(floor(sqrt(maxNumber)));
      getSeeds(2, sqrtNumber);
    }

#pragma omp for
    for (int i = sqrtNumber + 1; i <= maxNumber; i++)
    {
      if (results[i] == 1)
      {
        continue;
      }

      for (int j = 0; j < numSeeds; j++)
      {
        if (i % seeds[j] == 0)
        {
          results[i] = 1;
          break;
        }
      }
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  int countPrimes = 0;
  // uncomment to print the actual numbers
  // cout << "primes < " << maxNumber << " : ";
  for (int i = 1; i <= maxNumber; i++)
  {
    if (results[i] == 0)
    {
      // uncomment to print the actual numbers
      // cout << i << ", ";
      countPrimes++;
    }
  }
  cout << "\ntotal number of primes < " << maxNumber << " : ";
  cout << countPrimes;
  cout << "\ntime taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms\n";
  return 0;
}

static void show_usage(string name)
{
  cerr << "Usage: " << name << "\n"
       << "Options:\n"
       << "\t-h,--help\t\t\tShow this help message\n"
       << "\t-n,--threads\tREQUIRED\tSpecify number of threads\n"
       << "\t-m,--max-number\tREQUIRED\tSpecify max number"
       << endl;
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    show_usage(argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; ++i)
  {
    string arg = argv[i];
    if ((arg == "-h") || (arg == "--help"))
    {
      show_usage(argv[0]);
      return 0;
    }
    else if ((arg == "-n") || (arg == "--threads"))
    {
      if (i + 1 < argc)
      {
        numThreads = stoi(argv[++i]);
      }
      else
      {
        cerr << "--threads option requires one argument." << endl;
        return 1;
      }
    }
    else if ((arg == "-m") || (arg == "--max-number"))
    {
      if (i + 1 < argc)
      {
        maxNumber = stoll(argv[++i]);
      }
      else
      {
        cerr << "--max-number option requires one argument." << endl;
        return 1;
      }
    }
  }
  return getAnswer(numThreads, maxNumber);
}
