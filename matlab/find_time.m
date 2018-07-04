% index = find_time(t,st)
% 从时间列表t中，按照st的时间点分段，给出对应t的分段的下标
% 如果t中刚好含有st中的各个时间点，则有 t(index) == st
% Example:
% t = 1:0.1:1000; % 下标刚好等于对应的数值
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