% »­Á½¸ösubplotÇúÏß
% [a1 a2] = plot2(data)
% while data is n*2 matrix,
%   use plot(data(:,1) and plot(data(:,2)),
%   a1 a2 return two axes' handle
% while data is n*3 matrix,
%   use plot(data(:,1),data(:,2)) and plot(data(:,1),data(:,3))
%   a1 a2 return two axes' handle

function [a1 a2]=plot2(data)

[m n] = size(data);

if n==2
    subplot 211,a1 = plot(data(:,1));grid on;
    subplot 212,a2 = plot(data(:,2));grid on;
elseif n==3
    subplot 211,a1 = plot(data(:,1),data(:,2));grid on;
    subplot 212,a2 = plot(data(:,1),data(:,3));grid on;
else
    error('plot2: Input must be n*2 or n*3 matrix');
end
        
        