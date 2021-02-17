% CW方程状态转移矩阵
% phi = cwstm6(w,t)
%    phi：一步状态转移矩阵
%     w: 轨道角速度,单位：弧度/秒 w=sqrt(miu/r^3)
%     t: 步长,单位：秒 w=sqrt(miu/r^3)
%
% Example:
%    PHI = cwstm6(sqrt(GEarth/6978^3),10);
%
% See Also: cwstm4, cwtraj, cw2p
%
% 用于脉冲变轨时计算过程如下：
% t1时刻变轨前: x0 = [r; v]
% t1时刻变轨后：x0 = [r; v+dv]
% t2时刻：      x = phi*[r; v+dv]
function phi = cwstm6(w,t)
wt = w*t;
s = sin(wt);
c = cos(wt);
phi = [1   0   6*(wt-s)   4*s/w-3*t   0    2/w*(1-c); ...
       0   c    0             0       s/w      0;...
       0   0   4-3*c      -2/w*(1-c)   0      s/w;...
       0   0   6*w*(1-c)   4*c-3       0      2*s;...
       0  -s*w   0           0         c       0;...
       0   0   3*w*s        -2*s        0      c];