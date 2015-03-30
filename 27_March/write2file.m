%Writing To file for Image!

function [] = write2file(time)

run('global_variables');

index = 1;

for j=1:l_matrix
	for k=1:b_matrix
		Net_Matrix(index,:) = [k,j,Sim_Matrix(k,j)];
		index = index+1;
	end
end

v = (Net_Matrix(:,3) == 1);
Matrix = Net_Matrix(v,1:2);

filename = sprintf('%s_%d.%s','/home/guru/BTP/27_March/2D_porous_files/file',time,'txt');

dlmwrite(filename,[Matrix(:,1),Matrix(:,2)],'delimiter','\t');

end
