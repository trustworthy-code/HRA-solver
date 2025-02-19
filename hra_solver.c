#include <stdlib.h>
#include "two_satisfiability_solver.h"

void hra_solver(int task_count, int task_dependency_count, int* task_costs, int* task_dependencies,
                int* schedule)
{
    int* combination_list = (int*)malloc(20*task_dependency_count*sizeof(int));

    for (int i = 0; i < task_dependency_count; i++)
    {
        int combination_list_index = 20 * i;
        int task_dependency_index = 4 * i;
        combination_list[combination_list_index] =
            task_costs[2 * (task_dependencies[task_dependency_index] - 1)] +
            task_costs[2 * (task_dependencies[task_dependency_index + 1] - 1)];
        combination_list[combination_list_index + 1] = task_dependencies[task_dependency_index];
        combination_list[combination_list_index + 2] = task_dependencies[task_dependency_index + 1];
        combination_list[combination_list_index + 3] = 0;
        combination_list[combination_list_index + 4] = 0;
        combination_list[combination_list_index + 5] =
            task_costs[2 * (task_dependencies[task_dependency_index] - 1) + 1] +
            task_costs[2 * (task_dependencies[task_dependency_index + 1] - 1) + 1];
        combination_list[combination_list_index + 6] = task_dependencies[task_dependency_index];
        combination_list[combination_list_index + 7] = task_dependencies[task_dependency_index + 1];
        combination_list[combination_list_index + 8] = 1;
        combination_list[combination_list_index + 9] = 1;
        combination_list[combination_list_index + 10] =
            task_costs[2 * (task_dependencies[task_dependency_index] - 1)] +
            task_dependencies[task_dependency_index + 2] +
            task_costs[2 * (task_dependencies[task_dependency_index + 1] - 1) + 1];
        combination_list[combination_list_index + 11] = task_dependencies[task_dependency_index];
        combination_list[combination_list_index + 12] = task_dependencies[task_dependency_index + 1];
        combination_list[combination_list_index + 13] = 0;
        combination_list[combination_list_index + 14] = 1;
        combination_list[combination_list_index + 15] =
            task_costs[2 * (task_dependencies[task_dependency_index] - 1) + 1] +
            task_dependencies[task_dependency_index + 3] +
            task_costs[2 * (task_dependencies[task_dependency_index + 1] - 1)];
        combination_list[combination_list_index + 16] = task_dependencies[task_dependency_index];
        combination_list[combination_list_index + 17] = task_dependencies[task_dependency_index + 1];
        combination_list[combination_list_index + 18] = 1;
        combination_list[combination_list_index + 19] = 0;
    }

    int is_sorted = 0;

    for (int j = 1; (j < 4*task_dependency_count) && (is_sorted == 0); j++)
    {
        is_sorted = 1;
        int index_j = 5 * j;

        for (int i = 0; i < 4*task_dependency_count-j; i++)
        {
            int index_i = 5 * i;

            if (combination_list[index_i] < combination_list[index_i + 5])
            {
                int tmp_1 = combination_list[index_i];
                int tmp_2 = combination_list[index_i + 1];
                int tmp_3 = combination_list[index_i + 2];
                int tmp_4 = combination_list[index_i + 3];
                int tmp_5 = combination_list[index_i + 4];

                combination_list[index_i] = combination_list[index_i + 5];
                combination_list[index_i + 1] = combination_list[index_i + 6];
                combination_list[index_i + 2] = combination_list[index_i + 7];
                combination_list[index_i + 3] = combination_list[index_i + 8];
                combination_list[index_i + 4] = combination_list[index_i + 9];

                combination_list[index_i + 5] = tmp_1;
                combination_list[index_i + 6] = tmp_2;
                combination_list[index_i + 7] = tmp_3;
                combination_list[index_i + 8] = tmp_4;
                combination_list[index_i + 9] = tmp_5;

                is_sorted = 0;
            }
        } 
    }

    int* formula;
    int formula_variable_count = 0;
    int formula_disjunction_count = 0;

    int* formula_tmp;
    int formula_tmp_variable_count = 0;
    int formula_tmp_disjunction_count = 0;

    int satisfiable = 1;

    for (int i = 0; (i <= 4*task_dependency_count) && (satisfiable == 1); i++)
    {
        formula_tmp = (int*)malloc(2 * (formula_tmp_disjunction_count + 1) * sizeof(int));
        for (int j = 0; j < formula_tmp_disjunction_count; j++)
        {
            int index_j = 2 * j;
            formula_tmp[index_j] = formula[index_j];
            formula_tmp[index_j + 1] = formula[index_j + 1];
        }

        int index_i = 5 * i;
        int formula_tmp_first_literal;
        int formula_tmp_second_literal;
        int first_task = combination_list[index_i + 1];
        int second_task = combination_list[index_i + 2];
        int first_task_assignment = combination_list[index_i + 3];
        int second_task_assignment = combination_list[index_i + 4];

        if ((first_task_assignment == 0) && (second_task_assignment == 0))
        {
            formula_tmp_first_literal = first_task;
            formula_tmp_second_literal = second_task;
        }
        else if ((first_task_assignment == 1) && (second_task_assignment == 1))
        {
            formula_tmp_first_literal = -first_task;
            formula_tmp_second_literal = -second_task;
        }
        else if ((first_task_assignment == 0) && (second_task_assignment == 1))
        {
            formula_tmp_first_literal = first_task;
            formula_tmp_second_literal = -second_task;
        }
        else if ((first_task_assignment == 1) && (second_task_assignment == 0))
        {
            formula_tmp_first_literal = -first_task;
            formula_tmp_second_literal = second_task;
        }

        formula_tmp[2 * formula_tmp_disjunction_count] = formula_tmp_first_literal;
        formula_tmp[2 * formula_tmp_disjunction_count + 1] = formula_tmp_second_literal;
        formula_tmp_disjunction_count++;
        if (first_task > formula_tmp_variable_count)
        {
            formula_tmp_variable_count = first_task;
        }
        if (second_task > formula_tmp_variable_count)
        {
            formula_tmp_variable_count = second_task;
        }

        int* model = (int*)malloc(formula_tmp_variable_count * sizeof(int));
        satisfiable = two_satisfiability_solver(formula_tmp_variable_count,
                                                formula_tmp_disjunction_count, formula_tmp, model);
        if (satisfiable == 1)
        {
            if (formula_tmp_disjunction_count > 1)
            {
                free(formula);
            }

            formula = (int*)malloc(2 * formula_tmp_disjunction_count * sizeof(int));
            for (int j = 0; j < formula_tmp_disjunction_count; j++)
            {
                int index_j = 2 * j;
                formula[index_j] = formula_tmp[index_j];
                formula[index_j + 1] = formula_tmp[index_j + 1];
            }
            formula_variable_count = formula_tmp_variable_count;
            formula_disjunction_count = formula_tmp_disjunction_count;
        }

        free(formula_tmp);
        free(model);
    }

    free(combination_list);

    int* model = (int*)malloc(task_count * sizeof(int));

    int result = two_satisfiability_solver(task_count,
                                           formula_disjunction_count, formula, model);

    for (int i = 0; i < task_count; i++)
    {
         if (model[i] > 0)
         {
            schedule[model[i]-1] = 1;
         }
         else if (model[i] < 0)
         {
            schedule[-model[i]-1] = 0;
         }
    }

    free(model);
    free(formula);
}