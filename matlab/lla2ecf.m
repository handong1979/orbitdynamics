% 由地理经纬度、高度，计算地固位置
% u = lla2ecf(lon,lat,alt)
% 输入： lon地理经度，单位弧度
%        lat地理纬度，单位弧度
%        alt高度，单位km
% 输出： 地固系位置u

function u = lla2ecf(lon,lat,alt)
% a = 6378.137;
e = 0.08181919;
% u(1,1) = a*cosd(lon)/sqrt(1+(1-e^2)*tand(lat)^2) + alt*cosd(lon)*cosd(lat);
% u(2,1) = a*sind(lon)/sqrt(1+(1-e^2)*tand(lat)^2) + alt*sind(lon)*cosd(lat);
% u(3,1) = a*(1-e^2)*sind(lat)/sqrt(1-e^2*sind(lat)^2) + alt*sind(lat);
% 两组公式完全等价，下面公式计算量小
N = 6378.137/sqrt(1-e^2*sin(lat)*sin(lat));
u = [(N+alt)*cos(lon)*cos(lat);...
     (N+alt)*sin(lon)*cos(lat);...
     (N*(1-e^2)+alt)*sin(lat)];