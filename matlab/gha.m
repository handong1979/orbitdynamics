% 格林威治真恒星时(Greenwich hour angle)
% gha(year,month,day,hour,minute,second)计算UTC日期的格林威治真恒星时(rad)
% gha(mjd)计算UTC(MJD表示)时刻的格林威治真恒星时(rad)
% Example:
%    gha(2007,7,1,12,0,0)
%    gha(54282.5)
% See also: gham, ghamj2000, date2mjd
function SG = gha(Y,M,D,h,m,s)
if nargin == 6
    UTC  = date2mjd(Y,M,D,h,m,s);
elseif nargin ==1 
    UTC = Y;
else
    error('输入参数数目错误');
end
dksi = nutation_angle(UTC);
eps = ecliptic_equator_angle(UTC);
SG = mod(dksi*cos(eps)+gham(UTC),pi*2);
