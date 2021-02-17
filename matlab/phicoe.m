% 轨道六根数状态转移矩阵
% phi = phicoe(kp0,step,mode)
% kp0 初始六根数
% step 步长
% mode 模型设置,字符型，有以下选择:
%      '2body'  二体模型
%      'J2'     J2模型，待补充
%      'full'   完整摄动模型，待补充
function phi = phicoe(kp0,step,mode)
if nargin < 3
	mode = '2body';
end
switch mode
case '2body'
	phi = eye(6,6);
	phi(6,1) = -1.5*sqrt(GEarth/kp0(1)^5)*step;
case 'J2'
	error('todo');
end