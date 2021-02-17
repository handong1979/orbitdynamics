% �ɽ��ص�뾶��Զ�ص�뾶������ص��ٶȺ�Զ�ص��ٶ�
% [vp va] = VelPeAp(rp,ra,CB)
%  rp�����ص�뾶
%  ra��Զ�ص�뾶
%  CB������������������
%  vp�����ص��ٶ�
%  va��Զ�ص��ٶ�
function [vp,va] = VelPeAp(rp,ra,CB)
if nargin == 2
    CB = 'e';
end
if CB == 'e'
    miu = 398600.4418;
    R = 6378.14;
elseif CB == 'm'
    miu = 4902.8;
    R = 1738;
else
    error('�����������岻�ԣ�����Ϊe������Ϊm');
end
a = (rp+ra)/2;
vp = sqrt(miu*(2/rp-1/a));
va = sqrt(miu*(2/ra-1/a));
if nargout < 2
    vp = [vp,va];
end