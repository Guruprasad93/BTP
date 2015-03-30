
%$Changing all Grid-points to "not Visited" if not a pore; else - Visited

for j=1:l_matrix
    for k=1:b_matrix
	if porous_matrix(k,j) == 'N'
	    gridsvisited(k,j) = 'N';
	else
	    gridsvisited(k,j) = 'Y';
	end
    end
end
