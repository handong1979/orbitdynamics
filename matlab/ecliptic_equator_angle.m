% �Ƴཻ��
% ecliptic_equator_angle(year,month,day,hour,minute,second)
%         ����UTC����ʱʱ�̵ĻƳཻ��
% ecliptic_equator_angle(mjd)
%         �����Լ������mjdʱ�̵ĻƳཻ��
% 
% Example: 
%    ecliptic_equator_angle(2006,1,1,0,0,0)
%    ecliptic_equator_angle(53736)
%
% See alse: date2mjd
function e = ecliptic_equator_angle(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('���������Ŀ����');
end
T = (TDT-mjd2000)/36525.0;
e = (23.4392911 - 0.013004167*T - 1.64e-7*T*T ) * rad;