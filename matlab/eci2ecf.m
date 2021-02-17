% 惯性系到地固系转换矩阵
%   eci2ecf(year,month,day,hour,minute,second)计算UTC世界时时刻的惯性系到地固系转换矩阵
%   eci2ecf(mjd)计算简约儒略日mjd时刻的惯性系到地固系转换矩阵
%
% Example:
%      eci2ecf(2006,1,1,0,0,0)
%      eci2ecf(53736)
%
% See also precession, nutation, gha, polewander
function c = eci2ecf(Y,M,D,h,m,s)
if nargin == 6
    UTC = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1
    UTC = Y;
else
    error('输入参数数目错误');
end
c = polewander(UTC)*rotz(gha(UTC))*nutation(UTC)*precession(UTC);