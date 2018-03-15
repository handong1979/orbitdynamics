%% param
epoch = [2015,1,1,4,0,0];
kp = [24371.137,0.73,28,0,0,0];
F = 0.2;
Isp = 2400;
dm = -F/Isp/9.8;
Mass = 2000;
%% de421
mjd = date2mjd(epoch);
moon = de421(mjd,'Earth','Moon');
mars = de421(mjd,'Earth','Mars');
venus = de421(mjd,'Earth','Venus');

%% orbitdyn
s1 = mexOrbitDyn('sat','init',[epoch,kp,Mass]);
% mexOrbitDyn('sat','setforce',[8,1+2+4+8+16+32]);
% mexOrbitDyn('sat','SETAD',20);
% mexOrbitDyn('sat','SETSRP',20);
% ecf = mexOrbitDyn('sat','GetECF');
step = 60;

return
for i = 1:1000
    qbi = c2q(getcoi(s1(2:7)));
    s1 = mexOrbitDyn('sat','STEP',[step, qbi', 0, 0, F, dm]);
    s(i,:)=s1;
end
mexOrbitDyn('sat','REMOVE');
figure,plot6coe(1:1000,s(:,2:7));
figure,plot6(1:1000,s(:,8:13));


