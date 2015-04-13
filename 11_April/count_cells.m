function [cell_ctr] = count_cells()

    run('global_variables');
    v = (Sim_Matrix == 1);
    cell_ctr = sum(sum(v));

end
