%Sending Cell to stationary Phase due to overcrowding

function [] = gridcell_stationaryphase(k,j)
	run('global_variables');
	mig_index(k,j) = 0;
	persctr(k,j) = stationary_persctr;
end
