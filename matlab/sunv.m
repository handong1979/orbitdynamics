% ¼ÆËãÌ«ÑôÊ¸Á¿
function si = sunv(mjd)
sun = de421(mjd,'Earth','Sun');
si = sun(1:3)'/norm(sun(1:3));