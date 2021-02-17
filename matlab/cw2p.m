% dv = cw2p(x0,xf,T,w0)
% CW两脉冲制导，计算第一个脉冲量
% x0:初始相对位置速度
% xf:目标相对位置速度
% T:转移时间
% w0:轨道角速度
% Example:
% 1. 从跟飞建立绕飞轨道:
%    x0 = [-10;0;0;0;0;0];
%    xf = [10;0;0;0;0;0];
%    w0 = We; T = pi/We;
%    [dv1,~] = cw2p(x0,xf,T,w0)
% 2. 停泊点转移:
%    kc = [42114,0,0,0,0,0]; kt = [42164,0,0,0,0,0];
%    x0 = tcr(kt,kc);
%    xf = [-10;0;0;0;0;0];
%    w0 = We; T = 0.5*pi/We;
%    [dv1,dv2,dvall] = cw2p(x0,xf,T,w0)
function [dv1,dv2,dvall] = cw2p(x0,xf,T,w0)
phi = cwstm6(w0,T);
A = phi(1:3,1:3);
B = phi(1:3,4:6);
dv1 = B\(xf(1:3) - A*x0(1:3)) - x0(4:6);
if nargout > 1
    xfb = phi*(x0+[0;0;0;dv1]);
    dv2 = xf(4:6)-xfb(4:6);
end
if nargout > 2
    dvall = sum(abs(dv1)+abs(dv2));
end