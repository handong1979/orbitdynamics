% Áú¸ñ¿âËþ4½×»ý·Ö
function xk = rk4(dynfunc,step,x0)
if nargin == 0
    warning('self test');
    step = 1;
    x0 = 0;
    for t=1:360
        xk(t) = rk4(@(x)testfun(t,x),step,x0);
%         [tt,y] = ode45(@mcos(t,x),[0 1],x0)
        x0 = xk(t);
    end
    y = sin(rad:rad:2*pi);
%     y = 0.5*(1:1:360).^2;
    err = xk-y;
    subplot 211,plot(xk),hold on,plot(y);
    subplot 212,plot(err);
    return;
end
k1 = dynfunc(x0);
k2 = dynfunc(x0+0.5*step*k1);
k3 = dynfunc(x0+0.5*step*k2);
k4 = dynfunc(x0+step*k3);
xk = x0 + step/6*(k1+2*k2+2*k3+k4);

    function y = testfun(t,x)
        y = cos(2*pi/360*t);
%         y = t;
    end
end