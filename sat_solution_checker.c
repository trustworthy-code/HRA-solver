int sat_solution_checker(int variable_count,
                         int disjunction_count,
                         int* twocnf_formula,
                         int* variable_values)
{
    int result = 1;
    for (int i = 0; (i < disjunction_count) && (result == 1);
         i++)
    {
        int index_i = 2*i;
        int first_literal = twocnf_formula[index_i];
        int second_literal = twocnf_formula[index_i + 1];
        
        if ((first_literal > 0) && (second_literal > 0))
        {
            int first_variable_index = first_literal-1;
            int second_variable_index = second_literal-1;
            
            if ((variable_values[first_variable_index] < 0)
             && (variable_values[second_variable_index] < 0))
            {
                result = 0;
            }
        }
        else if ((first_literal > 0) && (second_literal < 0))
        {
            int first_variable_index = first_literal-1;
            int second_variable_index = -second_literal-1;
            
            if ((variable_values[first_variable_index] < 0)
             && (variable_values[second_variable_index] > 0))
            {
                result = 0;
            }
        }
        else if ((first_literal < 0) && (second_literal > 0))
        {
            int first_variable_index = -first_literal-1;
            int second_variable_index = second_literal-1;
            
            if ((variable_values[first_variable_index] > 0)
             && (variable_values[second_variable_index] < 0))
            {
                result = 0;
            }
        }
        else
        {
            int first_variable_index = -first_literal-1;
            int second_variable_index = -second_literal-1;
            
            if ((variable_values[first_variable_index] > 0)
             && (variable_values[second_variable_index] > 0))
            {
                result = 0;
            }
        }
    }
    return result;
}