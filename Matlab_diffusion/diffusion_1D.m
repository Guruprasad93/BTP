function [] = diffusion_1D()

    
    %Initial Temperature
    Temp = [0 20 40 60 80 100];
    
    d_param = 0.25;
    diag = 1+d_param;
    time_of_sim = 70;
    
    %Initializing with zeros matrix
    LHS_A = zeros(6);
    
    %LHS - Transformation Matrix
    for row = 1:6
        col = imm_neighbors1D(row);
        LHS_A(row,col(1:end)) = -d_param/2;
        
        if row == 1 || row == 6
            LHS_A(row,row) = 1+d_param/2;
        else
            LHS_A(row,row) = 1+d_param;
        end
    end
    
    
    
    %Initializing RHS Matrix
    RHS_A = zeros(6);
    
    %RHS - Transformation Matrix
    for row = 1:6
        col = imm_neighbors1D(row);
        RHS_A(row,col(1:end)) = d_param/2;
        
        if row == 1 || row == 6
            RHS_A(row,row) = 1-d_param/2;
        else
            RHS_A(row,row) = 1-d_param;
        end
        
    end

    
    for time = 1:time_of_sim
        Intermediate = RHS_A*Temp';
        Temp_time = LHS_A\Intermediate;
        %Temp_time(1) = 0;
        %Temp_time(end) = 0;
        Temp = Temp_time'
        
    
    end

    

    
            
            
            
    