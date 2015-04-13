function [num_row] = row_number(position, l_matrix, b_matrix)

x = position(1);
y = position(2);


num_row = (y-1)*b_matrix+x;

end
