gj = load('groundtraj.txt');
figure
subplot 211,plot(gj(:,1),gj(:,2)),xlabel('t'),ylabel('\DeltaL(km)');
subplot 212,plot(gj(:,1),gj(:,3)),xlabel('t'),ylabel('\Deltaa(km)');
figure
plot(gj(:,2),gj(:,3)),xlabel('\DeltaL(km)'),ylabel('\Deltaa(km)');
figure
plot(gj(:,3),gj(:,2)),xlabel('\Deltaa(km)'),ylabel('\DeltaL(km)');
p = polyfit(gj(:,3),gj(:,2),2);
da = [-20:1:10];
dl = polyval(p,da);
hold on;
plot(da,dl,'r');