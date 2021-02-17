% 均匀分布的随机+1或-1
function pn = randpn()
r = rand;
if r>0.5
    pn = 1;
else
    pn = -1;
end