% UTCʱת��ΪUT1ʱ
% utc2ut1(mjd) ��Э������ʱmjd(��Լ�����ձ�ʾ)ת��Ϊ����ʱ1(��Լ�����ձ�ʾ)
% Example:
%     utc2ut1(53736)
% See also: utc2tdt, date2mjd
function UT1 = utc2ut1(UTC)
persistent eop
% �ڳ����һ�����е�����ʱ,eop=[]������load eop.mat
% ����eop��ȫ�ֱ��������ٴε��ôκ���ʱ��eop����ֵ,�������ٴ�load
% �����eop��polewander�й���һ����Ҳ��ֻ��Ҫloadһ�μ���
% ��������clear all�Ժ�ȫ�ֱ���eop�ű����
if isempty(eop)
    load eop.mat;
end
if UTC > eop(length(eop),1) || UTC<eop(1,1)
    % ʱ�䳬������ʱ�䷶Χ
    dut1 = 0.0;
else
    % ��ֵ�õ�UTCʱ��dut1
    dut1 = interp1(eop(:,1),eop(:,4),UTC,'pchip');
end
UT1 = UTC + dut1/86400;
