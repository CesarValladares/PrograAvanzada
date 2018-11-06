/*
    Simple program to compute the integral of an equation
    This version is a linear program, without any parallelisation

    The method followed consists on evaluating the equation at several points
    The value of f(x) obtained is multiplied by the size of the iteration step,
    to compute the area of a rectangle. Then add all the rectangles for the final area.

    The method is illustrated at:
        http://hyperphysics.phy-astr.gsu.edu/hbasees/integ.html#c4

    Gilberto Echeverria
    28/10/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 8

typedef struct data_struct {
    double min_x;
    double max_x;
    double steps;
    double * total_area;
    pthread_mutex_t * area_mutex;
} data_t;

// Function definitions
void usage(const char * program);
double equation(double x);
void * getIntegral(void * arg);
double startThreads(double min_x, double max_x, int steps);

int main(int argc, char * argv[])
{
    double total_area = 0.0;
    double min_x;
    double max_x;
    int steps;

    // Get the limit as an argument
    if (argc == 4)
    {
        min_x = atof(argv[1]);
        max_x = atof(argv[2]);
        steps = atoi(argv[3]);
    }
    else
    {
        usage(argv[0]);
    }

    printf("Computing the area under the curve:\n");

    total_area = startThreads(min_x, max_x, steps);

    printf("The total area is: %.10lf\n", total_area);

    return 0;
}

double startThreads(double min_x, double max_x, int steps)
{
    double total_area = 0.0;
    pthread_mutex_t area_mutex = PTHREAD_MUTEX_INITIALIZER;
    data_t * thread_data = NULL;
    pthread_t tid[NUM_THREADS];
    int status;
    
    int thread_steps = steps / NUM_THREADS;
    
    for (int i=0; i<NUM_THREADS; i++)
    {
        thread_data = malloc(sizeof (data_t));
        
        thread_data->min_x = thread_steps * i;
        thread_data->max_x = thread_steps * (i + 1);
        thread_data->steps = thread_steps;
        thread_data->total_area = &total_area;
        thread_data->area_mutex = &area_mutex;
        
        status = pthread_create(&tid[i], NULL, getIntegral, (void *)thread_data);
        if (status == -1)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL); 
    }
    
    return total_area;
}

void usage(const char * program)
{
    printf("Usage:\n");
    printf("\t%s {min_x_value} {max_x_value} {number_of_area_steps}\n", program);
    exit(EXIT_FAILURE);
}

double equation(double x)
{
    //return 7 * x - 8.5 * pow(x, 2) + 3 * pow(x, 3);
    //return sin(x) + pow(cos(x/7), 2) + 3;
    return sin(x) + pow(cos(x/7), 2) + 0.1 * x + sqrt(fabs(tan(x))) - log(pow(x, 2)) + 5;
}

void * getIntegral(void * arg)
{
    data_t * thread_data = (data_t *) arg;
    double step_size = (thread_data->max_x - thread_data->min_x) / thread_data->steps;
    double y;
    double area;
    double local_total = 0.0;

    for (double x = thread_data->min_x + step_size / 2; x < thread_data->max_x; x += step_size) 
    {
        y = equation(x);
        area = step_size * y;
        //printf("Area at x: %lf -> %lf * %lf =  %lf\n", x, y, step_size, area);
        local_total += area;
    }
    
    pthread_mutex_lock(thread_data->area_mutex);
    *(thread_data->total_area) += local_total;
    pthread_mutex_unlock(thread_data->area_mutex);
        
    free(thread_data);

    pthread_exit(NULL);
}
    