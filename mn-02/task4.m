function [A_k S] = task4(image, k)
  %TODO
  A_k = [];
  S = [];
  
  A = imread(image);
  A = double(A);
  [m, n] = size(A);
  
  miu = [];
  for i = 1:m
    miu(i, 1) = mean(A(i, :));
    A(i, :) = A(i, :) - miu(i, 1);
  end
  
  Z = A*(A')./(n-1);
  [V, S] = eig(Z);
  
  W = V(:, (end - k + 1):end);
  Y = (W')*(A);
  
  A_k = W*Y + miu;

endfunction