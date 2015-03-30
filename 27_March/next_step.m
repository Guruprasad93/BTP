%Finding Next step

function [newpos] = next_step(k,j,mig_dir)
	run('global_variables');

	if mig_dir == 1
		newpos(1) = k;
		newpos(2) = j+1;
	end

	if mig_dir == 2
		newpos(1) = k+1;
		newpos(2) = j;
	end

	if mig_dir == 3
		newpos(1) = k;
		newpos(2) = j-1;
	end

	if mig_dir == 4
		newpos(1) = k-1;
		newpos(2) = j;
	end

	c = (newpos(1)-1)*(newpos(1)-b_matrix);
	d = (newpos(2)-1)*(newpos(2)-l_matrix);
	if ((c<=0 && d<=0)&&porous_matrix(newpos(1),newpos(2)) == 'N')
		return
	else
		newpos = [-1,-1];
	end
end
	
	
