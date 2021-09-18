#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include <math.h>

using namespace std;

int numThreads;
int *results;
long long maxNumber;
// double a = 0, b = 1, w, results;

// struct Config
// {
//   int startI;
//   int numTrapPerThread;
// } * cfg;

pthread_mutex_t results_mutex;

// double function(double x)
// {
//   return 4 / (1 + (x * x));
// }

// void *calculateFactorial(void *conf)
// {
//   Config *cfg = (Config *)conf;
//   double localResults;

//   for (int i = 0; i < cfg->numTrapPerThread; i++)
//   {
//     int pos = cfg->startI * cfg->numTrapPerThread + i;
//     if (pos == 0 || pos == maxNumber)
//     {
//       continue;
//     }
//     localResults = localResults + 2 * function(a + ((pos)*w));
//   }

//   pthread_mutex_lock(&results_mutex);
//   results += localResults;
//   pthread_mutex_unlock(&results_mutex);

//   pthread_exit(0);
// }

// int getNumTrapPerThread(int i, int numThreads, int maxNumber)
// {
//   // Add surplus to the last thread
//   if (i == numThreads - 1)
//   {
//     return (maxNumber / numThreads) + (maxNumber % numThreads);
//   }
//   return maxNumber / numThreads;
// }

int *markNonPrimes(int startNumber, int maxNumber)
{
  for (; startNumber <= maxNumber; startNumber++)
  {
    // TODO:add a read mutex here
    if (results[startNumber] == 1)
    {
      continue;
    }
    int multiple = 2;
    while (multiple * startNumber <= maxNumber)
    {
      // TODO:add a write mutex here
      results[multiple * startNumber] = 1;
      multiple++;
    }
  }
  return results;
}

int getAnswer(int numThreads, int maxNumber)
{
  results = (int *)malloc(sizeof(int) * maxNumber + 1);

  pthread_t threads[numThreads];
  pthread_mutex_init(&results_mutex, NULL);

  auto begin_time = chrono::high_resolution_clock::now();

  results[1] = 1;

  int init = int(floor(sqrt(maxNumber)));
  markNonPrimes(2, maxNumber);

  // for (int i = 0; i < numThreads; i++)
  // {
  //   cfg = (Config *)malloc(sizeof(struct Config));
  //   cfg->startI = i;
  //   cfg->numTrapPerThread = getNumTrapPerThread(i, numThreads, maxNumber);
  //   pthread_create(&threads[i], NULL, calculateFactorial, (void *)cfg);
  // }

  // for (int j = 0; j < numThreads; j++)
  // {
  //   pthread_join(threads[j], NULL);
  // }

  // double total = (((b - a) / maxNumber) / 2) * (function(a) + results + function(b));
  auto end_time = std::chrono::high_resolution_clock::now();

  cout << "results: ";
  for (int i = 1; i <= maxNumber; i++)
  {
    if (results[i] == 0)
    {
      cout << i << ", ";
    }
  }
  cout << "\n time taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms\n";
  free(results);
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
