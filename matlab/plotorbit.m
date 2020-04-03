%  以轨道根数elem为初值，按照二体运动绘制一个轨道周期的三维诡计
function rv = plotorbit(elem,dt,varargin)
n = sqrt(GEarth/elem(1)^3);
if nargin == 1
    dt = 2*pi/n;
end
M0 = elem(6);
Mend = dt*n*deg;
M = [0:1:Mend,Mend];
len = length(M);
rv = zeros(len,6);
for k=1:len
    elem(6) = M0 + M(k)*rad;
    rv(k,1:6) = kepler2cart(elem);
end
plot3(rv(:,1),rv(:,2),rv(:,3),varargin{:}),grid on,hold on;
