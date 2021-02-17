% TDTʱת��ΪUTCʱ
% tdt2utc(mjd)��TDTʱ��mjd(��Լ�����ձ�ʾ)ת��ΪUTCʱ��(��Լ�����ձ�ʾ)
%
% Example:
%     tdt2utc(53736)
%
% See also: utc2tdt,utc2ut1,date2mjd

%   Copyright 2002-2009 RDC BICE. 
function utc = tdt2utc(tdt)
if tdt>= 57754
    dTai = 37; % 2017 JAN  1
elseif tdt>= 57204
    dTai = 36; % 2015 JUL  1
elseif tdt>= 56109
    dTai = 35; % 2012 JUL  1
elseif tdt>=54832   %%# TODO :ͬ�ϣ�ʹ��TDTʱ����Ϊ�жϱ߽磬��������UTC
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