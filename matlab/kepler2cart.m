%�ɹ����������λ���ٶ�
% [r v] = kepler2cart(kepler,miu)
% use:
%     kepler2cart(kepler); %Ĭ��Ϊ����
%     kepler2cart(kepler,miu); %������������
%     keplerΪ[a e i Omega w M],��λΪkm,rad
%     ����ʱmiuΪ398600.5km^3/s^2������ʱmiuΪ4902.8km^3/s^2
% result:
%     cart = [x y z  vx vy vz] (��λ:km  km/s)
% See also: cart2kepler
% Copyright: ���� RDC BICE
function [r,v] = kepler2cart(kp,miu)
if nargin == 0 %self test
    kp = [8044,0.01,0.7,-1,1,0; ...
        7044,0.02,1.7,1,-1,2];
%     kepler = kepler';
end
[m,n] = size(kp);
kepler = kp;
if m == 6    
    tr = 0;
    len = n;
elseif n == 6;
    kepler = kp'; % ͳһ��Ϊ��ʸ������
    len = m;
    tr = 1; % ��������Ҫת�ó�������һ��
else
    error('kepler2cart: input kepler must dimension error');
end
if nargin < 2
    miu = 398600.4416;
end
sini = sin(kepler(3,:));
cosi = cos(kepler(3,:));
sino = sin(kepler(4,:));
coso = cos(kepler(4,:));
sinw = sin(kepler(5,:));
cosw = cos(kepler(5,:));
P = [coso.*cosw-sino.*sinw.*cosi;    sino.*cosw+coso.*sinw.*cosi;    sinw.*sini];
Q = [-coso.*sinw-sino.*cosw.*cosi;    -sino.*sinw+coso.*cosw.*cosi;    cosw.*sini];
E = keplerfunc(kepler(2,:),kepler(6,:));
if all(kepler(2,:) < 1.0)
    % ��Բ���
    r = kepler(1,:).*( 1.0 - kepler(2,:).*cos(E) );
    f = 2*atan2(sqrt((1+kepler(2,:))./(1-kepler(2,:))).*tan(E/2),1.0);
else
    % ˫���߹��
    r = kepler(1,:)*( 1.0 - kepler(2,:)*cosh(E) );
    f = atan2(-kepler(1,:).*sqrt(kepler(2,:)^1-1).*sinh(E),kepler(1,:).*(cosh(E) - kepler(1,:)));
end
p = kepler(1,:).*(1-kepler(2,:).^2);
rv = nan(6,len);
rv(1:3,1:len) = (ones(3,1)*(r.*cos(f))).*P + (ones(3,1)*(r.*sin(f))).*Q;
% rcf = r.*cos(f);
% rsf = r.*sin(f);
% rv(1,1:len) = rcf.*P(1,1:len) + rsf.*Q(1,1:len);
% rv(2,1:len) = rcf.*P(2,1:len) + rsf.*Q(2,1:len);
% rv(3,1:len) = rcf.*P(3,1:len) + rsf.*Q(3,1:len);
rv(4:6,1:len) = -(ones(3,1)*sqrt(miu./p)).*( (ones(3,1)*sin(f)).*P - (ones(3,1)*(cos(f)+kepler(2,:))).*Q );
% mpsf = -sqrt(miu./p).*sin(f);
% mpcf = sqrt(miu./p).*(cos(f)+kepler(2,:));
% rv(4,1:len) = mpsf.*P(1,1:len) + mpcf.*Q(1,1:len);
% rv(5,1:len) = mpsf.*P(2,1:len) + mpcf.*Q(2,1:len);
% rv(6,1:len) = mpsf.*P(3,1:len) + mpcf.*Q(3,1:len);

if nargout == 1 || nargout == 0
    if tr
        r = rv';
    else
        r = rv;
    end
elseif nargout == 2
    r = rv(1:3,:);
    v = rv(4:6,:);
    if tr
        r = r';
        v = v';
    end
end