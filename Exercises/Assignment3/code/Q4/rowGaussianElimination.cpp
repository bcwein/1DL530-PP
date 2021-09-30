#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

using namespace std;

long long numUnknowns;

int getSolution()
{
  cout << setprecision(3) << fixed;
  float a[numUnknowns][numUnknowns], b[numUnknowns], x[numUnknowns], ratio;

  if (numUnknowns <= 3)
  {
    cout << "Enter Coefficients of Augmented Matrix: " << endl;
    for (int row = 0; row < numUnknowns; row++)
    {
      for (int col = 0; col < numUnknowns; col++)
      {
        cout << "a[" << row << "]" << col << "]= ";
        cin >> a[row][col];
      }
    }
  }

  for (int row = numUnknowns - 1; row >= 0; row--)
  {
    x[row] = b[row];
    for (int col = row + 1; col < numUnknowns; col++)
      x[row] -= a[row][col] * x[col];
    x[row] /= a[row][row];
  }

  /* Obtaining Solution by Back Substitution Method */
  x[n] = a[n][n + 1] / a[n][n];

  for (i = n - 1; i >= 1; i--)
  {
    x[i] = a[i][n + 1];
    for (j = i + 1; j <= n; j++)
    {
      x[i] = x[i] - a[i][j] * x[j];
    }
    x[i] = x[i] / a[i][i];
  }

  /* Displaying Solution */
  cout << endl
       << "Solution: " << endl;
  for (i = 1; i <= n; i++)
  {
    cout << "x[" << i << "] = " << x[i] << endl;
  }

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
