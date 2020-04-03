% 东南坐标系相对惯性系的旋转矩阵
% cdni = getcdni(mjd,rv)
% 输入： mjd: MJD时间,UTC时
%        rv:  J2000.0惯性系位置
% 输出: cdni：东南系相对惯性系的转换矩阵
function cdni = getcdni(mjd,rv)
% 输入J2000.0轨道参数的算法
cfi = eci2ecf(mjd);
rv = rv(:);
rf = cfi*rv(1:3);
xfyf2 = sqrt(rf(1)^2+rf(2)^2);
rr = norm(rf);
cdni = [-rf(2)/xfyf2,rf(1)/xfyf2,0;...
    rf(1)*rf(3)/rr/xfyf2,rf(2)*rf(3)/rr/xfyf2,-xfyf2/rr;...
    -rf(1)/rr,-rf(2)/rr,-rf(3)/rr]*cfi;