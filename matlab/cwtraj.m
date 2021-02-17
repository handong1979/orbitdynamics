% ���ݳ�ʼ״̬����CW�켣
% [traj,T] = cwtraj(x0,w,T,h)
%    x0: ��ʼʱ��λ���ٶȣ�������4ά��6άʸ�����������4άʸ������Ϊ��
%        ���ƽ���ڵ�λ���ٶȣ����trajҲ��4άʸ����ɵĹ켣���������6άʸ����
%        ����Ϊ����άλ�ú���ά�ٶȣ����trajҲ��6άʸ����ɵĹ켣��
%        λ���ٶ�ʸ���ĳ��ȵ�λ������mҲ������km������������뵥λ��ͬ
%     w: ������ٶ�,��λ������/�� w=sqrt(miu/r^3)
%     T: �켣ʱ�䳤�ȣ���λ����
%     h: �켣����������������λ����
% Example��
%  1. GEO����Ʒ�
%  ag = 42164.2;
%  b = 35; % �Ʒɰ����
%  kt = [ag,0,0,0,0,0];
%  kc = [ag,b/ag,0,0,0,0];
%  rv = tcr(kt,kc);
%  traj = cwtraj(rv,We,86400,60);
% See Also: cwstm4, csstm6, cw2p
function [traj,t] = cwtraj(x0,w,T,h)
if ~isvector(x0)
    error('�����ʼ״̬x0����Ϊʸ��');
end
[m,n] = size(x0);
dim = max(m,n);
if ~(dim==4 || dim==6)
    error('�����ʼ״̬����Ϊ4ά��6άʸ��');
end
t = [0:h:T];
if t(end)~=T
    t = [t T];
end
len = length(t);
traj = nan(dim,len);
if m==1
    x0 = x0'; %ת����ʸ��
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
