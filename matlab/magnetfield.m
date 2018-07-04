% 简化地磁场
function [Bo,B] = magnetfield(lon,lat,a,u,i)
persistent g0 g1 h1 R0
if isempty(g0)
    g0 = -29439.5; % -29556.8~2005     -29439.5~2015
    g1 = -1502.4;% -1671.8~2005   -1502.4~2015
    h1 = 4801.1; % 5080~2005   4801.1~2015
    R0 = 6371.2; % 需求中使用6378.138,多了0.109%
    lamdam = atan2(h1,g1)*deg;
    dg0 = 11.4;
    dg1 = 16.7;
    dh1 = -28.8;
end

rs3 = (R0/a)^3;
br = (2*g0*sin(lat) + 2*(g1*cos(lon) + h1*sin(lon)*cos(lat)))*rs3;
bl = (g1*sin(lon) - h1*cos(lon))*rs3;
bd = (g0*cos(lat) - (g1*cos(lon) + h1*sin(lon))*sin(lat))*rs3;
psi = -atan(cos(u)*tan(i));
Bo = nan(3,1);
Bo(1) = bl*cos(psi) + bd*sin(psi);
Bo(2) = -bl*sin(psi) + bd*cos(psi);
Bo(3) = -br;
if nargout == 2
    B = [br;bl;bd];
end