% ���޷���0��2pi�ķ��ǣ����ڴ�2pi���䵽0�ĵ㣩������һ�������仯�ķ���
function u = lineargument(u)
if ~isvector(u)
    error('input must be a vector');
end
% ������λ��
jumppoint = find(abs(diff(u))>0.1)+1;
% �����ĸ���
njpoint = length(jumppoint);
% �Ӻ�ǰ�����μ���2pi
for i=njpoint:-1:1
    u(jumppoint(i):end) = u(jumppoint(i):end) + 2*pi;
end