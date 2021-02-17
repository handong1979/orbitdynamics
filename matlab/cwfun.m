% CW方程的加速度
function rdd = cwfun(w,r,v)
if isvector(r) && isvector(v)
	rdd(1) = 2*w*v(3);
	rdd(2) = -w*w*r(2);
	rdd(3) = 2*w*v(1) - 3*w*w*r(3);
elseif ismatrix(r) && ismatrix(v)
	[m,n] = size(r);
	rdd = nan(m,n);
	rdd(:,1) = 2*w*v(:,3);
	rdd(:,2) = -w*w*r(:,2);
	rdd(:,3) = 2*w*v(:,1) - 3*w*w*r(:,3);
end