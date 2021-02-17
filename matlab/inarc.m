% 判断角度a是否在a1-a2之间的弧段内
function b = inarc(a,a1,a2)
if nargin == 0 % self test
    a1 = -85;
    a2 = -113;
    cir = -180:1:180;
%     bin = logical(length(cir),1);
    for i = 1:length(cir)
        bin(i) = inarc(cir(i),a1,a2);
    end
    arcin = cir(bin);
    figure,plot(arcin,zeros(1,length(arcin)),'.'),hold on;plot(a1,0,'ro'),plot(a2,0,'rx');
    xlim([-180,180]);
    return
end

a  = mod180(a);
a1 = mod180(a1);
a2 = mod180(a2);
if a2 > a1
    if a>a1 && a<a2
        b = logical(1);
    else
        b = logical(0);
    end
elseif a2 < a1
    if a < a2
        a = a + 360;
    end
    a2 = a2 + 360;
    if a>a1 && a<a2
        b = logical(1);
    else
        b = logical(0);
    end
else % a1=a2
    b = logical(0);
end
% if nargout > 1
%     % 返回一个1*361的逻辑数组，表示该弧段的角度范围是位于0~360的那个弧段，用于画图
% end
    

function b = mod180(a)
b = mod(a,360);
if b > 180
    b = b - 360;
end