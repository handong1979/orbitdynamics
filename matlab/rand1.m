% -1~+1之间均匀分布的随机数
function n = rand1(varargin)
n = rand(varargin{:})*2-1;