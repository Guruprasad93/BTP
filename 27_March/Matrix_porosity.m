%Initializing porous Matrix
%%Done in initialize script

num_pores = 0.5*(l_matrix*b_matrix);

for i=1:num_pores
    
    row_pos = randi(b_matrix,1,1);
    col_pos = randi(l_matrix,1,1);
    porous_matrix(row_pos,col_pos) = 'Y';
    gridsvisited(row_pos,col_pos) = 'Y';
end

