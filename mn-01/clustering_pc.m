% computes the NC centroids corresponding to the given points using K-Means
function centroids = clustering_pc(points, NC)
	centroids = zeros(NC, 3);
  numberOfPoints = size(points)(1);
  
  groups = zeros(numberOfPoints, 1);
  
  % TODO K-Means code 
  
  % get random indexes so we can choose points as centroids
  [jk randomNumbers] = sort(rand(numberOfPoints, 1));
  
  % NC << number of points
  for i = 1 : NC
    centroids(i, :) = points(randomNumbers(i), :);
  end

  while(1)
    newCentroids = zeros(NC, 3);
  
    for i = 1 : numberOfPoints
      min = inf;
      for j = 1 : NC
        % calculate the distance
        dist = norm(points(i, :) - centroids(j, :));
        
        % add the point to the correct cluster
        if(dist < min)
          min = dist;
          groups(i, 1) = j;
        end
      end
    end
    
    for i = 1 : NC
      groupMean = zeros(1, 3);
      count = 0;
      
      for j = 1 : numberOfPoints
        if(groups(j, 1) == i)
          % add the value of all points from the j cluster
          groupMean(1, :) = groupMean(1, :) + points(j, :);
          count = count + 1;
        end
      end
        
      % divide by how many there are to get the mean
      if(count != 0)
        newCentroids(i, :) = groupMean(1, :)/count;
      else
         newCentroids(i, :) = centroids(i, :);
      end
    end
    
    % if the old centroids differ from the new ones continue
    % else we found the correct centroid and break 
    if(centroids == newCentroids)
      break;
    else
      centroids = newCentroids;
    end
    
  end
  
end
