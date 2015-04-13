% Calculating COncentration of Scaffold after every time step

function [Conc] = conc_glc_matrix(Conc_profile)

	run('global_variables');
    min_glc = 4.75*10^-2;
    r = 0.08;
    Cbulk = 5;
	row_in_col = 1;
	LHS_A = sparse((l_matrix*b_matrix),(l_matrix*b_matrix));
	RHS_A = sparse((l_matrix*b_matrix),(l_matrix*b_matrix));

	for y = 1:l_matrix
		for x = 1:b_matrix
			position = [x,y];
			Total_neighbors = num_neighbors(position, l_matrix, b_matrix);
			if (Total_neighbors == 4)
				neighbors = octave_find(position, l_matrix, b_matrix);
				LHS_A(row_in_col, row_in_col) = 1+4*r;
				LHS_A(row_in_col, neighbors(1:end)) = -r;
				RHS_A(row_in_col, neighbors(1:end)) = r;
				RHS_A(row_in_col, row_in_col) = 1-4*r;
				row_in_col = row_in_col + 1;
			else
				LHS_A(row_in_col, row_in_col) = 1;
				RHS_A(row_in_col, row_in_col) = 1;
				row_in_col = row_in_col + 1;
			end
		end
    end

    
       
    for j = 1:l_matrix
        for k = 1:b_matrix
            if Sim_Matrix(k,j) == 1
                row_num = row_number([k,j],l_matrix, b_matrix);
                %disp(['Glc concentration:' num2str(Conc_profile(row_num)) '  Min_glc:' num2str(min_glc)])
                if Conc_profile(row_num)>min_glc
                    Conc_profile(row_num) = (Conc_profile(row_num) - min_glc);
                    glc_ctr(k,j) = starve_time; %Resetting glucose counter
                    
                else
                    Conc_profile(row_num) = 0;
                    %Check if food counter ==0 
                    if (glc_ctr(k,j) == 0)
                        %Death of cell
                        Sim_Matrix(k,j) = 0;
                        mig_index(k,j) = 0;
                        persctr(k,j) = 0;
                        divctr(k,j) = 0;
                        glc_ctr(k,j) = 0;
                    else
                        glc_ctr(k,j) = glc_ctr(k,j) -1;
                    end
                end
            end
        end
    end
    %display('Cell Matrix')
    %reshape(Sim_Matrix, l_matrix, b_matrix)
    
    %display('After consuming glucose')
    %reshape(Conc_profile, l_matrix, b_matrix)
    
	Intermediate = RHS_A*Conc_profile;
	Conc_time = LHS_A\Intermediate;
	Conc = Conc_time;
    
    %Ensuring borders are of bulk concentration
    for y = 1:l_matrix
		for x = 1:b_matrix
			position = [x,y];
			Total_neighbors = num_neighbors(position,l_matrix, b_matrix);
			if (Total_neighbors <4)
				row_num = row_number(position, l_matrix, b_matrix);
				Conc(row_num) = Cbulk;
			end
		end
    end
    
    
    %Maintaining zero concentration at pores
    pore_grids = reshape(porous_matrix,[],1);
    v = (pore_grids=='Y');
    Conc(v) = 0;
    
    %reshape(Conc,l_matrix, b_matrix)
    
end




	
