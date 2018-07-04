<<<<<<< HEAD
% ÈÆZÖáÐý×ª¾ØÕó
function R = rotz(angle)
if isscalar(angle)
    R = [ cos(angle)  sin(angle)  0;
         -sin(angle)  cos(angle)  0;
              0            0      1];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [ cos(angle)    sin(angle)     zeros(1,1,n);
         -sin(angle)    cos(angle)     zeros(1,1,n);
          zeros(1,1,n)  zeros(1,1,n)   ones(1,1,n)];
else
    error('ÊäÈë±ØÐëÎª±êÁ¿»òÊ¸Á¿');
end
=======
% ç»•Zè½´æ—‹è½¬çŸ©é˜µ
function R = rotz(angle)
if isscalar(angle)
    R = [ cos(angle)  sin(angle)  0;
         -sin(angle)  cos(angle)  0;
              0            0      1];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [ cos(angle)    sin(angle)     zeros(1,1,n);
         -sin(angle)    cos(angle)     zeros(1,1,n);
          zeros(1,1,n)  zeros(1,1,n)   ones(1,1,n)];
else
    error('è¾“å…¥å¿…é¡»ä¸ºæ ‡é‡æˆ–çŸ¢é‡');
end
>>>>>>> 943c4faedec0557bb03b6d917afd83996852ce2a
