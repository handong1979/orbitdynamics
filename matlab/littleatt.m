% п║╫г╤хвкл╛╬ьуС--╥Ш╨етккЦ
function c = littleatt()
phi = sym('phi');
theta = sym('theta');
psi = sym('psi');
c = [1,psi,-theta;...
    -psi,1,phi;...
    theta,-phi,1];