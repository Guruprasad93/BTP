function [neighbors] = corner_neighbor(position, extra_l, extra_b)

x = position(1);
y = position(2);

%Checking L&R of position

index = 1;

if (x-1>0) && (x-1<=extra_b)
    pos = [x-1,y];
    neighbors(index,:) = ((pos(2)-1)*extra_b+pos(1));
    index = index+1;
end

if (x+1>0) && (x+1<=extra_l)
    pos = [x+1,y];
    neighbors(index,:) = ((pos(2)-1)*extra_b+pos(1));
    index = index+1;
end
