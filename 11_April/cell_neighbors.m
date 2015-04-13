function [neighbor_matrix, empty_num] = cell_neighbors(k,j)

run('global_variables');

index = 1;

if (k-1>=1 && k-1<=b_matrix)
    neighbor_matrix(index,:) = [k-1,j,Sim_Matrix(k-1,j)];
    pore(index,:) = [porous_matrix(k-1,j)];
    index = index+1;
end
if (k+1>=1 && k+1<=b_matrix)
    neighbor_matrix(index,:) = [k+1,j,Sim_Matrix(k+1,j)];
    pore(index,:) = [porous_matrix(k+1,j)];
    index = index+1;
end
if (j-1>=1 && j-1<=l_matrix)
    neighbor_matrix(index,:) = [k,j-1,Sim_Matrix(k,j-1)];
    pore(index,:) = [porous_matrix(k,j-1)];
    index = index+1;
end
if (j+1>=1 && j+1<=l_matrix)
    neighbor_matrix(index,:) = [k,j+1,Sim_Matrix(k,j+1)];
    pore(index,:) = [porous_matrix(k,j+1)];
end

%Checking for non-porous cells
v = (pore(:,1) == 'N');
neighbor_matrix = neighbor_matrix(v,:);

%Checking for grid-cells with no biological cells
v = (neighbor_matrix(:,3) == 0);
neighbor_matrix = neighbor_matrix(v,1:2);

empty_num = size(neighbor_matrix,1);

