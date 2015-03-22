

l_matrix = 5;
b_matrix = 5;
h_matrix = 5;
r = 0.15;
time_of_sim = 10;

%Initial Concentration values


row_in_col = 1;

LHS_A = zeros(l_matrix*b_matrix*h_matrix);
RHS_A = zeros(l_matrix*b_matrix*h_matrix);


for z = 1:h_matrix
    for y = 1:l_matrix
        for x = 1:b_matrix
            position = [x,y,z];
            neighbors = find_neighbors(position);
            LHS_A(row_in_col,row_in_col) = 1+6*r;
            LHS_A(row_in_col,neighbors(1:end)) = -r;
            RHS_A(row_in_col,row_in_col) = 1 -6*r;
            RHS_A(row_in_col,neighbors(1:end)) = r;
            row_in_col = row_in_col + 1;
        end
    end
end


for time = 1:time_of_sim

    %filename = sprintf('%s_%d.%s','/home/guru/BTP/18_March/Conc_Matrix/Conc_Matrix',time,'txt');
    %file_image = sprintf('%s_%d.%s','/home/guru/BTP/18_March/Conc_Images/img',time,'png');
    Intermediate = RHS_A*Conc;
   
    Conc_time = LHS_A\Intermediate;

    %finalConcMatrix = vec2mat(Temp_time,l_matrix);
    
    %csvwrite(filename, finalConcMatrix)
    
    %finalConcMatrix = finalConcMatrix/max(finalConcMatrix(:));
    %imagesc(finalConcMatrix)
    %print(file_image,'-dpng');
    %pause(0.01)
    Conc = Conc_time;
end



