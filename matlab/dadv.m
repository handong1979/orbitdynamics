% �ٶ���������İ볤���С���仯��
% da = dadv(a,dv)
% dv:�ٶ���������λm/s
% a:��ʼ�볤�ᣬ��λkm
% da:�볤��仯������λkm
% Example:
%     da = dadv(7000,1)
%     da = dadv(42164,1)
% See also:
%    dvda dnda duda drda
function da = dadv(a,dv)
n = sqrt(GEarth/a^3);
da = 2/n*dv/1000;