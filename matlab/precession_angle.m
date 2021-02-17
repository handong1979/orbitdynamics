% ���Ǽ���
% [zeta theta Z] = precession_angle(year,month,day,hour,minute,second)
%         ����UTC����ʱʱ�̵�����zeta,theta,Z
% [zeta theta Z] = precession_angle(mjd)
%         �����Լ������mjdʱ�̵�����zeta,theta,Z
% zeta+Z Ϊ�ྭ��� theta  Ϊ��γ���
%
% Example:
%    [zeta theta Z] = precession_angle(2006,1,1,0,0,0)
%    [zeta theta Z] = precession_angle(53736)
%
% See also precession, nutation, nutation_angle.
function [zeta theta Z] = precession_angle(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('���������Ŀ����');
end
dt = (TDT-mjd2000)/36525.0;
% 206264.806247096 = 3600*DEG
zeta  = dt*(2306.2181 + dt*(0.30188 + 0.017998*dt))/206264.806247096;
theta = dt*(2004.3109 - dt*(0.42665 + 0.041833*dt))/206264.806247096;
Z     = dt*(2306.2181 + dt*(1.09468 + 0.018203*dt))/206264.806247096;