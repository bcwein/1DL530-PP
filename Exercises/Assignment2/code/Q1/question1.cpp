#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include <mutex>
using namespace std;

int numThreads, numTrapezes;
double a = 0, b = 1, w, results;

struct Config
{
  int startI;
  int numTrapPerThread;
} * cfg;

std::mutex results_mutex;

double function(double x)
{
  return 4 / (1 + (x * x));
}

void *calculateFactorial(void *conf)
{
  Config *cfg = (Config *)conf;
  double localResults;

  for (int i = 0; i < cfg->numTrapPerThread; i++)
  {
    int pos = cfg->startI * cfg->numTrapPerThread + i;
    if (pos == 0 || pos == numTrapezes)
    {
      continue;
    }
    localResults = localResults + 2 * function(a + ((pos)*w));
  }

  results_mutex.lock();
  results += localResults;
  results_mutex.unlock();

  pthread_exit(0);
}

int getNumTrapPerThread(int i, int numThreads, int numTrapezes)
{
  // Add surplus trapezes to the last thread
  if (i == numThreads - 1)
  {
    return (numTrapezes / numThreads) + (numTrapezes % numThreads);
  }
  return numTrapezes / numThreads;
}

int getAnswer(int numThreads, int numTrapezes)
{
  w = (b - a) / numTrapezes;

  pthread_t threads[numThreads];

  auto begin_time = chrono::high_resolution_clock::now();
  for (int i = 0; i < numThreads; i++)
  {
    cfg = (Config *)malloc(sizeof(struct Config));
    cfg->startI = i;
    cfg->numTrapPerThread = getNumTrapPerThread(i, numThreads, numTrapezes);
    pthread_create(&threads[i], NULL, calculateFactorial, (void *)cfg);
  }

  for (int j = 0; j < numThreads; j++)
  {
    pthread_join(threads[j], NULL);
  }

  double total = (((b - a) / numTrapezes) / 2) * (function(a) + results + function(b));
  auto end_time = std::chrono::high_resolution_clock::now();

  cout << "result: " << fixed << setprecision(25) << total << "\n";
  cout << "time taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms\n";
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