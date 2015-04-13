%Cell Migration

function [] = cell_migrate(k,j)
	run('global_variables');

	mig_dir = mig_index(k,j);
	if (mig_dir == 0 || mig_dir == 7)
		return 
	end

	newpos = next_step(k,j,mig_dir);

	if (newpos(1) == -1)
		gridcell_collisionstate_porous(k,j);
		return
	end

	if (Sim_Matrix(newpos(1),newpos(2)) == 0)
		ctr_pers = persctr(k,j);
		tdiv = divctr(k,j);
		food_ctr = glc_ctr(k,j);
        Sim_Matrix(newpos(1),newpos(2)) = 1;
		mig_index(newpos(1),newpos(2)) = mig_dir;
		persctr(newpos(1),newpos(2)) = ctr_pers;
		divctr(newpos(1),newpos(2)) = tdiv;
        glc_ctr(newpos(1),newpos(2)) = food_ctr;
        
		Sim_Matrix(k,j) = 0;
		mig_index(k,j) = 0;
		persctr(k,j) = 0;
		divctr(k,j) = 0;
        glc_ctr(k,j) = 0;

	else

		%Go to collision state
		gridcell_collisionstate(k,j);
		gridcell_collisionstate(newpos(1),newpos(2));
		divctr(newpos(1),newpos(2)) = divctr(newpos(1),newpos(2)) - 1;
	end
	
	gridsvisited(newpos(1),newpos(2)) = 'Y';

end

