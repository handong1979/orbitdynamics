% index = find_stairs(x)
% ����ʸ���з�������ĵ㣬ͨ�����ڲ���ģʽ�ַ����仯�ĵ�
% x(id) ��Ӧģʽ�л��ĵ�һ����
% Example:
% x = [1 1 1 2 2 2 2 3 3 3 3 3 4 5];
% id = find_stairs(x)
% id =
%      4     8    13    14
% disp(x(id))
%     2     3     4     5
% See also:
%    find_time
function index = find_stairs(x)
index = find(x(2:end) - x(1:end-1)) + 1;