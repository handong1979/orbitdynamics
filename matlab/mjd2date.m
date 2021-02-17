% 由简约儒略日计算日期
% mjd2date(mjd)简约儒略日mjd对应的日期时间[year,month,day,hour,minute,second]
%
% Example：
%      mjd2date(54212.5212962963)
%
% See also date2mjd
function date = mjd2date(MJD)
JD = MJD+2400000.5;
J  = floor( JD+0.5 );
N  = floor( 4*(J+68569)/146097 );
L1 = J + 68569 - floor( (N*146097+3)/4 );
Y1 = floor( 4000*(L1+1)/1461001 );
L2 = L1 + 31 - floor( 1461*Y1/4 );
M1 = floor( 80*L2/2447 );
D  = L2 - floor( (2447*M1)/80 );
L3 = floor( M1/11 );
M  = M1 + 2 - 12*L3;
Y  = floor( 100*(N-49)+Y1+L3 );
hms= 86400*( MJD - floor(MJD) );
h  = floor(hms/3600);
m  = floor((hms-h*3600)/60);
s  = hms-h*3600-m*60;
date = [Y,M,D,h,m,s];