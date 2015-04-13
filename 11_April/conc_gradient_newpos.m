function [newposition] = conc_gradient_newpos(Conc_growthFactor,neighbors,empty_number)

run('global_variables');

max = -Inf;
Conc_Matrix = reshape(Conc_growthFactor,[],l_matrix+2);


for index = 1:empty_number
    x = neighbors(index,1);
    y = neighbors(index,2);
    growthFactorConc = Conc_Matrix(x+1,y+1);
    if max<growthFactorConc
        max = growthFactorConc;
        newposition = index;
    end
end


    
