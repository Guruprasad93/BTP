%Major script for Automaton evolution

run('global_variables');
%Sim_Matrix, porous_matrix

%Variables
time_of_sim = 100;

for time=1:time_of_sim
    run('change_grid_status');
    for j=1:l_matrix
		for k=1:b_matrix
	    	if(gridsvisited(k,j) == 'Y')
	        	%disp('Moving on')
	    	else
	    		op1 = checkcell(k,j);
	        	if (op1 == 1)
		    		op2 = checkdivctr(k,j);
		    		if (op2 == 0)
		        		dividecells(k,j);
		    		else
		    			divctr(k,j) = divctr(k,j) -1;
						op3 = checkpersistencectr(k,j);
						if (op3 == 0)
			    			changedirofmigration(k,j)
						else
			    			persctr(k,j) = persctr(k,j) -1;
			    			cell_migrate(k,j);
		   				end
		    		end
				end
	    	end
		end
    end
	%run('view_Matrix');
	disp(['Time =' num2str(time)]);
	%Sim_Matrix, porous_matrix
	write2file(time);
end


	    
