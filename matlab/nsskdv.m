% 分年份计算每年的南北位保速度增量
function [dvyear,dvall] = nsskdv(year)
if nargin == 0 % self test
    year = 2022:2030;
%     year = 2019:2038;
end
    
n = length(year);
year = year(:);
mjd = date2mjd([year,7*ones(n,1),ones(n,1),zeros(n,3)]);
T = (mjd - mjd2000)/36525;
Omegam = 125.0445555 - 1934.136185*T + 7.476/3600*T.^2;
dix = -3.5e-4*sind(Omegam)*365;
diy = (22.79e-4+2.59e-4*cosd(Omegam))*365;
di = sqrt(dix.^2+diy.^2);
dvyear = 3074.659*di*rad;
dvall = sum(dvyear);

if nargin == 0
    plot(year-2000,dvyear,'-o'),grid on;
end