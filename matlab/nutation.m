% �¶��������
%   nutation(year,month,day,hour,minute,second)����UTC����ʱʱ�̵��¶�����
%   nutation(mjd)�����Լ������mjdʱ�̵��¶�����
%
% �¶����� nutation matrix
%   ����һ��ʸ����˲ʱƽ�������ϵ�µ�����Ϊr������˲ʱ��������ϵ�е�����Ϊ Nutation*r
%   ��֮��һ��ʸ����˲ʱ��������ϵ�µ�����Ϊr������˲ʱƽ�������ϵ�е�����Ϊ Nutation'*r
%   �����¶���u1,u2,u3��ת��ΪNutation = Rz(-(eps+deps))*Ry(dksi)*Rx(eps)
% 
% Example:
%      nutation(2006,1,1,0,0,0);
%      nutation(53736)
%
% See also nutation_angle, precession_angle, ecliptic_equator_angle, precession.
function nu = nutation(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = utc2tdt(Y);
else
    error('���������Ŀ����');
end
eps = ecliptic_equator_angle(TDT);
[dksi,deps] = nutation_angle(TDT);
nu = rotx(-eps-deps)*rotz(-dksi)*rotx(eps);