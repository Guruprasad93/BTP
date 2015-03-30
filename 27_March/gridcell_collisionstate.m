%Collide --> with other cells

function [] = gridcell_collisionstate(k,j)
	run('global_variables');
	Sim_Matrix(k,j) = 1;
	mig_index(k,j) = 7;
	persctr(k,j) = collide_persctr;
end

