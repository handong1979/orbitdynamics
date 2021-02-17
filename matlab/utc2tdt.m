% UTCʱת��ΪTDTʱ
% utc2tdt(mjd)��UTCʱ��mjd(��Լ�����ձ�ʾ)ת��ΪTDTʱ��(��Լ�����ձ�ʾ)
%
% Example:
%     utc2tdt(53736)
%
% See also: utc2ut1,date2mjd

%   Copyright 2002-2009 RDC BICE. 
function tdt = utc2tdt(utc)
if utc>= 57754
    dTai = 37; % 2017 JAN  1
elseif utc>= 57204
    dTai = 36; % 2015 JUL  1
elseif utc>= 56109
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