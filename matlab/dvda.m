% 半长轴的小量变化所需速度增量
% dv = dvda(a,da)
% dv:速度增量，单位m/s
% a:初始半长轴，单位km
% da:半长轴变化量，单位km
% Example:
%     dv = dvda(7000,1)
%     dv = dvda(42164,1)
% See also:
%    dadv dnda duda drda
function dv = dvda(a,da)
n = sqrt(GEarth/a^3);
dv = n/2*da*1000;