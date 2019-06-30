% computes a clustering solution total cost
function cost = compute_cost_pc(points, centroids)
	cost = 0; 
  numberOfPoints = size(points)(1);
  NC = size(centroids)(1);
  
	% TODO compute clustering solution cost
  for i = 1 : numberOfPoints
    min = inf;
    for j = 1 : NC
      % calculate distance to centroid j
      dist = norm(points(i, :) - centroids(j, :));
      
      % get the minimum value
      if(dist < min)
        min = dist;
      end
    end
    % increase the cost
    cost = cost + min;
  end
  
end

