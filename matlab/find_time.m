% index = find_time(t,st)
% ��ʱ���б�t�У�����st��ʱ���ֶΣ�������Ӧt�ķֶε��±�
% ���t�иպú���st�еĸ���ʱ��㣬���� t(index) == st
% Example:
% t = 1:0.1:1000; % �±�պõ��ڶ�Ӧ����ֵ
% st = 38:200:800;
% id = find_time(t,st)
% id =
%          371
%         2371
%         4371
%         6371
% disp([t(id);st])
%     38   238   438   638
%     38   238   438   638
% See also:
%     find_stairs
function index = find_time(t,st)
    n = length(st);
    index = ones(n,1);
    for i=1:n
        id = find(t >= st(i));
        if isempty(id)
            index = index(1:i-1);
            break;
        else
            index(i) = id(1);
        end
    end
end