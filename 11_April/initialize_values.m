%Initializing Values
run('global_variables')
l_matrix = 50;
b_matrix = 50;
T_Div = 1200;
starve_time = 180; %Survives for 3hrs without nutrients!
%T_Div = 10;
Sim_Matrix = zeros(l_matrix);
mig_index = zeros(l_matrix);
persctr = zeros(l_matrix);
divctr = zeros(l_matrix);
glc_ctr = zeros(l_matrix);
gridsvisited(1:b_matrix, 1:l_matrix) = 'N';
porous_matrix(1:b_matrix, 1:l_matrix) = 'N';
collide_persctr = 30;
%collide_persctr = 5;
collide_persctr_porous = 10;
%collide_persctr_porous = 3;
stationary_persctr = 10;
%stationary_persctr = 3;
KeySet = {'0','1','2','3','4','5','6','7'};
ValueSet = {stationary_persctr, 24, 24, 24, 24, 24, 24, collide_persctr};
%ValueSet = {stationary_persctr, 3, 3, 3, 3, 3, 3, collide_persctr};
checkpersctr = containers.Map(KeySet, ValueSet);

