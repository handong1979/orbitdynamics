%This routine implements a new algorithm that solves Lambert's problem. The
%algorithm has two major characteristics that makes it favorable to other
%existing ones. 
%
%Programmed by:         Dario Izzo (Advanced Concept Team) Date:
%28/05/2004 Revision:              1 Tested by:             ----------
%
%
%   1) It describes the generic orbit solution of the boundary condition
%   problem through the variable X=log(1+cos(alpha/2)). By doing so the
%   graphs of the time of flight become defined in the entire real axis and
%   resembles a straight line. Convergence is granted within few iterations
%   for all the possible geometries (except, of course, when the transfer
%   angle is zero). When multiple revolutions are considered the variable is
%   X=tan(cos(alpha/2)*pi/2).
%
%   2) Once the orbit has been determined in the plane, this routine
%   evaluates the velocity vectors at the two points in a way that is not
%   singular for the transfer angle approaching to pi (Lagrange coefficient
%   based methods are numerically not well suited for this purpose).
%
%   As a result Lambert's problem is solved (with multiple revolutions
%   being accounted for) with the same computational effort for all
%   possible geometries. The case of near 180 transfers is also solved
%   efficiently.
%
%   We note here that even when the transfer angle is exactly equal to pi
%   the algorithm does solve the problem in the plane (it finds X), but it
%   is not able to evaluate the plane in which the orbit lies. A solution 
%   to this would be to provide the direction of the plane containing the
%   transfer orbit from outside. This has not been implemented in this 
%   routine since such a direction would depend on which application the 
%   transfer is going to be used in.
%
%Usage: [v1,v2,a,p,theta,iter]=lambertI(r1,r2,t,mu,lw,N,branch)
%
%Inputs:
%           r1=Position vector at departure (column) 
%           r2=Position vector at arrival (column, same units as r1)
%           t=Transfer time (scalar)
%           mu=gravitational parameter (scalar, units have to be
%           consistent with r1,t units) 
%           lw=1 if long way is chosen    
%           branch='l' if the left branch is selected in a problem where N
%           is not 0 (multirevolution)
%           N=number of revolutions
%
%Outputs:
%           v1=Velocity at departure        (consistent units)
%           v2=Velocity at arrival
%           a=semi major axis of the solution
%           p=semi latus rectum of the solution 
%           theta=transfer angle in rad
%           iter=number of iteration made by the newton solver (usually 6)
%
%please report bugs to dario.izzo@esa.int


function [v1,v2,iter]=lambertI(r1,r2,t,mu,N,branch)
crs=cross(r1,r2);
lw=sign(crs(3));
if lw==1
        lw=0;
    else
        lw=1;
end

%Preliminary control on the function call
if nargin==5
    N=0;
end
if t<=0
%     warning('Negative time as input')
    v1=NaN;
    v2=NaN;
    return
end


tol=1e-11;  %Increasing the tolerance does not bring any advantage as the 
%precision is usually greater anyway (due to the rectification of the tof
%graph) except near particular cases such as parabolas in which cases a
%lower precision allow for usual convergence.


