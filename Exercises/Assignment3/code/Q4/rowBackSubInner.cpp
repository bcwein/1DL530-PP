#include <chrono>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

using namespace std;

long long numUnknowns;
int numThreads;

int getSolution()
{
  cout << setprecision(5) << fixed;

  double b[numUnknowns], x[numUnknowns];
  double *a[numUnknowns];
  for (long long i = 0; i < numUnknowns; i++)
    a[i] = (double *)malloc(numUnknowns * sizeof(double));

  if (numUnknowns == 3)
  {
    a[0][0] = 2;
    a[0][1] = -3;
    a[0][2] = 0;
    b[0] = 3;
    a[1][0] = 0;
    a[1][1] = 1;
    a[1][2] = 1;
    b[1] = 1;
    a[2][0] = 0;
    a[2][1] = 0;
    a[2][2] = -5;
    b[2] = 0;
  }
  else
  {
    srand(time(NULL));
#pragma omp parallel for collapse(2)
    for (int row = 0; row < numUnknowns; row++)
    {
      for (int col = 0; col < numUnknowns; col++)
      {
        a[row][col] = rand() % 10;
        if (col == numUnknowns - 1)
        {
          b[row] = rand() % 10;
        }
      }
    }
  }

  auto begin_time = chrono::high_resolution_clock::now();
  for (int row = numUnknowns - 1; row >= 0; row--)
  {
    x[row] = b[row];

#pragma omp parallel default(shared)
    {
#pragma omp master
      numThreads = omp_get_num_threads();
#pragma omp for
      for (int col = row + 1; col < numUnknowns; col++)
      {
        x[row] -= a[row][col] * x[col];
      }
    }

    x[row] /= a[row][row];
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  if (numUnknowns == 3)
  {
    if ((x[0] != 3) || (x[1] != 1) || (x[2] != 0))
    {
      cout << endl
           << "Solution: " << endl;
      for (int row = 0; row < numUnknowns; row++)
      {
        cout << "x[" << row << "] = " << x[row] << endl;
      }
    }
  }
  cout << "\nnumber of threads: " << numThreads;
  cout << "\ntime taken: " << chrono::duration<double, std::milli>(end_time - begin_time).count() << " ms\n";
  return (0);
}

static void show_usage(string name)
{
  cerr << "Usage: " << name << "\n"
       << "Options:\n"
       << "\t-h,--help\t\t\tShow this help message\n"
       << "\t-u,--number-of-unknowns\tREQUIRED\tSpecify number of unknowns"
       << endl;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
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
    else if ((arg == "-u") || (arg == "----number-of-unknowns"))
    {
      if (i + 1 < argc)
      {
        numUnknowns = stoll(argv[++i]);
      }
      else
      {
        cerr << "--number-of-unknowns option requires one argument." << endl;
        return 1;
      }
    }
  }
  return getSolution();
}
