% 速度增量引起的半长轴的小量变化量
% da = dadv(a,dv)
% dv:速度增量，单位m/s
% a:初始半长轴，单位km
% da:半长轴变化量，单位km
% Example:
%     da = dadv(7000,1)
%     da = dadv(42164,1)
% See also:
%    dvda dnda duda drda
function da = dadv(a,dv)
n = sqrt(GEarth/a^3);
da = 2/n*dv/1000;