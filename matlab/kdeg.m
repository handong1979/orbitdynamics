% ����������еĻ���ת��Ϊ��
function k = kdeg(ki)
[m n] = size(ki);
if n~=6
   error('�������Ϊn*6�ľ���'); 
end
k = ki;
k(:,3:6) = k(:,3:6)*deg;