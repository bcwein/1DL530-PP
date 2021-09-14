#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

double function(double x)
{
  return 4 / (1 + (x * x));
}

int calculateFactorial(int numThreads, int numTrapezes)
{
  double a = 0, b = 1;
  double w = (b - a) / numTrapezes;

  double results;

  for (int i = 1; i < numTrapezes; i++)
  {
    results = results + 2 * function(a + (i * w));
  }

  double total = (w / 2) * (function(a) + results + function(b));
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

  int numThreads, numTrapezes;

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
  return calculateFactorial(numThreads, numTrapezes);
}