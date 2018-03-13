% Áú¸ñ¿âËþ4½×»ý·Ö
function xk = rk4(dynfunc,step,x0)
if nargin == 0
    warning('self test');
    step = 1;
    y = 0;
    t = 0;
    for i=1:360
        y = rk4(@(y)testfun(t,y),step,y);
        yl(i) = y;
        t = i;
    end
%     yr = 360/2/pi*sin(2*pi/360*(0:359));
    yr = 360/2/pi*sin(2*pi/360*(1:360));
    subplot 211,plot(yl),hold on,plot(yr);
    subplot 212,plot(yl-yr);
    return
end
k1 = step*dynfunc(x0);
k2 = step*dynfunc(x0+0.5*k1);
k3 = step*dynfunc(x0+0.5*k2);
k4 = step*dynfunc(x0+k3);
xk = x0 + 1/6*(k1+2*k2+2*k3+k4);

end

function y = testfun(t,y)
y = cos(2*pi/360*t);
% y = cos(y);
end