function [min_dist output_img_index] = face_recognition(image_path, m, A, eigenfaces, pr_img)
  %TODO
  min_dist = inf;
  output_img_index = -1;
  
  currentImage = double(rgb2gray(imread(image_path)));
  currentImage = currentImage(:);
  
  currentImage = currentImage - m;
  prTestImg = (eigenfaces')*currentImage;
  
  [n m] = size(pr_img);
  
  dist = sqrt(sum((prTestImg - pr_img).^2));
  
  for i = 1 : m      
    if(dist(i) < min_dist)
      min_dist = dist(i);
      output_img_index = i;
    end
  end
  
end