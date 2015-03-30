%Initializing Values
run('global_variables')
l_matrix = 50;
b_matrix = 50;
T_Div = 1200;
Sim_Matrix = zeros(l_matrix);
mig_index = zeros(l_matrix);
persctr = zeros(l_matrix);
divctr = zeros(l_matrix);
gridsvisited(1:b_matrix, 1:l_matrix) = 'N';
porous_matrix(1:b_matrix, 1:l_matrix) = 'N';
collide_persctr = 30;
collide_persctr_porous = 10;
stationary_persctr = 10;
KeySet = {'0','1','2','3','4','5','6','7'};
ValueSet = {stationary_persctr, 24, 24, 24, 24, 24, 24, collide_persctr};
checkpersctr = containers.Map(KeySet, ValueSet);

