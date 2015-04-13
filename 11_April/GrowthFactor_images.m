%Script to convert all Conc_GrowthFactor files to Images - GrowthFactor concentration

run('global_variables');
%run('initialize_values');
l_matrix = 50;
ctr = 1;
cd 2D_Concgrowth_files
for i = 1:10:5000
    filename = sprintf('%s_%d.%s','ConcGrowth',i,'txt');
    imgname = sprintf('%s_%d.%s','/home/guru/BTP/11_April/GFactor_images/GFactor_Conc',ctr,'png');
    A = importdata(filename);
    A = reshape(A, [],l_matrix+2);
    A = A(2:l_matrix+1,2:l_matrix+1);
    %A = A/max(max(A));
    h = imagesc(A);
    saveas(h, imgname, 'png');
    ctr = ctr+1;
    
end
