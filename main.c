#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

typedef struct ThreadData {
    int insidePoints;
    int totalPoints;
    double radius;
} Spot;

void* monteCarlo(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    double radius = data->radius; 
    unsigned int seed = (unsigned int)time(NULL);

    for(int i = 0; i < data->totalPoints; ++i) {
        double x = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;
        double y = (((double)rand_r(&seed) / RAND_MAX) * 2 * radius) - radius;


        if(x * x + y * y <= radius * radius) {
            data->insidePoints++;
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s <radius> <number of threads>\n", argv[0]);
        return 1;
    }

    int Points = 1000;

    double radius = atof(argv[1]);
    int numThreads = atoi(argv[2]);

    pthread_t threads[numThreads];
    Spot alpha[numThreads];
    int numPointThread = Points / numThreads;

    struct timeval start, end;
    int seconds, microSeconds;

    gettimeofday(&start, NULL);
    for(int i = 0; i < numThreads; ++i) {
        alpha[i].insidePoints = 0;
        alpha[i].totalPoints = numPointThread;
        pthread_create(&threads[i],   NULL, monteCarlo, (void*)&alpha[i]);
    }

    for(int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    int pointInsideCircle = 0;
    for(int i = 0; i < numThreads; ++i) {
        pointInsideCircle += alpha[i].insidePoints;
    }
   
   //printf("%d   ,   %.6lf\n", pointInsideCircle, (double)pointInsideCircle / Points);
    double area = 4 * radius * radius * pointInsideCircle / Points ;

    printf("Area of the cirlce with radius %.3lf: %.3lf \n", radius, area);

    seconds = end.tv_sec - start.tv_sec;
    microSeconds = end.tv_usec - start.tv_usec;
    double time = seconds + microSeconds;
    printf("Execution time: %.3f ms\n", time);

    return 0;
}

