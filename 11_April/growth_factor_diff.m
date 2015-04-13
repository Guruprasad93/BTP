%Making LHS and RHS Matrix

function [Conc] = growth_factor_diff(Conc_profile)

run('global_variables');

r = 0.001;
%init_conc = 2;

%RHS_A = zeros((l_matrix+2)*(b_matrix+2));
%LHS_A = zeros((l_matrix+2)*(b_matrix+2));

RHS_A = sparse((l_matrix+2)*(b_matrix+2),(l_matrix+2)*(b_matrix+2));
LHS_A = sparse((l_matrix+2)*(b_matrix+2),(l_matrix+2)*(b_matrix+2));

%Conc_profile = zeros((l_matrix+2)*(b_matrix+2),1);
%Conc_profile(13) = init_conc;

for x = 2:b_matrix+1
    for y = 2:l_matrix+1
        position = [x,y];
        [neighbors] = octave_find_concgrad(position,l_matrix+2,b_matrix+2);
        row_num = row_number(position,l_matrix+2,b_matrix+2);
        LHS_A(row_num, row_num) = (1+4*r);
        RHS_A(row_num, row_num) = (1-4*r);
        LHS_A(row_num, neighbors(1:end)) = -r;
        RHS_A(row_num, neighbors(1:end)) = r;
    end
end

for rowNumber = 1:(l_matrix+2)*(b_matrix+2)
    if sum(LHS_A(rowNumber,:) == 0) == ((l_matrix+2)*(b_matrix+2))
        LHS_A(rowNumber,rowNumber) = 1;
        RHS_A(rowNumber,rowNumber) = 1;
    end
end

Intermediate = RHS_A*Conc_profile;
Conc_time = LHS_A\Intermediate;
Conc = Conc_time;

%Maintaining zero concentration at pores
pore_grids = reshape(porous_matrix,[],1);
v = (pore_grids=='Y');
Conc(v) = 0;

%Equalizing concentration of matrix to extended matrix => to ensure flux = 0

for index = 2:l_matrix+1
    left_pos = [index,1];
    right_pos = [index,l_matrix+2];
    up_pos = [1,index];
    bottom_pos = [b_matrix+2,index];
    Conc((left_pos(2)-1)*(l_matrix+2)+left_pos(1)) = Conc(left_pos(2)*(l_matrix+2)+left_pos(1));
    Conc((right_pos(2)-1)*(l_matrix+2)+right_pos(1)) = Conc((right_pos(2)-2)*(l_matrix+2)+right_pos(1));
    Conc((up_pos(2)-1)*(l_matrix+2)+up_pos(1)) = Conc((up_pos(2)-1)*(l_matrix+2)+up_pos(1)+1);
    Conc((bottom_pos(2)-1)*(l_matrix+2)+bottom_pos(1)) = Conc((bottom_pos(2)-1)*(l_matrix+2)+bottom_pos(1)-1);
end

Corners(1,:) = [1,1];
Corners(2,:) = [1,l_matrix+2];
Corners(3,:) = [b_matrix+2,1];
Corners(4,:) = [l_matrix+2,b_matrix+2];

for index = 1:4
    pos = Corners(index,:);
    row_num = row_number(pos,l_matrix+2,b_matrix+2);
    cor_neighbor = corner_neighbor(pos,l_matrix+2, b_matrix+2);
    Conc(row_num) = Conc(cor_neighbor);
end


        