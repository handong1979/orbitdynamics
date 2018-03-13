% 地球极移矩阵
% polewander(year,month,day,hour,minute,second)计算UTC时刻的极移矩阵
% polewander(UTC)计算UTC(简约儒略日表示)时刻的极移矩阵
% 
% 地球极移矩阵为：
% [1,   0,   Xp]
% [0,   1,  -Yp]
% [Xp,  Yp,   1]
%   设有一个矢量在准地固坐标系下的坐标为r，其在地固坐标系中的坐标为 PoleWander*r
%   反之,一个矢量在地固坐标系下的坐标为r，其在准地固坐标系中的坐标为 PoleWander'*r
% 
% Example:
%    polewander(year,month,day,hour,minute,second)
%    polewander(mjd)
%  
% See also:  precession, nutation
function pw = polewander(Y,M,D,h,m,s)
if nargin == 6
    UTC = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1
    UTC = Y;
else
    error('输入参数数目错误');
end
% 在程序第一次运行到这里时,eop=[]，运行load eop.mat
% 由于eop是persistent变量，当再次调用次函数时，eop已有值,则无需再次load
persistent eop;
if isempty(eop)
    load eop.mat;
end

if UTC > eop(length(eop),1) || UTC<eop(1,1)
    % 时间超出表中时间范围
    Xp = 0.0;
    Yp = 0.0;
else
    % 插值得到UTC时的Xp,Yp
    Xp = interp1(eop(:,1),eop(:,2),UTC)/3600*rad;
    Yp = interp1(eop(:,1),eop(:,3),UTC)/3600*rad;
end

pw = [1,   0,   Xp;...
      0,   1,  -Yp;...
      Xp,  Yp,   1];
