% ���Ƕ����Ӽ�pi*2���0������ֵ
function y = limitpi(x)
% while any(x>pi)
    flag = x>pi;
    x = x - pi*2*flag;
% end
% while any(x<-pi)
    flag = x<-pi;
    y = x + pi*2*flag;
% end