% 年月日时分秒(世界时)转化为简约儒略日
% 输入参数格式可以见datenum函数的格式
%
% Example:
%      date2mjd('2009-1-1 12:0:0');
%      date2mjd(2009,1,1,12,0,0);
%
% See Also mjd2date,datenum,datevec
function MJD = date2mjd( varargin )
[year,month,day,hour,min,sec] = datevec(datenum(varargin{:}));
tmp = fix((month-14)/12);
J = day - 32075 +  fix( 1461.*(year+4800+tmp)./4 )  ...
    +  fix( 367.*(month - 2- tmp*12)./12 )  ...
    - fix( 3 .* fix((year+4900+tmp)./100) ./ 4 );
JD = J - 0.5 + hour./24 + min./1440 + sec./86400;
MJD = JD - 2400000.5;
