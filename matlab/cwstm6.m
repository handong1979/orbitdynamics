% CW����״̬ת�ƾ���
% phi = cwstm6(w,t)
%    phi��һ��״̬ת�ƾ���
%     w: ������ٶ�,��λ������/�� w=sqrt(miu/r^3)
%     t: ����,��λ���� w=sqrt(miu/r^3)
%
% Example:
%    PHI = cwstm6(sqrt(GEarth/6978^3),10);
%
% See Also: cwstm4, cwtraj, cw2p
%
% ����������ʱ����������£�
% t1ʱ�̱��ǰ: x0 = [r; v]
% t1ʱ�̱���x0 = [r; v+dv]
% t2ʱ�̣�      x = phi*[r; v+dv]
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