#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <pthread.h>

using namespace std;

int numThreads, numTrapezes;
double a = 0, b = 1, results;

double function(double x)
{
  return 4 / (1 + (x * x));
}

void *calculateFactorial(void *n)
{
  double localResults;
  int *numTrapPerThread = (int *)n;
  double w = (b - a) / numTrapezes;
  for (int i = 0; i < *numTrapPerThread; i++)
  {
    localResults = localResults + 2 * function(a + (i * w));
  }
  results += localResults;
  pthread_exit(0);
}

int getAnswer(int numThreads, int numTrapezes)
{
  int numTrapPerThread = numTrapezes / numThreads;
  int finalAmountOfTrap = numTrapPerThread + numTrapezes % numThreads;

  pthread_t threads[numThreads];
  for (int i = 0; i < numThreads - 1; i++)
  {
    pthread_create(&threads[i], NULL, calculateFactorial, (void *)&numTrapPerThread);
  }
  // Spin-up the last thread which takes on the same amount of jobs + remainder
  pthread_create(&threads[numThreads - 1], NULL, calculateFactorial, (void *)&finalAmountOfTrap);

  for (int j = 0; j < numThreads; j++)
  {
    pthread_join(threads[j], NULL);
  }

  double total = (((b - a) / numTrapezes) / 2) * (function(a) + results + function(b));
  cout << fixed << setprecision(20) << total;
  return 0;
}

static void show_usage(string name)
{
  cerr << "Usage: " << name << "\n"
       << "Options:\n"
       << "\t-h,--help\t\t\tShow this help message\n"
       << "\t-n,--threads\tREQUIRED\tSpecify number of threads\n"
       << "\t-t,--trapezes\tREQUIRED\tSpecify number of trapezes"
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
    else if ((arg == "-t") || (arg == "--trapezes"))
    {
      if (i + 1 < argc)
      {
        numTrapezes = stoi(argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
      }
      else
      {
        cerr << "--trapezes option requires one argument." << endl;
        return 1;
      }
    }
  }
  return getAnswer(numThreads, numTrapezes);
}