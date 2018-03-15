% 正定矩阵的UD分解
% [U, D] = UDUT(P)
% satifized: P = U*D*U'
function [UU, DD] = UD(P)
if nargin == 0 % self test
    n = 6;
    warning('self test mode');
    % create P U D
    D = diag(randc(n,1)*20);
    U = triu(randc(n,n)*20,1) + eye(n,n);
    P = U*D*U';
    % use matlab ldl function
%     [ll dd] = ldl(P);
end
%% UDUT 分解
n = length(P);
DD = zeros(n,1);
UU = eye(n,n);
DD(n) = P(n,n);
for i=n-1:-1:1
    UU(i,n) = P(i,n)/DD(n);
end
for j=n-1:-1:1
    sum = 0;
    for k=j+1:1:n
        sum = sum + DD(k)*UU(j,k)^2;
    end
    DD(j) = P(j,j) - sum;
    for i=j-1:-1:1
        sum = 0;
        for k=j+1:1:n
            sum = sum + DD(k)*UU(i,k)*UU(j,k);
        end
        UU(i,j) = (P(i,j)-sum) / DD(j);
    end
end
if nargin == 0 % self test output
    fprintf('norm(UU-U)=%e\n',norm(UU-U));
    fprintf('norm(diag(DD)-D)=%e\n',norm(diag(DD)-D));
    fprintf('norm(UU*DD*UU''-P)=%e\n',norm(UU*diag(DD)*UU'-P));
end