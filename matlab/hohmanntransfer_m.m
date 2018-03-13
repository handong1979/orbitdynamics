% 环月轨道的霍曼转移
% [dv1 dv2] = hohmanntransfer(h1,h2)
% a1 初始圆轨道半长轴(km)
% a2 目标园轨道半长轴(km)
% dv1 第一次变轨速度增量
% dv2 第二次变轨速度增量
function [dv dv1 dv2] = hohmanntransfer_m(a1,a2)
if a1<Rm
    a1 = Rm + a1;
end
if a2<Rm
    a2 = Rm + a2;
end
at = (a1 + a2) / 2;
dv1 = ( sqrt(GMoon*(2/a1-1/at)) - sqrt(GMoon/a1) )*1000;
dv2 = ( sqrt(GMoon/a2) - sqrt(GMoon*(2/a2-1/at)) )*1000;
dv = abs(dv1)+abs(dv2);
