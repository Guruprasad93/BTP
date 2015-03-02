function [col] = imm_neighbors1D(row)

    lowest = 1;
    highest = 6;
    
    %for 1D
    n1 = row-1;
    n2 = row+1;
    
    if (n1>=lowest && n1<=highest) 
    
        col(1) = n1;
        if (n2>=lowest && n2<=highest)
        col(2) = n2;
        end
    
    else
        col(1) = n2;
        
    end

end
       
        