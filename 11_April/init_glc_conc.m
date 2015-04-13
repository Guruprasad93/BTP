% Initializing Glucose Concentration in 2D scaffold

function [Conc] = init_glc_conc()
	
	run('global_variables');
	Cbulk = 5;
	r = 0.08;

	Conc = zeros(l_matrix*b_matrix,1);
    
   
% 	for y = 1:l_matrix
% 		for x = 1:b_matrix
% 			position = [x,y];
% 			Total_neighbors = num_neighbors(position,l_matrix, b_matrix);
% 			if (Total_neighbors <4)
% 				row_num = row_number(position, l_matrix, b_matrix);
% 				Conc(row_num) = Cbulk;
% 			end
% 		end
%     end
    
    Conc(1:end) = Cbulk;    
        
    %No concentration in the pores of scaffold
    pore_grids = reshape(porous_matrix,[],1);
    v = (pore_grids=='Y');
    Conc(v) = 0;
    
    
    
end



