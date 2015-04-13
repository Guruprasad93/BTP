%Writing To file for Image!

function [] = write2file(time,Concglc,Concgrowth)

run('global_variables');

index = 1;

for j=1:l_matrix
	for k=1:b_matrix
		Net_Matrix(index,:) = [k,j,divctr(k,j),glc_ctr(k,j),Sim_Matrix(k,j)];
		index = index+1;
	end
end

v = (Net_Matrix(:,5) == 1);
Matrix = Net_Matrix(v,1:4);

filename = sprintf('%s_%d.%s','2D_porous_files/file',time,'txt');
file_glc = sprintf('%s_%d.%s','2D_Concglc_files/ConcGlc',time,'txt');
file_growthfactor = sprintf('%s_%d.%s','2D_Concgrowth_files/ConcGrowth',time,'txt');
dlmwrite(filename,[Matrix(:,1),Matrix(:,2),Matrix(:,3),Matrix(:,4)],'delimiter','\t');
dlmwrite(file_glc,Concglc);
dlmwrite(file_growthfactor,Concgrowth);

end
