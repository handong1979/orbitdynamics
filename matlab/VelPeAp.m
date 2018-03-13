% 由近地点半径、远地点半径计算近地点速度和远地点速度
% [vp va] = VelPeAp(rp,ra,CB)
%  rp：近地点半径
%  ra：远地点半径
%  CB：中心天体引力常数
%  vp：近地点速度
%  va：远地点速度
function [vp va] = VelPeAp(rp,ra,CB)
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
    error('输入中心天体不对，地球为e，月球为m');
end
a = (rp+ra)/2;
vp = sqrt(miu*(2/rp-1/a));
va = sqrt(miu*(2/ra-1/a));
if nargout < 2
    vp = [vp,va];
end