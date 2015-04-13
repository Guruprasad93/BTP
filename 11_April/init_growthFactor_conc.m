function [Conc_profile] = init_growthFactor_conc()

run('global_variables');

init_conc = 2;

Conc_profile = zeros(l_matrix+2);
Conc_profile((l_matrix+2)/2:(l_matrix+2)/2+1,(l_matrix+2)/2:(l_matrix+2)/2+1) = init_conc;
Conc_profile = reshape(Conc_profile, [], 1);
%Maintaining zero concentration at pores
pore_grids = reshape(porous_matrix,[],1);
v = (pore_grids=='Y');
Conc_profile(v) = 0;






