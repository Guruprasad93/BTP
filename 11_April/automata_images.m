%Generate Automata images (with cells) 
%Distinguish cells based on age!

cd 2D_porous_files

for i=1:10:5000
    filename = sprintf('%s_%d.%s','file',i,'txt');
    imgname = sprintf('%s_%d.%s', '2D_images/Cell_img',i,'png');
    Matrix = importdata(filename);
    x = Matrix(:,1);
    y = Matrix(:,2);
    c = Matrix(:,3);
    
    %Making 4 bins [(0-300),(300-600),(600-900),(900-1300)] - Time in minutes
    
    c(c>=0 & c<300) = 1;
    c(c>=300 & c<600) = 2;
    c(c>=600 & c<900) = 3;
    c(c>=900) = 4;
    
    %h = figure();
    gscatter(x,y,c);
    legend('15-20 hrs','10-15 hrs','5-10 hrs','0-5 hrs')
    saveas(gcf,imgname);
end