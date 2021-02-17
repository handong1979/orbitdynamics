% ����ϵ���ع�ϵת������
%   eci2ecf(year,month,day,hour,minute,second)����UTC����ʱʱ�̵Ĺ���ϵ���ع�ϵת������
%   eci2ecf(mjd)�����Լ������mjdʱ�̵Ĺ���ϵ���ع�ϵת������
%
% Example:
%      eci2ecf(2006,1,1,0,0,0)
%      eci2ecf(53736)
%
% See also precession, nutation, gha, polewander
function c = eci2ecf(Y,M,D,h,m,s)
if nargin == 6
    UTC = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1
    UTC = Y;
else
    error('���������Ŀ����');
end
c = polewander(UTC)*rotz(gha(UTC))*nutation(UTC)*precession(UTC);