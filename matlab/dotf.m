% ������Ŀ����ո���(a,e,i,Omega,w,M)����������f�ı仯�ʺͽǼ��ٶ�
% [df,ddf] = dotf(k)
function [df,ddf] = dotf(a,e,f)
p = a*(1-e^2);
r = p./(1+e*cos(f));
df = sqrt(GEarth*p)./r.^2;
rdot = sqrt(GEarth/a/(1-e^2))*e*sin(f);
if nargout > 1
    ddf = -2*rdot.*df./r;
end