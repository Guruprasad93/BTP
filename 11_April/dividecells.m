%Cell proliferation in automaton

function [] = dividecells(k,j)
    
    run('global_variables');

    [neighbors,empty_num_neighbors] = cell_neighbors(k,j);
    if (empty_num_neighbors == 0)
		gridcell_stationaryphase(k,j);
    else
		randomposition = randi(empty_num_neighbors,1,1);
		mig_old = randi(4,1,1);
		Sim_Matrix(k,j) =1;
		mig_index(k,j) = mig_old;
		persctr(k,j) = checkpersctr(int2str(mig_old));
		divctr(k,j) = T_Div;
        glc_ctr(k,j) = starve_time; %Resetting food counter for daughter cells

		mig_new = randi(4,1,1);
		Sim_Matrix(neighbors(randomposition,1),neighbors(randomposition,2)) = 1;
		mig_index(neighbors(randomposition,1),neighbors(randomposition,2)) = mig_new;
		persctr(neighbors(randomposition,1),neighbors(randomposition,2)) = checkpersctr(int2str(mig_new));
		divctr(neighbors(randomposition,1),neighbors(randomposition,2)) = T_Div;
		gridsvisited(neighbors(randomposition,1),neighbors(randomposition,2)) = 'Y';
        glc_ctr(neighbors(randomposition,1),neighbors(randomposition,2)) = starve_time; %Resetting for daughtercell
    end
end



	

