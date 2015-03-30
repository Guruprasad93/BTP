%Collision --> with Pores

function [] = gridcell_collisionstate_porous(k,j)
	run('global_variables');

	mig_index(k,j) = 7;
	persctr(k,j) = collide_persctr_porous;
	Sim_Matrix(k,j) = 1;
end
