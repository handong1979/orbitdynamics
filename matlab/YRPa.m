% 方向余弦阵==>姿态角
function a = YRPa(b)
if abs(b(2,3))>1
    a(1) = asin(b(2,3)/abs(b(2,3)));
else
    a(1) = asin(b(2,3));
end
if b(3,3)==0
    a(2) = -sign(b(1,3))*pi/2;
else
    a(2) = atan(-b(1,3)/b(3,3));
    if b(3,3)<0
        if b(1,3)>0
            a(2) = a(2) - pi;
        else
            a(2) = a(2) + pi;
        end
    end
end
if b(2,2) == 0
    a(3) = -sign(b(2,1))*pi/2;
else
    a(3) = atan(-b(2,1)/b(2,2));
    if b(2,2)<0
        if b(2,1)>0
            a(3) = a(3) - pi;
        else
            a(3) = a(3) + pi;
        end
    end
end
a = a';