%Major script for Automaton evolution

run('global_variables');
%Sim_Matrix, porous_matrix

%Variables
time_of_sim = 5000;

%Initial Glucose concentration in Scaffold
Conc_glucose = init_glc_conc();
%display('Initial Glucose Conc')
%reshape(Conc, l_matrix, b_matrix)

%Initial Growth factor concentration in Scaffold
%Conc_growthFactor = init_growthFactor_conc();
Conc_growthFactor = user_defined_growthFactor();
%write2file(0,Conc);

%display('Initial CEll matrix')
%reshape(Sim_Matrix, l_matrix, b_matrix)

%display('Porous grid')
%reshape(porous_matrix, l_matrix, b_matrix)

Cell_countfile = sprintf('%s','Cell_counter.txt');
fid = fopen(Cell_countfile,'w');



for time=1:time_of_sim
    Conc_glc = conc_glc_matrix(Conc_glucose);
    Conc_growth = growth_factor_diff(Conc_growthFactor);
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
			    			changedirofmigration(k,j,Conc_growth)
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
	Conc_glucose = Conc_glc;
    Conc_growthFactor = Conc_growth;
    %conc_glc_matrix(Conc);
	cell_ctr = count_cells();
	fprintf(fid, sprintf('%d%s',cell_ctr,'\n'));
    disp(['Time =' num2str(time)]);
	%Sim_Matrix, porous_matrix
	write2file(time,Conc_glucose,Conc_growthFactor);
end

fclose(fid);

	    
