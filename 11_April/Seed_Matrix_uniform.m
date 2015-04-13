%Seeding Scaffold Uniformly

run('global_variables');

prob_seed = 0.9;

for j=1:l_matrix
    for k = 1:b_matrix
        if porous_matrix(k,j) == 'N'
            rand_number = rand>prob_seed;
            if (rand_number == 1)
                migration = randi(4,1,1);
                Sim_Matrix(k,j) = 1;
                mig_index(k,j) = migration;
                persctr(k,j) = checkpersctr(int2str(migration)); 
                divctr(k,j) = round(normrnd(960,125));
                gridsvisited(k,j) = 'N';
                glc_ctr(k,j) = starve_time;
            else
                Sim_Matrix(k,j) = 0;
                mig_index(k,j) = 0;
                persctr(k,j) = 0;
                divctr(k,j) = 0;
                gridsvisited(k,j) = 'N';
                glc_ctr(k,j) = 0;
            end
        end
    end
end

