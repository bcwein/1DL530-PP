#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <pthread.h>

using namespace std;

int numThreads, numTrapezes, numTrapPerThread;
double a = 0, b = 1, w, results;

pthread_mutex_t results_mutex;

double function(double x)
{
  return 4 / (1 + (x * x));
}

void *calculateFactorial(void *index)
{

  double localResults;
  long startI = (long)index;

  for (int i = 0; i < numTrapPerThread; i++)
  {
    int pos = startI * numTrapPerThread + i;
    if (pos == 0 || pos == numTrapezes)
    {
      continue;
    }
    localResults = localResults + 2 * function(a + ((pos)*w));
  }

  pthread_mutex_lock(&results_mutex);
  results += localResults;
  pthread_mutex_unlock(&results_mutex);

  pthread_exit(0);
}

int getAnswer(int numThreads, int numTrapezes)
{
  numTrapPerThread = numTrapezes / numThreads;
  w = (b - a) / numTrapezes;

  pthread_t threads[numThreads];
  pthread_mutex_init(&results_mutex, NULL);

  long i;
  for (i = 0; i < numThreads; i++)
  {
    pthread_create(&threads[i], NULL, calculateFactorial, (void *)i);
  }

  for (int j = 0; j < numThreads; j++)
  {
    pthread_join(threads[j], NULL);
  }

  double total = (((b - a) / numTrapezes) / 2) * (function(a) + results + function(b));
  cout << fixed << setprecision(15) << total;
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