function [XYZ, H, DEC, DIP, F] = wrldmagmgeo(height, lat, lon, dyear, varargin )
%  WRLDMAGM Use World Magnetic Model.
%   [XYZ, H, DEC, DIP, F] = WRLDMAGM(HEIGHT, LAT, LON, DYEAR ) calculates the 
%   Earth's magnetic field at a specific location and time using the World 
%   Magnetic Model (WMM).  The default WMM is WMM-2010 which is valid from
%   January 1, 2010 until December 31, 2014.
%
%   Inputs required by WMM are:
%   HEIGHT :a scalar value in meters. 
%   LAT    :a scalar geodetic latitude in degrees where north latitude is 
%          positive, and south latitude is negative.
%   LON    :a scalar geodetic longitude in degrees where east longitude 
%          is positive, west is negative.
%   DYEAR  :a scalar decimal year.  Decimal year is the desired year in 
%          a decimal format to include any fraction of the year that has 
%          already passed.
%
%   Output calculated for the Earth's magnetic field includes:
%   XYZ    :magnetic field vector in nanotesla (nT). 
%   H      :horizontal intensity in nanotesla (nT). 
%   DEC    :declination in degrees. 
%   DIP    :inclination in degrees. 
%   F      :total intensity in nanotesla (nT).
%
%   [XYZ, H, DEC, DIP, F] = WRLDMAGM(HEIGHT, LAT, LON, DYEAR, '2010' ) is an 
%   alternate method for calling WMM-2010 or 2010 epoch.
%
%   Limitations:
%
%   This function has the limitations of the World Magnetic Model(WMM). For
%   more information see the documentation. 
%
%   Example:
%
%   Calculate the magnetic model 1000 meters over Natick, Massachusetts on 
%   July 4, 2010 using WMM-2010:
%      [XYZ, H, DEC, DIP, F] = wrldmagm(1000, 42.283, -71.35, decyear(2010,7,4),'2010')
%
%   See also DECYEAR, IGRF11MAGM.

%   Copyright 2000-2011 The MathWorks, Inc.

%   Limitations:  The WMM specification produces data that is reliable 
%   five years after the epoch of the model, which begins January 1, of the 
%   model year selected.  The WMM specification describes only the 
%   long-wavelength spatial magnetic fluctuations due to the Earth's core. 
%   Intermediate and short-wavelength fluctuations, contributed from the 
%   crustal field (the mantle and crust), are not included. Also, the 
%   substantial fluctuations of the geomagnetic field, which occur constantly
%   during magnetic storms and almost constantly in the disturbance field 
%   (auroral zones), are not included.

%   Reference:
%   [1] The WMM-2010 can be found on the web at
%   http://www.ngdc.noaa.gov/geomag/WMM/DoDWMM.shtml and in "NOAA Technical
%   Report: The US/UK World Magnetic Model for 2010-2015". 

narginchk(4, 5);

% Get string value for epoch
if ((nargin > 4) && ischar(varargin{1}))
    varargin{1} = str2double(varargin{1});
end

% Use 2010 epoch or catch numeric inputs for epoch
if ((nargin < 5) || (varargin{1} == 2010))
    epoch = 2010;
    load aerowmm2010;
elseif (varargin{1} == 2005)
    warning(message('aero:wrldmagm:obsoleteEpoch',2005));
    epoch = 2005;
    load aerowmm2005;
elseif (varargin{1} == 2000)
    warning(message('aero:wrldmagm:obsoleteEpoch',2000));
    epoch = 2000;
    load aerowmm2000;
else
    error(message('aero:wrldmagm:invalidEpoch'));
end

% Calculate the time difference from the epoch
dt = dyear - epoch;

if ((nargin < 5) && ( dt < 0.0 && dt >= -5.0 ))
    error(message('aero:wrldmagm:updateEpoch',2010));
end

if ( isnan(dt) || dt < 0.0 || dt > 5.0)
    error(message('aero:wrldmagm:invalidDYear'));
end

if ( isnan(height) || height < 0.0  || height > 40000.0  )
    error(message('aero:wrldmagm:invalidHeight'));
end

if ( isnan(lat) || lat < -90.0  || lat > 90.0  )
    error(message('aero:wrldmagm:invalidLatitude'));
end

if ( isnan(lat) || lon < -180.0  || lon > 180.0  )
    error(message('aero:wrldmagm:invalidLongitude'));
end

maxord = maxdef + 1;

tc = zeros(maxord,maxord);
dp = zeros(maxord,maxord);
pp = ones(maxord,1);
snorm = zeros(maxord*maxord,1);
sp = zeros(1,maxord); 
cp = zeros(1,maxord);
dp(1,1) = 0.0;
cp(1) = 1.0; 
snorm(1) = 1.0; 
pp(1) = 1.0;

