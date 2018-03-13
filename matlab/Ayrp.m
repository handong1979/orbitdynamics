% 姿态角==>方向余弦阵
function A = Ayrp(b1,b2,b3)
error(nargchk(3,3,nargin));
c1 = cos(b1);
c2 = cos(b2);
c3 = cos(b3);
s1 = sin(b1);
s2 = sin(b2);
s3 = sin(b3);
A =[ c2*c3 - s1*s2*s3,     c2*s3 + s1*s2*c3,          -c1*s2;
    -c1*s3,                c1*c3,                      s1;
    s2*c3 + s1*c2*s3,      s2*s3 - s1*c2*c3,           c1*c2];