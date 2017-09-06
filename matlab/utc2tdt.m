% UTC时转化为TDT时
% utc2tdt(mjd)将UTC时间mjd(简约儒略日表示)转化为TDT时间(简约儒略日表示)
%
% Example:
%     utc2tdt(53736)
%
% See also: utc2ut1,date2mjd

%   Copyright 2002-2009 RDC BICE. 
function tdt = utc2tdt(utc)
if utc>= 56109
    dTai = 35; % 2012 JUL  1
elseif utc>=54832
    dTai = 34; % 2009 JAN  1
elseif utc>=53736
    dTai = 33; % 2006 JAN  1
elseif utc>=51179
    dTai = 32; % 1999 JAN  1
elseif utc>=50630
    dTai = 31; % 1997 JUL  1 
elseif utc>=50083
    dTai = 30; % 1996 JAN  1
elseif utc>=49534
    dTai = 29; % 1994 JUL  1
elseif utc>=49169
    dTai = 28; % 1993 JUL  1
else
    dTai = 27;
end
tdt = utc + (dTai+32.184)/86400;