#include <iostream>
#include <string>
#include <omp.h>
#include <sys/time.h>

static void show_usage(std::string name)
{
  std::cerr << "Usage: " << name << "\n"
       << "Options:\n"
       << "\t-h,--help\t\t\tShow this help message\n"
       << "\t-d,--dim\tREQUIRED\tSpecify dimensions of arrays\n"
       << "\t-t,--threads\tREQUIRED\tSpecify no of threads\n"
       << std::endl;
}

int main(int argc, char* argv[]) 
{
    if (argc != 3)
    {
        show_usage(argv[0]);
        return 1;
    }
	double time;			                        //variables for timing
	struct timeval ts, tf;

    int i, j, k;
    int dim = atoi(argv[1]);                        // Initialise dimensions
    int numThreads = atoi(argv[2]);

    double** a = new double*[dim];
    double** b = new double*[dim];
    double** c = new double*[dim];

    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp for private(i)
        for(i = 0; i < dim; i++) {
            a[i] = new double[dim];
            b[i] = new double[dim];
            c[i] = new double[dim];
        }
    }
    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp for private(i, j) collapse(2) schedule(static)
        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
            {
                a[i][j] = rand() % 10;
                b[i][j] = rand() % 10;;
                c[i][j] = 0;
            }
    }
    gettimeofday(&ts,NULL);

    #pragma omp parallel num_threads(numThreads) default(shared) private(i, j, k)
    {
        #pragma omp for collapse(3) schedule(static)
        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                for (k = 0; k < dim; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }

    gettimeofday(&tf,NULL);

	time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;
	printf("Matmul: Size %d Time %lf\n", dim, time);
    return 0;
}