% ���ٶ���������ȼ������
% dm = dvdm(dv,m0,I)
% ���ݻ�����̼��㣺dm = m0.*(1-exp(-dv/I/9.8));
% dv: �ٶ�����m/s
% m0: ��ʼ������kg
% I: �ȳ�s
function dm = dvdm(dv,m0,I)
dm = m0.*(1-exp(-abs(dv)/I/9.8));