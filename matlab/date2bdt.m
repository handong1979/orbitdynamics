% ����ʱת��Ϊ����ʱ����
function bdt = date2bdt(t)
t0tdt = utc2tdt(date2mjd(2006,1,1,0,0,0));
ttt = utc2tdt(date2mjd(t));
bdt = (ttt - t0tdt)*86400;