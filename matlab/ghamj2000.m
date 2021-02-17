% ���J2000ƽ���ֵ�ĸ�������ƽ����ʱ(Greenwich hour angle)
% "���J2000ƽ���ֵ�ĸ�������ƽ����ʱ"��û��ȷ�е����������壬
%  ��Ϊ���ֲ�֮ǰ���ǹ�������е�ȱ�ݶ��ճ�������,�����ʹ��
% ghamj2000(year,month,day,hour,minute,second)
%        ����UTC���ڵ����J2000ƽ���ֵ�ĸ�������ƽ����ʱ(rad)
% ghamj2000(mjd)����UTC(MJD��ʾ)ʱ�̵����J2000ƽ���ֵ�ĸ�������ƽ����ʱ(rad)
% Example:
%    ghamj2000(2007,7,1,12,0,0)
%    ghamj2000(54282.5)
% See also: gham, gha, date2mjd
function SG = ghamj2000(Y,M,D,h,m,s)
warning('"���J2000ƽ���ֵ�ĸ�������ƽ����ʱ"��û��ȷ�е����������壬');
warning('��Ϊ���ֲ�֮ǰ���ǹ�������е�ȱ�ݶ��ճ�������,�����ʹ��');
if nargin == 6
    UTC  = date2mjd(Y,M,D,h,m,s);
elseif nargin ==1 
    UTC = Y;
else
    error('���������Ŀ����');
end
dt = utc2ut1(UTC) - mjd2000;
[zeta theta Z] = precession_angle(UTC);
SG = mod((280.4606184 + 360.985647365*dt + 0.2908e-12*dt^2)*rad - zeta - Z,pi*2);