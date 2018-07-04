<<<<<<< HEAD
% ÈÆYÖáÐý×ª¾ØÕó
function R = roty(angle)
if isscalar(angle)
    R = [cos(angle)  0    -sin(angle);
              0      1         0;
         sin(angle)  0     cos(angle)];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [cos(angle)    zeros(1,1,n)    -sin(angle);
         zeros(1,1,n)  ones(1,1,n)      zeros(1,1,n);
         sin(angle)    zeros(1,1,n)     cos(angle)];
else
    error('ÊäÈë±ØÐëÎª±êÁ¿»òÊ¸Á¿');
end
=======
% ç»•Yè½´æ—‹è½¬çŸ©é˜µ
function R = roty(angle)
if isscalar(angle)
    R = [cos(angle)  0    -sin(angle);
              0      1         0;
         sin(angle)  0     cos(angle)];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [cos(angle)    zeros(1,1,n)    -sin(angle);
         zeros(1,1,n)  ones(1,1,n)      zeros(1,1,n);
         sin(angle)    zeros(1,1,n)     cos(angle)];
else
    error('è¾“å…¥å¿…é¡»ä¸ºæ ‡é‡æˆ–çŸ¢é‡');
end
>>>>>>> 943c4faedec0557bb03b6d917afd83996852ce2a
