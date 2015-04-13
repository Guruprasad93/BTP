%Checking if cell is present

function [result] = checkcell(k,j)

    run('global_variables');
    
    if Sim_Matrix(k,j) == 1
        result = 1;
    else
    	result = 0;
    end
end

