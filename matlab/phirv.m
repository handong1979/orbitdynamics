% 位置速度轨道动力方程的状态转移矩阵
% kp0 初始六根数
% step 步长
% mode 模型设置,字符型，有以下选择:
%      '2body'  二体模型
%      'J2'     J2模型
%      'full'   完整摄动模型，待补充

function phi = phirv(rv,step,mode)
if nargin == 0
    warning('self test');
    kt = [42166.258681, 0.0001,   0.070747,  88.977921,  0.0, 38.920273];
    rv = kepler2cart(krad(kt));
    P = diag([1,1,1,0.0001,0.0001,0.0001].^2)/1.2e2;
    [xx,yy,zz] = sphere(20);
    for kk = 1:1440*1
        phi = phirv(rv,60,'J2');
        P = phi*P*phi';
        if mod(kk,120)==0
            [v,d] = eig(P);
            a = sqrt(d(1,1));
            b = sqrt(d(2,2));
            c = sqrt(d(3,3));
            A = v(1:3,1:3);
            coi = getcoi(cart2kepler(rv));

            x = a*cosd(0:5:360);
            y = b*sind(0:5:360);
            z = zeros(size(x));
            xyz = coi*A*[x;y;z];
%             xxx = xx * a;
%             yyy = yy * b;
%             zzz = zz * c;
%             [m,n] = size(xxx);
%             for aa = 1:m
%                 for bb = 1:n
%                     r = coi*A*[xxx(aa,bb);yyy(aa,bb);zzz(aa,bb)];
%                     x(aa,bb) = r(1);
%                     y(aa,bb) = r(2);
%                     z(aa,bb) = r(3);
%                 end
%             end
            subplot(4,3,round(kk/120)),plot(xyz(1,:),xyz(3,:)),grid on,hold on,xlabel('x_e(km)'),ylabel('z_e(km)');
%             subplot(3,4,round(kk/120)),surf(x,y,z),grid on,hold on,axis equal;
            title([num2str(round(kk/120)*2),'h']);
        end
        kt(6) = kt(6) + sqrt(GEarth/kt(1)^3)*deg*60;
        rv = kepler2cart(krad(kt));
    end
    return
end
if nargin < 3
	mode = '2body';
end
switch mode
case '2body'
	%      (   Miu  [ x ] )
	%    d (- ----- [ y ] )
	%      (   r^3  [ z ] )
	%D = ------------------
	%          [ x ]
	%        d [ y ]
	%          [ z ]
	radius = sqrt(rv(1)^2+rv(2)^2+rv(3)^2);
	r = rv(1:3);
	D = GEarth/radius^3*(3*r*r'/radius^2 - eye(3));
	D = [zeros(3,3) eye(3);D  zeros(3,3)];
	phi = eye(6) + D*step;
case 'J2'
	x = rv(1);
	y = rv(2);
	z = rv(3);
	r = sqrt(x^2+y^2+z^2);
	xr = x/r;
	yr = y/r;
	zr = z/r;
	ur3 = GEarth/r^3;
	rr2j2 = J2*(Re/r)^2;
	D = [zeros(3,3) eye(3);zeros(3,6)];
	D(4,1) = -ur3*( (1-3*xr^2) + rr2j2*( 1.5*(1-5*zr^2) - 7.5*xr^2*(1-7*zr^2) ) );
	D(4,2) = ur3*xr*yr*(3+7.5*rr2j2*(1-7*zr^2));
	D(4,3) = ur3*xr*zr*(3+7.5*rr2j2*(1-7*zr^2));
	D(5,1) = D(4,2);
	D(5,2) = -ur3*( (1-3*yr^2) + rr2j2*( 1.5*(1-5*zr^2) - 7.5*yr^2*(1-7*zr^2) ) );
	D(5,3) = ur3*yr*zr*(3+7.5*rr2j2*(3-7*zr^2));
	D(6,1) = D(4,3);
	D(6,2) = D(5,3);
	D(6,3) = -ur3*( (1-3*zr^2) + 1.5*rr2j2*(35*zr^4-30*zr^2+3) );
	phi = eye(6) + D*step + 0.5*D^2*step^2;
end