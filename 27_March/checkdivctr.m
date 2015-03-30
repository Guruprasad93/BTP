%Checking if Division counter is 0

function [result] = checkdivctr(k,j)

    run('global_variables');
    if divctr(k,j) == 0
	result = 0;
    else
	result = 1;
    end
end
