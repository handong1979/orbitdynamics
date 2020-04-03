% 重力梯度力矩
function Tg = gravitygrad(ic,r,Is)
Tg = 3*GEarth/r^3*cross(ic,Is*ic);