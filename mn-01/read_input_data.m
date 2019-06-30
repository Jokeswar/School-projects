% reads cluster count and points from input files 
function [NC points] = read_input_data(file_params, file_points)
	
	% set these values correctly
	NC = 0;
	points = [];
  
  format long;
  
	% TODO read NC
  NC = load(file_params);
  
	% TODO read points
  points = load(file_points);
  
  % conversion to matrix
  points = struct2cell(points);
  points = cell2mat(points);
  
end

