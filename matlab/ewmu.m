function u = ewmu(e,w,m)
% ��e,w��M������γ�ȷ���u
f = ma2ta(e,m);
u = mod(w+f,2*pi);