for j =1:l_matrix
    for k =1:b_matrix
	disp(['SIM:' num2str(Sim_Matrix(k,j)) ' Mig_index:' num2str(mig_index(k,j)) ' Pers_ctr:' num2str(persctr(k,j)) ' Div_ctr:' num2str(divctr(k,j)) ' Position:' num2str(k) ' ' num2str(j) ' Glc_ctr:' num2str(glc_ctr(k,j))])
    end
end
