% ����ת��
% [dv1,dv2] = hohmanntransfer(h1,h2)
% a1 ��ʼԲ����볤��(km)
% a2 Ŀ��԰����볤��(km)
% dv1 ��һ�α���ٶ�����
% dv2 �ڶ��α���ٶ�����
function [dv1,dv2,eta0] = hohmanntransfer(a1,a2)
if a1<Re
    a1 = Re + a1;
end
if a2<Re
    a2 = Re + a2;
end
at = (a1 + a2) / 2;
dv1 = ( sqrt(GEarth*(2/a1-1/at)) - sqrt(GEarth/a1) )*1000;
dv2 = ( sqrt(GEarth/a2) - sqrt(GEarth*(2/a2-1/at)) )*1000;
% dv = abs(dv1)+abs(dv2);
eta0 = pi*(1-sqrt(((a1+a2)/2/a2)^3));
