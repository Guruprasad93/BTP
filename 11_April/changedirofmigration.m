%Changing direction of migration

function [] = changedirofmigration(k,j,Conc_growthFactor)

    run('global_variables');
    [neighbors, empty_num_neighbors] = cell_neighbors(k,j);
    if (empty_num_neighbors == 0)
        gridcell_stationaryphase(k,j);
    else
        %randomposition = randi(empty_num_neighbors,1,1);
        %Position selected based on growth factor gradient
        randomposition = conc_gradient_newpos(Conc_growthFactor,neighbors,empty_num_neighbors);
        
        migdir = checkmigdir(k,j,neighbors(randomposition,:));
        tdiv = divctr(k,j);
        food_ctr = glc_ctr(k,j);
	
        %NEW GRID CELL
        Sim_Matrix(neighbors(randomposition,1),neighbors(randomposition,2)) = 1;
        mig_index(neighbors(randomposition,1),neighbors(randomposition,2)) = migdir;
        persctr(neighbors(randomposition,1),neighbors(randomposition,2)) = checkpersctr(num2str(migdir));
        divctr(neighbors(randomposition,1),neighbors(randomposition,2)) = tdiv;
        glc_ctr(neighbors(randomposition,1),neighbors(randomposition,2)) = food_ctr; %Cell movement

        %OLD GRID CELL
        Sim_Matrix(k,j) = 0;
        mig_index(k,j) = 0;
        persctr(k,j) = 0;
        divctr(k,j) = 0;
        glc_ctr(k,j) = 0;
        	
    	%Updating grids visited	
        gridsvisited(neighbors(randomposition,1),neighbors(randomposition,2)) = 'Y';
    end
end

