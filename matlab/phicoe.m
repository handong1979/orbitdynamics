% ���������״̬ת�ƾ���
% phi = phicoe(kp0,step,mode)
% kp0 ��ʼ������
% step ����
% mode ģ������,�ַ��ͣ�������ѡ��:
%      '2body'  ����ģ��
%      'J2'     J2ģ�ͣ�������
%      'full'   �����㶯ģ�ͣ�������
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