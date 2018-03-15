% remove an entire row of data containing the outlier
% 第一列为时间，不进行超差判断，删除时对应的删除
function [t,X] = removebias(t,X,y)
error(nargchk(1, 3, nargin))
if nargin == 1
    X = t;
    y = 6;
elseif nargin == 2
    y = 6;
    if nargout ~= 2
        error('input t and X, but not enough output arguments');
    end
end
[n p] = size(X);
m = mean(X);
s = std(X);
m = repmat(m,n,1);
s = repmat(s,n,1);
outliers = abs(X - m) > y*s;
if nargin == 1
    t(any(outliers,2),:) = [];
elseif nargout == 2
    dt = [0; diff(t)];
    mt = mean(dt);
    st = std(dt);
    tout = abs(dt-mt) > y*st;
    outliers = [tout,outliers];
    t(any(outliers,2),:) = [];
    X(any(outliers,2),:) = [];
end