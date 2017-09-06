% TDT时转化为UTC时
% tdt2utc(mjd)将TDT时间mjd(简约儒略日表示)转化为UTC时间(简约儒略日表示)
%
% Example:
%     tdt2utc(53736)
%
% See also: utc2tdt,utc2ut1,date2mjd

%   Copyright 2002-2009 RDC BICE. 
function utc = tdt2utc(tdt)
if tdt>= 56109.0007775926
    dTai = 35; % 2012 JUL  1
elseif tdt>=54832   %%# TODO :同上，使用TDT时间作为判断边界，而不是用UTC
    dTai = 34; % 2009 JAN  1
elseif tdt>=53736
    dTai = 33; % 2006 JAN  1
elseif tdt>=51179
    dTai = 32; % 1999 JAN  1
elseif tdt>=50630
    dTai = 31; % 1997 JUL  1 
elseif tdt>=50083
    dTai = 30; % 1996 JAN  1
elseif tdt>=49534
    dTai = 29; % 1994 JUL  1
elseif tdt>=49169
    dTai = 28; % 1993 JUL  1
else
    dTai = 27;
end
utc = tdt - (dTai+32.184)/86400;