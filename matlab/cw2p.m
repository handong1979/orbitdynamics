% dv = cw2p(x0,xf,T,w0)
% CW�������Ƶ��������һ��������
% x0:��ʼ���λ���ٶ�
% xf:Ŀ�����λ���ٶ�
% T:ת��ʱ��
% w0:������ٶ�
% Example:
% 1. �Ӹ��ɽ����Ʒɹ��:
%    x0 = [-10;0;0;0;0;0];
%    xf = [10;0;0;0;0;0];
%    w0 = We; T = pi/We;
%    [dv1,~] = cw2p(x0,xf,T,w0)
% 2. ͣ����ת��:
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