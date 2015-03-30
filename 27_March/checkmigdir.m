% Finding Migration direction given the new position

function [mig_dir] = checkmigdir(k,j,new_step)
	run('global_variables');
	
	if (j+1 == new_step(2))
		mig_dir = 1;
	end

	if (k+1 == new_step(1))
		mig_dir = 2;
	end

	if (j-1 == new_step(2))
		mig_dir = 3;
	end

	if (k-1 == new_step(1))
		mig_dir = 4;
	end

end
	

