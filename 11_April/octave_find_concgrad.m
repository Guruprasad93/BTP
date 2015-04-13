function [neighbors] = octave_find_concgrad(position,extra_l, extra_b)

%l_matrix = 5;
%b_matrix = 5;
%h_matrix = 5;

x = position(1);
y = position(2);

index = 1;

pos(1,:) = [x-1,y];
pos(2,:) = [x+1,y];
pos(3,:) = [x,y+1];
pos(4,:) = [x,y-1];


for index = 1:4
    neighbors(index,:) = (pos(index,2)-1)*extra_b+pos(index,1);
end


%Checking L&R of position

% if (x-1>0) && (x-1<=b_matrix)
%     pos = [x-1,y];
%     neighbors(index,:) = ((pos(2)-1)*b_matrix+pos(1));
%     index = index+1;
% end
% 
% if (x+1>0) && (x+1<=b_matrix)
%     pos = [x+1,y];
%     neighbors(index,:) = ((pos(2)-1)*b_matrix+pos(1));
%     index = index+1;
% end
% 
% if (y+1>0) && (y+1<=l_matrix)
%     pos = [x,y+1];
%     neighbors(index,:) = ((pos(2)-1)*b_matrix+pos(1));
%     index = index+1;
% end
% 
% 
% if (y-1>0) && (y-1<=l_matrix)
%     pos = [x,y-1];
%     neighbors(index,:) = ((pos(2)-1)*b_matrix+pos(1));
%     index = index+1;
% end


neighbors = sort(neighbors);

end
