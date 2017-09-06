%  平近点角M转换为真近点角f
%  f = ma2ta(e,M)
function f = ma2ta(e,M)
if nargin == 0 %self test
    e = 0:0.01:0.99;
    M = linspace(0,2*pi,length(e));
end
EE = keplerfunc(e,M);
cosf = (cos(EE)-e) ./ (1-e.*cos(EE));
sinf = sqrt(1-e.^2).*sin(EE)./(1-e.*cos(EE));
f = atan2(sinf,cosf);
f(f<0)=f(f<0)+pi*2;
if nargin == 0 %self test
    figure,plot(e,M,e,f),legend('M','f'),xlabel('e');
end
