% 方向余弦阵＝＝》四元数
function q = c2q(c)
narginchk(1,1);
trc1 = c(1,1) + c(2,2) + c(3,3) + 1.0;
if trc1 >= 0.004 
	qs = sqrt(trc1)/2.0;
	qx = (c(2,3) - c(3,2))/4.0/qs;
	qy = (c(3,1) - c(1,3))/4.0/qs;
	qz = (c(1,2) - c(2,1))/4.0/qs;
elseif  1.0 - c(1,1) + c(2,2) - c(3,3) >= 0.004
	qy = sqrt(1.0 - c(1,1) + c(2,2) - c(3,3)) / 2.0 * mysign(c(3,1)-c(1,3));
	qx = (c(2,1) + c(1,2))/4.0/qy;
	qz = (c(3,2) + c(2,3))/4.0/qy;
	qs = (c(3,1) - c(1,3))/4.0/qy;
elseif  1.0 + c(1,1) - c(2,2) - c(3,3) >= 0.004 
	qx = sqrt(1.0 + c(1,1) - c(2,2) - c(3,3)) / 2.0 * mysign(c(2,3)-c(3,2));
	qy = (c(2,1) + c(1,2))/4.0/qx;
	qz = (c(1,3) + c(3,1))/4.0/qx;
	qs = (c(2,3) - c(3,2))/4.0/qx;
else
	qz = sqrt(1.0 - c(1,1) - c(2,2) + c(3,3)) / 2.0 * mysign(c(1,2)-c(2,1));
	qx = (c(1,3) + c(3,1))/4.0/qz;
	qy = (c(2,3) + c(3,2))/4.0/qz;
	qs = (c(1,2) - c(2,1))/4.0/qz;
end
q = [qx;qy;qz;qs];
q = q/norm(q);


function y = mysign(x)
if x>=0
    y = 1;
else
    y = -1;
end