%�жϵ�Ӱ����
%umbra(spacecraft,position_sun,body,r_body)�жϺ������Ƿ������ڵ�������Ӱ��
%
%���룺
%spacecraft��������λ�ã��л���������
%position_sun��̫��λ�ã��л���������
%body���ڵ�����λ�ã��л���������
%r_body���ڵ�����뾶��
%���ϲ�����λ����һ�¡�
%
%�����
%    1Ϊ�������ڹ�����
%    0Ϊ����������Ӱ��

function [y,k]= umbra(s) %spacecraft,position_sun,body,r_body

rs = de421(s(1),'Earth','Sun');
rs = rs(1:3)';
r = s(8:10)';
dr = r - rs;
rm = norm(r,2);
drm = norm(dr,2);
thetaES = acos( r'*dr/rm/drm );  % ����-����-̫���н�
as = asin(695990.0/drm);  % �����Ͽ�̫�����Ӱ뾶
ae = asin(Re/rm);  % �����Ͽ�������Ӱ뾶
if thetaES <= ae-as
    k = 0.0; % ��Ӱ������ȫʳ
elseif thetaES < ae+as
    if as-thetaES>=ae
        % �����̫���Ӱ뾶С���ջ�ʳ
        k = 1.0 - ae*ae/as/as;
    else
        % ̫�����������̫��Բ����ı�ֵ����ƫʳ
        a = acos( (ae*ae+thetaES*thetaES-as*as)/(2.0*ae*thetaES) );
        b = acos( (as*as+thetaES*thetaES-ae*ae)/(2.0*as*thetaES) );
        k = 1.0 - ( as*as*(b-sin(b)*cos(b)) + ae*ae*(a-sin(a)*cos(a)) )/as/as/pi;  
    end		
else
    k = 1.0; % ������
end
% ��ֵ������
if k > 0.9
    y = 1;
else
    y = 0;
end


% a=reshape(position_sun(1:3),3,1);
% b=reshape(body(1:3),3,1);
% c=reshape(s(8:10),3,1);
% 
% ra=norm(a-b,2);
% rb=norm(b-c,2);
% rc=norm(c-a,2);
% 
% alpha=acos((ra^2+rb^2-rc^2)/2/ra/rb);
% h=rb*sin(alpha);
% 
% if alpha>pi/2 && h<r_body
%     y=0;%����������Ӱ��
% else
%     y=1;%�������ڹ�����
% end

