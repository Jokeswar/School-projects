function task5(image)
   %TODO
   A = double(imread(image));
   [m, n] = size(A);
   values = svd(A);
   k = length(values);
   
   % figure 1
   figure(1);
   [A_k, S] = task3(image, k);
   values = diag(S);
   plot(values);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
   
   % figure 2
   figure(2);
   totalSum = sum(values);
   
   k = length(values);
   partialSum = [];
   partialSum(1) = values(1);
   for i = 2 : k
      partialSum(i) = values(i) + partialSum(i-1);
   end
   partialSum = partialSum./totalSum;
   plot(partialSum);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
   
   % figure 3
   figure(3);
   err = [];
   x = [1 : 10 : k];
   for i = 1 : length(x)
    A_k = double(task3(image, x(i)));
    squareDiff = (A - A_k).^2;
    err(i) = sum(squareDiff(:));
   end
   err = err./(m*n);
   plot(x, err);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
   
   % figure 4
   figure(4);
   compresionRate = [];
   for i = 1:k
     compresionRate(i) = (2*i + 1)/(n);
   end
   plot(compresionRate);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
end