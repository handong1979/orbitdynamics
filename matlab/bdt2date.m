% 北斗时转换为UTC日历
function d = bdt2date(t)
t0tdt = utc2tdt(date2mjd(2006,1,1,0,0,0));
tdt = t/86400 + t0tdt;
d = mjd2date(tdt2utc(tdt));