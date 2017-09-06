%解开普勒方程 M = E - e*sin(E)
function E = keplerfunc(e,M)
% error(nargchk(2,2,nargin));
if nargin == 0 %self test
    e = 0:0.01:0.99;
    M = linspace(0,2*pi,length(e));
end
if any(e<0) || any(e>=1)
    error('偏心率小于零或大于1');
end
E0 = M;
if all(e<1.0)
    E1 = E0-(E0-e.*sin(E0)-M)./(1-e.*cos(E0));
else
    E1 = E0-(e.*sinh(E0)-E0-M)/(e.*cosh(E0)-1.0);
end
% n = 0;
while any(abs(E1-E0)>1e-12)
    E0 = E1;
%      n = n+1;
    if all(e<1.0)
        E1 = E0-(E0-e.*sin(E0)-M)./(1-e.*cos(E0));
    else
        E1 = E0-(e.*sinh(E0)-E0-M)./(e.*cosh(E0)-1.0);
    end
end
E = E1;
% fprintf('%d ',n);
% if nargin == 0 %self test
%     figure,plot(e,M,e,E),legend('M','E'),xlabel('e');
% end