function [neighbors] = find_neighbors(position)

l_matrix = 5;
b_matrix = 5;
h_matrix = 5;

x = position(1);
y = position(2);
z = position(3);

index = 1;

%Checking L&R of position

if (x-1>0) && (x-1<=b_matrix)
    pos = [x-1,y,z]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end

if (x+1>0) && (x+1<=b_matrix)
    pos = [x+1,y,z]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end

if (y+1>0) && (y+1<=l_matrix)
    pos = [x,y+1,z]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end


if (y-1>0) && (y-1<=l_matrix)
    pos = [x,y-1,z]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end


if (z-1>0) && (z-1<=h_matrix)
    pos = [x,y,z-1]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end

if (z+1>0) && (z+1<=h_matrix)
    pos = [x,y,z+1]
    neighbors(index,:) = ((pos(3)-1)*l_matrix*b_matrix+(pos(2)-1)*b_matrix+pos(1));
    index = index+1;
end

neighbors = sort(neighbors);
