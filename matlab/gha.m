% �������������ʱ(Greenwich hour angle)
% gha(year,month,day,hour,minute,second)����UTC���ڵĸ������������ʱ(rad)
% gha(mjd)����UTC(MJD��ʾ)ʱ�̵ĸ������������ʱ(rad)
% Example:
%    gha(2007,7,1,12,0,0)
%    gha(54282.5)
% See also: gham, ghamj2000, date2mjd
function SG = gha(Y,M,D,h,m,s)
if nargin == 6
    UTC  = date2mjd(Y,M,D,h,m,s);
elseif nargin ==1 
    UTC = Y;
else
    warning('self test');
    mjd0 = date2mjd([2015,1,1,12,0,0]);
    t = 0:1:365*30;
    for i=1:length(t)
        g0 = gha(mjd0+t(i));
        e01(i) = limitpi(mexsofa('GST06',mjd0+t(i))-g0)*deg*3600;
        e02(i) = limitpi(mexsofa('GST00A',mjd0+t(i))-g0)*deg*3600;
        e03(i) = limitpi(mexsofa('GST94',mjd0+t(i))-g0)*deg*3600;
    end
    t = t/365+15; % �������
    figure,plot(t,e01,t,e02,t,e03),grid on;
    xlabel('t(��)'),ylabel('err(����)'),legend('GST06-gha','GST00A-gha','GST94-gha');
end
dksi = nutation_angle(UTC);
eps = ecliptic_equator_angle(UTC);
SG = mod(dksi*cos(eps)+gham(UTC),pi*2);
