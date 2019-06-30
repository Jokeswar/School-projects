function task2(image)
   %TODO
   A = double(imread(image));
   [m, n] = size(A);
   
   % figure 1
   values = svd(A);
   figure(1);
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
   for i = 1 : length(x);
    A_k = task1(image, x(i));
    squareDiff = (A - A_k).^2;
    err(i) = sum(sum(squareDiff));
   end
   err = err./(m*n);
   plot(x, err);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
   
   % figure 4
   figure(4);
   compresionRate = [];
   for i = 1:k
     compresionRate(i) = (m*i + n*i + i)/(m*n);
   end
   plot(compresionRate);
   set(findall(gca, 'Type', 'Line'), 'LineWidth', 2);
end