function [m A eigenfaces pr_img] = eigenface_core(database_path)
  %TODO
  filenames = dir(fullfile(database_path, '*.jpg'));
  totalImages = numel(filenames);
  
  T = [];
 
  for i = 1 : 10
    f = strcat(database_path, strcat('/', strcat(int2str(i), '.jpg')));
    currentImage = double(rgb2gray(imread(f))); 
    currentImage = currentImage(:);
    T = horzcat(T, currentImage);
  end

  m = mean(T, 2);
  A = T - m;
  
  [V, S] = eig(A' * A); 
  [n n] = size(V);
  
  pos = 0;
  
  for i = 1 : n
    if (S(i, i) > 1)
      break;
    end
  end
  
  V = V(:, i:end);
  
  eigenfaces = A * V;
  pr_img = eigenfaces' * A;
  
end