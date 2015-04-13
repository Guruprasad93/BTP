%Taking curser defined input from user for growth factor placement in scaffold
function [Conc_profile] = user_defined_growthFactor()
run('global_variables');

l_matrix = 50;

box on
axis([1,l_matrix,1,l_matrix])

[row,col] = find(porous_matrix == 'Y');
scatter(row,col,'filled')

%Taking 10 inputs
[x,y] = ginput(10);
x = round(x);
y = round(y);
ctr = 1;
removed = [];
for index = 1:10
    a = find(row == x(index));
    b = find(col == y(index));
    v = ismember(a,b);
    z = all(v == 0);
    if z == 0
        removed(ctr,:) = [index];
        ctr = ctr+1;
    end
end

x([removed(1:end)]) = [];   
y([removed(1:end)]) = [];


init_conc = 2;

Conc_profile = zeros(l_matrix+2);
Conc_profile(sub2ind(size(Conc_profile),x+1,y+1)) = init_conc;
Conc_profile = reshape(Conc_profile, [], 1);
