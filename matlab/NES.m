% 地面站北东天坐标系到地固系的转换矩阵
function C = NES(Longitude,Latitude)
% syms lat lon
% roty(-lat)*rotz(lon)
% [ cos(lat)*cos(lon), cos(lat)*sin(lon), sin(lat)]
% [         -sin(lon),          cos(lon),    0]
% [-cos(lon)*sin(lat),-sin(lat)*sin(lon), cos(lat)]
if nargin == 0
    warning('self test');
    Longitude = 0;
    Latitude = 10;
    C = NES(Longitude,Latitude);
    return;
end
C = [-cosd(Longitude)*sind(Latitude), -sind(Longitude)*sind(Latitude),   cosd(Latitude); ...
     -sind(Longitude),                 cosd(Longitude),                  0;
      cosd(Longitude)*cosd(Latitude),  sind(Longitude)*cosd(Latitude),  sind(Latitude)];