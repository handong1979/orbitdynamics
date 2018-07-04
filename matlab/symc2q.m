% 方向余弦阵-->四元数
%  本函数适用于符号运算，而c2q函数考虑奇点的处理不适合符号运算
%  See also c2q, q2c.
function q = symc2q(c)
trc1 = c(1,1) + c(2,2) + c(3,3) + 1.0;
qs = sqrt(trc1)/2.0;
qx = (c(2,3) - c(3,2))/4.0/qs;
qy = (c(3,1) - c(1,3))/4.0/qs;
qz = (c(1,2) - c(2,1))/4.0/qs;
q = [qx;qy;qz;qs];