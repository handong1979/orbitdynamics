% 将Excel表格的列号转换为数字
function n = xlsn2num(x)
if ischar(x)
    n = length(x);
    if n > 2
        error('只能处理小于等于2个字符的列号');
    end
else
    error('输入必须是字符型');
end
n = sum((x - 'A' + 1).*[26 1]);