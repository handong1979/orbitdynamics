% index = find_stairs(x)
% 查找矢量中发生跳变的点，通常用于查找模式字发生变化的点
% x(id) 对应模式切换的第一个点
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