% convert to kilometers
% height = height*0.001;

a = 6378.137;
b = 6356.7523142;
re = 6371.2;
a2 = a*a;
b2 = b*b;
c2 = a2-b2;
a4 = a2*a2;
b4 = b2*b2;
c4 = a4 - b4;

rlon = convang( lon, 'deg','rad' );
rlat = convang( lat, 'deg','rad' );
srlon = sin(rlon);
srlat = sin(rlat);
crlon = cos(rlon);
crlat = cos(rlat);
srlat2 = srlat*srlat;
crlat2 = crlat*crlat;
sp(2) = srlon;
cp(2) = crlon;

% convert from geodetic coordinates to spherical coordinates
q = sqrt(a2-c2*srlat2);
q1 = height*q;
q2 = ((q1+a2)/(q1+b2))*((q1+a2)/(q1+b2));
ct = srlat/sqrt(q2*crlat2+srlat2);
st = sqrt(1.0-(ct*ct));
r2 = (height*height)+2.0*q1+(a4-c4*srlat2)/(q*q);
r = sqrt(r2);
d = sqrt(a2*crlat2+b2*srlat2);
ca = (height+d)/r;
sa = c2*crlat*srlat/(r*d);

for m = 3:maxord
    sp(m) = sp(2)*cp(m-1)+cp(2)*sp(m-1);
    cp(m) = cp(2)*cp(m-1)-sp(2)*sp(m-1);
end
aor = re/r;
ar = aor*aor;
br = 0.0; bt = 0.0; bp = 0.0; bpp = 0.0;
for n = 1:maxord-1
    ar = ar*aor;
    for m = 0:n
        %     Compute unnormalized associated legendre polynomials
        %     and derivatives via recursion relations
        if (n == m)
            snorm(n+1 + m*maxord) = st*snorm(n + (m-1)*maxord);
            dp(m+1, n+1) = st*dp(m, n)+ct*snorm(n +(m-1)*maxord);
        end
        if (n == 1 && m == 0)
            snorm(n+1) = ct*snorm(n);
            dp(m+1, n+1) = ct*dp(m+1, n)-st*snorm(n);
        end
        if (n > 1 && n ~= m)
            if (m > n-2)
                snorm(n-2+m*maxord) = 0.0;
                dp(m+1, n-1) = 0.0;
            end
            snorm(n+1 + m*maxord) = ct*snorm(n + m*maxord)-k(m+1, n+1)*snorm(n-1 + m*maxord);
            dp(m+1, n+1) = ct*dp(m+1, n) - st*snorm(n + m*maxord)-k(m+1, n+1)*dp(m+1, n-1);
        end

        %   Time adjust the Gauss coefficients
        tc(m+1, n+1) = c(m+1, n+1)+dt*dc(m+1, n+1);
        if (m ~= 0)
            tc(n+1, m) = c(n+1, m)+dt*dc(n+1, m);
        end

        %    Accumulate terms of the spherical harmonic expansions
        par = ar*snorm(n+1+m*maxord);
        if (m == 0)
            temp1 = tc(m+1, n+1)*cp(m+1);
            temp2 = tc(m+1, n+1)*sp(m+1);
        else
            temp1 = tc(m+1, n+1)*cp(m+1)+tc(n+1, m)*sp(m+1);
            temp2 = tc(m+1, n+1)*sp(m+1)-tc(n+1, m)*cp(m+1);
        end
        bt = bt-ar*temp1*dp(m+1, n+1);
        bp = bp+(fm(m+1)*temp2*par);
        br = br+(fn(n+1)*temp1*par);

        %   Special Case:  North/South geographic poles

        if (st == 0.0 && m == 1)
            if (n == 1)
                pp(n+1) = pp(n);
            else
                pp(n+1) = ct*pp(n)-k(m+1,n+1)*pp(n-1);
            end
            parp = ar*pp(n+1);
            bpp = bpp + (fm(m+1)*temp2*parp);
        end

    end
end
if (st == 0.0)
    bp = bpp;
else
    bp = bp/st;
end

%    Rotate magnetic vector components from spherical to geodetic
%    coordinates

bx = -bt*ca-br*sa;
by = bp;
bz = bt*sa-br*ca;

%   Compute declination (DEC), inclination (DIP) & total intensity (F)

bh = sqrt((bx*bx)+(by*by));
F = sqrt((bh*bh)+(bz*bz));
DEC = atan2(by,bx);
DIP = atan2(bz,bh);
%   Compute XYZ & H components of the magnetic field

X = F*cos(DEC)*cos(DIP);
Y = F*cos(DIP)*sin(DEC);
Z = F*sin(DIP);
XYZ = [X; Y; Z];

H = F*cos(DIP);
DEC = convang( DEC, 'rad','deg' );
DIP = convang( DIP, 'rad','deg' );
end

