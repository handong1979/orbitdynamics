% 章动矩阵计算
%   nutation(year,month,day,hour,minute,second)计算UTC世界时时刻的章动矩阵
%   nutation(mjd)计算简约儒略日mjd时刻的章动矩阵
%
% 章动矩阵 nutation matrix
%   设有一个矢量在瞬时平赤道坐标系下的坐标为r，其在瞬时真赤道坐标系中的坐标为 Nutation*r
%   反之，一个矢量在瞬时真赤道坐标系下的坐标为r，其在瞬时平赤道坐标系中的坐标为 Nutation'*r
%   三个章动角u1,u2,u3的转序为Nutation = Rz(-(eps+deps))*Ry(dksi)*Rx(eps)
% 
% Example:
%      nutation(2006,1,1,0,0,0);
%      nutation(53736)
%
% See also nutation_angle, precession_angle, ecliptic_equator_angle, precession.
function nu = nutation(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('输入参数数目错误');
end
eps = ecliptic_equator_angle(TDT);
[dksi,deps] = nutation_angle(TDT);
nu = rotx(-eps-deps)*rotz(-dksi)*rotx(eps);