% ��Excel�����к�ת��Ϊ����
function n = xlsn2num(x)
if ischar(x)
    n = length(x);
    if n > 2
        error('ֻ�ܴ���С�ڵ���2���ַ����к�');
    end
else
    error('����������ַ���');
end
n = sum((x - 'A' + 1).*[26 1]);