% 格林威治平恒星时(Greenwich hour angle)
% gham(year,month,day,hour,minute,second)计算UTC日期的格林威治平恒星时(rad)
% gham(mjd)计算UTC(MJD表示)时刻的格林威治平恒星时(rad)
% Example:
%    gham(2007,7,1,12,0,0)
%    gham(54282.5)
% See also: gha, ghamj2000, date2mjd
function SG = gham(Y,M,D,h,m,s)
if nargin == 6
    UTC  = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1 
    UTC = Y;
elseif nargin == 0
    warning('输入参数个数为0，自测试...');
    UTC = date2mjd(2006,1,1,0,0,0)
    sg0 = gham(UTC)
    sg1 = gham(UTC+365.25*9)
    dsg = mod(sg0+We*86400*365.25*9,2*pi)-sg1
else
    error('输入参数数目错误');
end
% last version:
% dt = utc2ut1(UTC) - mjd2000;
% SG = mod((280.4606184 + 360.985647365*dt + 0.2908e-12*dt^2)*rad,pi*2);

% JAT-EarthRef : IERS 1997   && Orbits-Xu p.14
ut1 = utc2ut1(UTC);
ut10 = floor(ut1);
T0 = (ut10 - mjd2000)/36525.0;
UT1 = (ut1-ut10)*86400;
alpha = 1.002737909350795 + (5.9006e-11 - 5.9e-12*T0)*T0;
SG = 6*3600 + 41*60 + 50.54841 + ...
    	 (8640184.812866 + (0.093104 - 6.2e-6*T0)*T0)*T0 + ...
		 alpha*UT1;
SG = mod(SG/3600.0*15.0,360)*rad;
