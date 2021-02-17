% �����ٶ��������㿪��ʱ���ȼ������
% [T,dm] = fireduration(dv,F,mass0,Isp)
%  dv: �ٶ�����,��λm/s
%   F: ��������������λN
%  mass0:��ʼ��������λkg
%  Isp���������ȳ壬��λs
%  �����
%     T: ����ʱ������λs
%     dm: ȼ�����ģ���λkg
function [T,dm] = fireduration(dv,F,mass0,Isp)
mdot = F/Isp/9.8;
dm = mass0*(1-exp(-dv/Isp/9.8));
T = dm/mdot;