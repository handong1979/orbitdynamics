% This function solves Lambert¡¯s problem.
% mu - gravitational parameter (km??3/s??2)
% R1, R2 - initial and final position vectors (km)
% r1, r2 - magnitudes of R1 and R2
% t - the time of flight from R1 to R2
%     (a constant) (s)
% V1, V2 - initial and final velocity vectors (km/s)
% c12 - cross product of R1 into R2
% theta - angle between R1 and R2
% string - 'pro' if the orbit is prograde
% 'retro' if the orbit is retrograde
% A - a constant given by Equation 5.35
% z - alpha*x??2, where alpha is the reciprocal of the
% semimajor axis and x is the universal anomaly
% y(z) - a function of z given by Equation 5.38
% F(z,t) - a function of the variable z and constant t,
% given by Equation 5.40
% dFdz(z) - the derivative of F(z,t), given by
% Equation 5.43
% ratio - F/dFdz
% tol - tolerance on precision of convergence
% nmax - maximum number of iterations of Newton¡¯s
% procedure
% f, g - Lagrange coefficients
% gdot - time derivative of g
% C(z), S(z) - Stumpff functions
% dum - a dummy variable
% -----------------------------------------------------------
function [V1, V2] = lambert(R1, R2, t, string)
if nargin == 0
    warning('self test');
    R1 = [7700;0;0];
    R2 = [0;0;7000];
    t = 25*60;
    string  = 'pro';
    [V1, V2] = lambert(R1, R2, t, string)
%     kp1 = cart2kepler([R1;V1])
%     kp2 = cart2kepler([R2;V2])
    return
end

mu = GEarth;
%...Magnitudes of R1 and R2:
r1 = norm(R1);
r2 = norm(R2);
c12 = cross(R1, R2);
theta = acos(dot(R1,R2)/r1/r2);

%...Determine whether the orbit is prograde or retrograde:
if strcmp(string, 'pro')
    if c12(3) < 0
        theta = 2*pi - theta;
    end
elseif strcmp(string,'retro')
    if c12(3) > 0
        theta = 2*pi - theta;
    end
else
    string = 'pro';
    fprintf('\n ** Prograde trajectory assumed.\n')
end
%...Equation 5.35:
A = sin(theta)*sqrt(r1*r2/(1 - cos(theta)));

%...Determine approximately where F(z,t) changes sign, and
%...use that value of z as the starting value for Equation 5.45:
z = -100;
while F(z,t) < 0
    z = z + 0.1;
end

%...Set an error tolerance and a limit on the number of iterations:
tol = 1.e-8;
nmax = 5000;
%...Iterate on Equation 5.45 until z is determined to within
%...the error tolerance:
ratio = 1;
n = 0;
while (abs(ratio) > tol) && (n <= nmax)
    n = n + 1;
    ratio = F(z,t)/dFdz(z);
    z = z - ratio;
end
%...Report if the maximum number of iterations is exceeded:
if n >= nmax
    fprintf('\n\n **Number of iterations exceeds')
    fprintf(' %g \n\n ', nmax)
end
%...Equation 5.46a:
f = 1 - y(z)/r1;
%...Equation 5.46b:
g = A*sqrt(y(z)/mu);
%...Equation 5.46d:
gdot = 1 - y(z)/r2;
%...Equation 5.28:
V1 = 1/g*(R2 - f*R1);
%...Equation 5.29:
V2 = 1/g*(gdot*R2 - R1);

% ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
% Subfunctions used in the main body:
%...Equation 5.38:
    function dum = y(z)
        dum = r1 + r2 + A*(z*stumpS(z) - 1)/sqrt(stumpC(z));
    end
%...Equation 5.40:
    function dum = F(z,t)
        dum = (y(z)/stumpC(z))^1.5*stumpS(z) + A*sqrt(y(z)) - sqrt(mu)*t;
    end
%...Equation 5.43:
    function dum = dFdz(z)
        if z == 0
            dum = sqrt(2)/40*y(0)^1.5 + A/8*(sqrt(y(0)) ...
                + A*sqrt(1/2/y(0)));
        else
            dum = (y(z)/stumpC(z))^1.5*(1/2/z*(stumpC(z) - 3*stumpS(z)/2/stumpC(z)) ...
                + 3*stumpS(z)^2/4/stumpC(z)) ...
                + A/8*(3*stumpS(z)/stumpC(z)*sqrt(y(z)) ...
                + A*sqrt(stumpC(z)/y(z)));
        end
    end
% ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
    function s = stumpS(z)
        % ????????????????????????????????????????????
        %
        % This function evaluates the Stumpff function S(z) according
        % to Equation 3.49.
        %
        % z - input argument
        % s - value of S(z)
        %
        % User M-functions required: none
        % ------------------------------------------------------------
        if z > 0
            s = (sqrt(z) - sin(sqrt(z)))/(sqrt(z))^3;
        elseif z < 0
            s = (sinh(sqrt(-z)) - sqrt(-z))/(sqrt(-z))^3;
        else
            s = 1/6;
        end
    end
% ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
    function c = stumpC(z)
        % ????????????????????????????????????????????
        %
        % This function evaluates the Stumpff function C(z) according
        % to Equation 3.50.
        %
        % z - input argument
        % c - value of C(z)
        %
        % User M-functions required: none
        % ------------------------------------------------------------
        if z > 0
            c = (1 - cos(sqrt(z)))/z;
        elseif z < 0
            c = (cosh(sqrt(-z)) - 1)/(-z);
        else
            c = 1/2;
        end        
    end
end