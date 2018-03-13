% 岁差矩阵计算
%   precessioin(year,month,day,hour,minute,second)计算UTC世界时时刻的岁差矩阵
%   precessioin(mjd)计算简约儒略日mjd时刻的岁差矩阵
%
%   设有一个矢量在J2000.0惯性坐标系下的坐标为r，其在瞬时平赤道坐标系中的坐标为 precession*r
%   反之，一个矢量在瞬时平赤道坐标系下的坐标为r，其在J2000.0惯性坐标系中的坐标为 precession'*r
%   三个岁差角u1,u2,u3的转序为Precession = Rz(-Z)*Ry(theta)*Rz(-zeta)
% 
% Example:
%      precession(2006,1,1,0,0,0);
%      precession(53736)
%
% See also precession_angle, nutation_angle, nutation
function PR = precession(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('输入参数数目错误');
end
[zeta,theta,Z] = precession_angle(TDT);
PR = rotz(-Z)*roty(theta)*rotz(-zeta);