%Non dimensional units
R=sqrt(r1'*r1);
V=sqrt(mu/R);
T=R/V;

%working with non-dimensional radii and time-of-flight
r1=r1/R;
r2=r2/R;
t=t/T;                     

%Evaluation of the relevant geometry parameters in non dimensional units
r2mod=sqrt(r2'*r2);
theta=real(acos((r1'*r2)/r2mod)); %the real command is useful when theta is very 
                                  %close to pi and the acos function could return complex numbers
if lw
    theta=2*pi-theta;
end
c=sqrt(1+r2mod^2-2*r2mod*cos(theta)); %non dimensional chord
s=(1+r2mod+c)/2;                      %non dimensional semi-perimeter
am=s/2;                               %minimum energy ellipse semi major axis
lambda=sqrt(r2mod)*cos(theta/2)/s;    %lambda parameter defined in BATTIN's book



%We start finding the log(x+1) value of the solution conic:
%%NO MULTI REV --> (1 SOL)
if N==0
    inn1=-.5233;    %first guess point
    inn2=.5233;     %second guess point
    x1=log(1+inn1);
    x2=log(1+inn2);
    y1=log(x2tof(inn1,s,c,lw,N))-log(t);
    y2=log(x2tof(inn2,s,c,lw,N))-log(t);
    
    %Newton iterations
    err=1;
    i=0;
    while ((err>tol) & (y1~=y2))
        i=i+1;
        xnew=(x1*y2-y1*x2)/(y2-y1);
        ynew=log(x2tof(exp(xnew)-1,s,c,lw,N))-log(t);
        x1=x2;
        y1=y2;
        x2=xnew;
        y2=ynew;
        err=abs(x1-xnew);
    end
    iter=i;
    x=exp(xnew)-1;
    
    
    %%MULTI REV --> (2 SOL) SEPARATING RIGHT AND LEFT BRANCH
else 
    if branch=='l'
        inn1=-.5234;
        inn2=-.2234;
    else
        inn1=.7234;
        inn2=.5234;
    end
    x1=tan(inn1*pi/2);
    x2=tan(inn2*pi/2);
    y1=x2tof(inn1,s,c,lw,N)-t;
    
    y2=x2tof(inn2,s,c,lw,N)-t;
    err=1;
    i=0;
    
    %Newton Iteration
    while ((err>tol) & (i<60) & (y1~=y2))
        i=i+1;
        xnew=(x1*y2-y1*x2)/(y2-y1);
        ynew=x2tof(atan(xnew)*2/pi,s,c,lw,N)-t;
        x1=x2;
        y1=y2;
        x2=xnew;
        y2=ynew;
        err=abs(x1-xnew);	    
    end
    x=atan(xnew)*2/pi;
    iter=i;
end
%The solution has been evaluated in terms of log(x+1) or tan(x*pi/2), we
%now need the conic. As for transfer angles near to pi the lagrange
%coefficient technique goes singular (dg approaches a zero/zero that is
%numerically bad) we here use a different technique for those cases. When
%the transfer angle is exactly equal to pi, then the ih unit vector is not
%determined. The remaining equations, though, are still valid.


a=am/(1-x^2);                       %solution semimajor axis
%calcolo psi
if x<1 %ellisse
    beta=2*asin(sqrt((s-c)/2/a));
    if lw
        beta=-beta;
    end
    alfa=2*acos(x);
    psi=(alfa-beta)/2;
    eta2=2*a*sin(psi)^2/s;
    eta=sqrt(eta2);
else %iperbole
    beta=2*asinh(sqrt((c-s)/2/a));
    if lw
        beta=-beta;
    end
    alfa=2*acosh(x);
    psi=(alfa-beta)/2;
    eta2=-2*a*sinh(psi)^2/s;
    eta=sqrt(eta2);
end
p=r2mod/am/eta2*sin(theta/2)^2;     %parameter of the solution
sigma1=1/eta/sqrt(am)*(2*lambda*am-(lambda+x*eta));

ih=cross(r1,r2)/norm(cross(r1,r2));
if lw
    ih=-ih;
end

vr1 = sigma1;
vt1 = sqrt(p);
v1  = vr1 * r1   +   vt1 * cross(ih,r1);

vt2=vt1/r2mod;
vr2=-vr1+(vt1-vt2)/tan(theta/2);
v2=vr2*r2/r2mod+vt2*cross(ih,r2/r2mod);
v1=v1*V;
v2=v2*V;
a=a*R;
p=p*R;

%Subfunction that evaluates the time of flight as a function of x
function t=x2tof(x,s,c,lw,N)  

am=s/2;
a=am/(1-x^2);
if x<1 %ELLISSE
    beta=2*asin(sqrt((s-c)/2/a));
    if lw
        beta=-beta;
    end
    alfa=2*acos(x);
else   %IPERBOLE
    alfa=2*acosh(x);
    beta=2*asinh(sqrt((s-c)/(-2*a)));
    if lw
        beta=-beta;
    end
end
t=tofabn(a,alfa,beta,N);
%subfunction that evaluates the time of flight via Lagrange expression
function t=tofabn(sigma,alfa,beta,N)

if sigma>0
    t=sigma*sqrt(sigma)*((alfa-sin(alfa))-(beta-sin(beta))+N*2*pi);
else
    t=-sigma*sqrt(-sigma)*((sinh(alfa)-alfa)-(sinh(beta)-beta));
end







  