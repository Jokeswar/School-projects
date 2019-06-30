function [A_k S] = task3(image, k)
  %TODO
  A_k = [];
  S = [];
  
  A = double(imread(image));
  [m, n] = size(A);
  
  miu = [];
  for i = 1:m
    miu(i, 1) = mean(A(i, :));
    A(i, :) = A(i, :) - miu(i, 1);
  end
  
  Z = (A')./sqrt(n-1);
  [U S V] = svd(Z);
  
  W = V(:, 1:k);
  Y = (W')*(A);
  
  A_k = W*Y + miu;

endfunction