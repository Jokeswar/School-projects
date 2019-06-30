% outputs a graphical representation of the clustering solution
function view_clusters(points, centroids)
  numberOfPoints = size(points)(1);
  NC = size(centroids)(1);
  
  colors = zeros(numberOfPoints, 3);
  % get distinct colors for evry cluster
  baseColors = hsv(NC);
	% TODO graphical representation coded here 
   
  for i = 1 : numberOfPoints
      min = inf;
      clusterID = 0;
      for j = 1 : NC
        dist = norm(points(i, :) - centroids(j, :));
        
        % get the cluster id
        if(dist < min)
          min = dist;
          clusterID = j;
        end
      end
      % assign color
      colors(i, :) = baseColors(clusterID, :);
    end
    
    scatter3(points(:, 1), points(:, 2), points(:, 3), [], colors, "s", "filled");
end

