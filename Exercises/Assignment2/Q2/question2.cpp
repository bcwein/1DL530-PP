#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include <math.h>

using namespace std;

int numThreads, numSeeds = 0;
int *results;
int *seeds;
long long sqrtNumber, maxNumber;

struct Config
{
  int threadID;
  int range;
} * cfg;

pthread_mutex_t results_mutex;

void markNumbers(int startNumber, int maxNumber)
{
  // cout << startNumber << " startNumber and maxnumber " << maxNumber << "\n";
  for (int i = startNumber; i <= maxNumber; i++)
  {
    if (results[i] == 1)
    {
      continue;
    }

    for (int j = 0; j < numSeeds; j++)
    {
      if (i % seeds[j] == 0)
      {
        // cout << "Marking " << i << " because " << seeds[j] << "\n";
        results[i] = 1;
        break;
      }
    }
  }
}

void *calculateResult(void *conf)
{
  Config *cfg = (Config *)conf;
  // cout << cfg->range << " ";

  int startPos = (sqrtNumber + 1) + (cfg->threadID * (maxNumber - sqrtNumber - 1)) / numThreads;
  int maxNumber = startPos + cfg->range;

  markNumbers(startPos, maxNumber);
  pthread_exit(0);
}

int getRangePerThread(int i, int numThreads, int maxNumber)
{
  // Add surplus to the last thread
  if (i == numThreads - 1)
  {
    return (maxNumber / numThreads) + (maxNumber % numThreads);
  }
  return maxNumber / numThreads;
}

void getSeeds(int startNumber, int maxNumber)
{
  int tempSeeds[maxNumber] = {};

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

int getAnswer(int numThreads, int maxNumber)
{
  results = (int *)malloc(sizeof(int) * maxNumber + 1);

  pthread_t threads[numThreads];
  pthread_mutex_init(&results_mutex, NULL);

  auto begin_time = chrono::high_resolution_clock::now();

  // mark initials
  results[1] = 1;
  sqrtNumber = (long long)(floor(sqrt(maxNumber)));
  getSeeds(2, sqrtNumber);

  for (int i = 0; i < numThreads; i++)
  {
    cfg = (Config *)malloc(sizeof(struct Config));
    cfg->threadID = i;
    cfg->range = getRangePerThread(i, numThreads, maxNumber - sqrtNumber - 1);

    pthread_create(&threads[i], NULL, calculateResult, (void *)cfg);
  }

  for (int i = 0; i < numThreads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  int countPrimes = 0;
  cout << "total number of primes < " << maxNumber << " : ";
  for (int i = 1; i <= maxNumber; i++)
  {
    if (results[i] == 0)
    {
      // uncomment to print the actual numbers
      // cout << i << ", ";
      countPrimes++;
    }
  }

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
        numThreads = stoll(argv[++i]);
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
        maxNumber = stoi(argv[++i]);
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
