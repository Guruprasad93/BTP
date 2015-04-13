%Checking whether persistence counter is 0

function [result] = checkpersistencectr(k,j)

    run('global_variables');
    if persctr(k,j) == 0
	result = 0;
    else
	result = 1;
    end
end

