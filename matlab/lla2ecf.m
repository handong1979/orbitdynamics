function u = lla2ecf(lon,lat,alt)
a = 6378.137;
e = 0.08181919;
u(1,1) = a*cosd(lon)/sqrt(1+(1-e^2)*tand(lat)^2) + alt*cosd(lon)*cosd(lat);
u(2,1) = a*sind(lon)/sqrt(1+(1-e^2)*tand(lat)^2) + alt*sind(lon)*cosd(lat);
u(3,1) = a*(1-e^2)*sind(lat)/sqrt(1-e^2*sind(lat)^2) + alt*sind(lat);