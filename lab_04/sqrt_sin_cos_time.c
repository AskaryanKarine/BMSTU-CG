#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// вычисление времени
unsigned long int tick(void)
{
    unsigned long int d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );
    return d;
}

int main(void)
{
    unsigned long int begin, end;
    double elem;
    unsigned long int time_spent, sum_sqrt = 0, sum_sin = 0, sum_cos = 0;
    int iteration = 1000;

    for (int i = 0; i < iteration; i++)
    {
        begin = tick();
        elem = sqrt(i);
        end = tick();
        time_spent = (end - begin);
        sum_sqrt += time_spent;
    }
    sum_sqrt /= iteration;

    for (int i = 0; i < iteration; i++)
    {
        begin = tick();
        elem = sin(i);
        end = tick();
        time_spent = (end - begin);
        sum_sin += time_spent;
    }
    sum_sin /= iteration;

    for (int i = 0; i < iteration; i++)
    {
        begin = tick();
        elem = cos(i);
        end = tick();
        time_spent = (end - begin);
        sum_cos += time_spent;
    }
    sum_cos /= iteration;


    printf("Время вычисления корня в тактах процессора: %lu\n", sum_sqrt);
    printf("Время вычисления синуса в тактах процессора: %lu\n", sum_sin);
    printf("Время вычисления косинуса в тактах процессора: %lu\n", sum_cos);

    return 0;
}