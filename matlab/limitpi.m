% 将角度误差加减pi*2变成0附近的值
function y = limitpi(x)
% while any(x>pi)
    flag = x>pi;
    x = x - pi*2*flag;
% end
% while any(x<-pi)
    flag = x<-pi;
    y = x + pi*2*flag;
% end