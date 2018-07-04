% 相对J2000平春分点的格林威治平恒星时(Greenwich hour angle)
% "相对J2000平春分点的格林威治平恒星时"并没有确切的物理量定义，
%  是为了弥补之前卫星轨道计算中的缺陷而凑出来的量,请谨慎使用
% ghamj2000(year,month,day,hour,minute,second)
%        计算UTC日期的相对J2000平春分点的格林威治平恒星时(rad)
% ghamj2000(mjd)计算UTC(MJD表示)时刻的相对J2000平春分点的格林威治平恒星时(rad)
% Example:
%    ghamj2000(2007,7,1,12,0,0)
%    ghamj2000(54282.5)
% See also: gham, gha, date2mjd
function SG = ghamj2000(Y,M,D,h,m,s)
warning('"相对J2000平春分点的格林威治平恒星时"并没有确切的物理量定义，');
warning('是为了弥补之前卫星轨道计算中的缺陷而凑出来的量,请谨慎使用');
if nargin == 6
    UTC  = date2mjd(Y,M,D,h,m,s);
elseif nargin ==1 
    UTC = Y;
else
    error('输入参数数目错误');
end
dt = utc2ut1(UTC) - mjd2000;
[zeta theta Z] = precession_angle(UTC);
SG = mod((280.4606184 + 360.985647365*dt + 0.2908e-12*dt^2)*rad - zeta - Z,pi*2);