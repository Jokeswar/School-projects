function view_cost_vs_nc(file_points)
	% TODO compute cost for NC = [1..10] and plot 
  
  points = load(file_points);
  
  % conversion to matrix
  points = struct2cell(points);
  points = cell2mat(points);
  
  cost = zeros(10, 1);
  x = [ 1 : 10 ];
  
  for i = 1 : 10
    % get centroids
    centroids = clustering_pc(points, i);
    % calculate the cost
    cost(i) = compute_cost_pc(points, centroids);
  end
  
  % plot the values
  plot(x, cost);
  xlabel("Number of centroids");
  ylabel("Cluster Cost")
end

