% 太阳同步轨道倾角、高度的计算
% function y = sunsyn(x)
% if x<pi          x为倾角(弧度)
% elseif x<=180    x为倾角(度)
% elseif x<6378    x为轨道高度(km)
% else             x为轨道半长轴

function y = sunsyn(x)
if x<pi % x为倾角(弧度)
    y = (-0.9856/9.97/Re^3.5/cos(i))^(-2/7);
elseif x<=180 % x为倾角(度)
    y = (-0.9856/9.97/Re^3.5/cos(x*rad))^(-2/7);
elseif x<6378 % x为轨道高度(km)
    y = acos( -0.9856/9.97/((x+Re)/Re)^3.5 ) * deg;
else % x为轨道半长轴
    y = acos( -0.9856/9.97/(x/Re)^3.5 ) * deg;
end