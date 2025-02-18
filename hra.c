#include <stdlib.h>
#include <stdio.h>
#include "hra_solver.h"

void hra(int argc, char* argv[])
{
    FILE *fin;
    int is_input_file = 0;
    if (argc > 1)
    {
        fin = fopen(argv[1], "r");
        is_input_file = 1;
    }

    int task_count = 0;
    int task_dependency_count = 0;
    if (is_input_file == 1)
    {
        fscanf(fin, " %d %d", &task_count, &task_dependency_count);
    }
    else
    {
        scanf(" %d %d", &task_count, &task_dependency_count);
    }

    int* task_dependencies =(int*)malloc(4 * task_count * sizeof(int));

    for (int i = 0; i < task_dependency_count; i++)
    {
        int predecessor;
        int successor;
        int first_cost;
        int second_cost;

        if (is_input_file == 1)
        {
            fscanf(fin, " %d %d %d %d", &predecessor, &successor, &first_cost, &second_cost);
        }
        else
        {
            scanf(" %d %d %d %d", &predecessor, &successor, &first_cost, &second_cost);
        }

        int index = 4 * i;
        task_dependencies[index] = predecessor;
        task_dependencies[index + 1] = successor;
        task_dependencies[index + 2] = first_cost;
        task_dependencies[index + 3] = second_cost;
    }

    int* task_costs =(int*)malloc(2 * task_count * sizeof(int));

    for (int i = 0; i < task_count; i++)
    {
        int first_cost;
        int second_cost;

        if (is_input_file == 1)
        {
            fscanf(fin, " %d %d", &first_cost, &second_cost);
        }
        else
        {
            scanf(" %d %d", &first_cost, &second_cost);
        }

        int index = 2 * i;
        task_costs[index] = first_cost;
        task_costs[index + 1] = second_cost;
    }

    if (is_input_file == 1)
    {
        fclose(fin);
    }

    int* schedule = (int*)malloc(task_count * sizeof(int));
    
    hra_solver(task_count, task_dependency_count, task_costs, task_dependencies, schedule);

    free(task_costs);

    free(task_dependencies);

    FILE *fout;
    int is_output_file = 0;
    if (argc > 2)
    {
        fout = fopen(argv[2], "w");
        is_output_file = 1;
    }

    if (is_output_file == 1)
    {
        for (int i = 0; i < task_count; i++)
        {
            fprintf(fout, "%d %d\n", i+1, schedule[i]+1);
        }
    }
    else
    {
        for (int i = 0; i < task_count; i++)
        {
            printf("\n%d %d\n", i+1, schedule[i]+1);
        }
    }

    free(schedule);

    if (is_output_file == 1)
    {
        fclose(fout);
    }
}