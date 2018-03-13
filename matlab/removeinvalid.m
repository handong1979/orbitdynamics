% Remove any rows containing NaN or Inf from a matrix X.
function X = removeinvalid(X)
X(any(isnan(X),2),:) = [];
X(any(isinf(X),2),:) = [];