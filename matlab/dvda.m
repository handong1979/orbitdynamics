% �볤���С���仯�����ٶ�����
% dv = dvda(a,da)
% dv:�ٶ���������λm/s
% a:��ʼ�볤�ᣬ��λkm
% da:�볤��仯������λkm
% Example:
%     dv = dvda(7000,1)
%     dv = dvda(42164,1)
% See also:
%    dadv dnda duda drda
function dv = dvda(a,da)
n = sqrt(GEarth/a^3);
dv = n/2*da*1000;