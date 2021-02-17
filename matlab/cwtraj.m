% 根据初始状态计算CW轨迹
% [traj,T] = cwtraj(x0,w,T,h)
%    x0: 初始时刻位置速度，可输入4维或6维矢量，如果输入4维矢量则认为是
%        轨道平面内的位置速度，输出traj也是4维矢量组成的轨迹；如果输入6维矢量，
%        则认为是三维位置和三维速度，输出traj也是6维矢量组成的轨迹。
%        位置速度矢量的长度单位可以是m也可以是km，输出将于输入单位相同
%     w: 轨道角速度,单位：弧度/秒 w=sqrt(miu/r^3)
%     T: 轨迹时间长度，单位：秒
%     h: 轨迹点间隔（步长），单位：秒
% Example：
%  1. GEO轨道绕飞
%  ag = 42164.2;
%  b = 35; % 绕飞半短轴
%  kt = [ag,0,0,0,0,0];
%  kc = [ag,b/ag,0,0,0,0];
%  rv = tcr(kt,kc);
%  traj = cwtraj(rv,We,86400,60);
% See Also: cwstm4, csstm6, cw2p
function [traj,t] = cwtraj(x0,w,T,h)
if ~isvector(x0)
    error('输入初始状态x0必须为矢量');
end
[m,n] = size(x0);
dim = max(m,n);
if ~(dim==4 || dim==6)
    error('输入初始状态必须为4维或6维矢量');
end
t = [0:h:T];
if t(end)~=T
    t = [t T];
end
len = length(t);
traj = nan(dim,len);
if m==1
    x0 = x0'; %转成列矢量
end
for i=1:len
    if dim==4
        phi = cwstm4(w,t(i));
    else
        phi = cwstm6(w,t(i));
    end
    traj(:,i) = phi*x0;
end
traj = traj';
t = t';
