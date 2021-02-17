%  ����TH-Y���̼�����Թ켣
function [traj,ts] = THtraj(tm,x0,t)
if nargin == 0
    warning('self test');
    tm = krad([24671,0.71,28,0,180,180]);
    x0 = [0;0;0;0.0001;0;0];
    [traj,ts] = THtraj(tm,x0,3600*30);
    figure,plot6(ts,traj);
    figure
    subplot 211,plot(traj(:,1),traj(:,3)),xlabel('x(km)'),ylabel('z(km)');
    subplot 212,plot(traj(:,1),traj(:,2)),xlabel('x(km)'),ylabel('y(km)');
    return
end

% �������
p = 2*pi/sqrt(GEarth/tm(1)^3);
% ȷ���켣����
if t <= p
   step = t/180;
else
   step = p/180;
end
% ʱ������
ts = 0:step:t;
% �����ڴ�
n = length(ts);
traj = nan(6,n);
% ѭ������
theta0 = ma2ta(tm(2),tm(6));
e = tm(2);
x0 = x0(:);
w = sqrt(GEarth/tm(1)^3);
for k=1:n
    thetaf = ma2ta(tm(2),tm(6) + w*(k-1)*step);
    T = THstm((k-1)*step,0,theta0,thetaf,tm(1),e);
    traj(:,k) = T*x0;
end
traj = traj';