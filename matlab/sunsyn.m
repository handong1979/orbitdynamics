% ̫��ͬ�������ǡ��߶ȵļ���
% function y = sunsyn(x)
% if x<pi          xΪ���(����)
% elseif x<=180    xΪ���(��)
% elseif x<6378    xΪ����߶�(km)
% else             xΪ����볤��

function y = sunsyn(x)
if x<pi % xΪ���(����)
    y = (-0.9856/9.97/Re^3.5/cos(i))^(-2/7);
elseif x<=180 % xΪ���(��)
    y = (-0.9856/9.97/Re^3.5/cos(x*rad))^(-2/7);
elseif x<6378 % xΪ����߶�(km)
    y = acos( -0.9856/9.97/((x+Re)/Re)^3.5 ) * deg;
else % xΪ����볤��
    y = acos( -0.9856/9.97/(x/Re)^3.5 ) * deg;
end