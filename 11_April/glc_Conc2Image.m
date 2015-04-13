%Script to convert all Conc files to Images - Glucose concentration

run('global_variables');
%run('initialize_values');
l_matrix = 50;
cd 2D_Concglc_files
for i = 1:10:5000
    filename = sprintf('%s_%d.%s','ConcGlc',i,'txt');
    imgname = sprintf('%s_%d.%s','/home/guru/BTP/11_April/Glc_images/Glc_Conc',i,'png');
    A = importdata(filename);
    A = reshape(A, [],l_matrix);
    %A = A/max(max(A));
    h = imagesc(A);
    saveas(h, imgname, 'png');
end

    

