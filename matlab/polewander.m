% �����ƾ���
% polewander(year,month,day,hour,minute,second)����UTCʱ�̵ļ��ƾ���
% polewander(UTC)����UTC(��Լ�����ձ�ʾ)ʱ�̵ļ��ƾ���
% 
% �����ƾ���Ϊ��
% [1,   0,   Xp]
% [0,   1,  -Yp]
% [Xp,  Yp,   1]
%   ����һ��ʸ����׼�ع�����ϵ�µ�����Ϊr�����ڵع�����ϵ�е�����Ϊ PoleWander*r
%   ��֮,һ��ʸ���ڵع�����ϵ�µ�����Ϊr������׼�ع�����ϵ�е�����Ϊ PoleWander'*r
% 
% Example:
%    polewander(year,month,day,hour,minute,second)
%    polewander(mjd)
%  
% See also:  precession, nutation
function pw = polewander(Y,M,D,h,m,s)
if nargin == 6
    UTC = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1
    UTC = Y;
else
    error('���������Ŀ����');
end
% �ڳ����һ�����е�����ʱ,eop=[]������load eop.mat
% ����eop��persistent���������ٴε��ôκ���ʱ��eop����ֵ,�������ٴ�load
persistent eop;
if isempty(eop)
    load eop.mat;
end

if (UTC > eop(length(eop),1)) || (UTC<eop(1,1))
    % ʱ�䳬������ʱ�䷶Χ
    Xp = 0.0;
    Yp = 0.0;
else
    % ��ֵ�õ�UTCʱ��Xp,Yp
    Xp = interp1(eop(:,1),eop(:,2),UTC)/3600*rad;
    Yp = interp1(eop(:,1),eop(:,3),UTC)/3600*rad;
end

pw = [1,   0,   Xp;...
      0,   1,  -Yp;...
      Xp,  Yp,   1];
