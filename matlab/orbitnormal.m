% ������߷���λʸ��
function n = orbitnormal(i,Omega)
i = i(:)';
Omega = Omega(:)';
n = [sin(i).*sin(Omega);...
    -sin(i).*cos(Omega);...
    cos(i)];