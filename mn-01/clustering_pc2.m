function centroids = clustering_pc2(points, NC)
  
	centroids = [];
 %1
  rand_idx= randperm(size(points, 1));
  centroids1= points(rand_idx(1:NC), :);
 %2 

  indice=zeros(size(points,1),1);
  centroids=zeros(NC,3);
  while (centroids != centroids1)
    centroids = centroids1;
    [linii coloane]= size(points);
  
    for i = 1:linii
      idx = 1;
      val_min =norm(points(1,:) - centroids(1,:));
      for j = 1:NC
        if norm(points(i,:) - centroids(j,:)) < val_min
          val_min = norm(points(i,:) - centroids(j,:));
          idx = j;
        endif
      endfor
      indice(i)=idx;  
    endfor
    clst=zeros(linii, 1);
    for i = 1:NC
      clst=find(indice == i);

      if(length(clst) != 0)
        centeroids1(i, :) = mean(points(find(clst), :));
      end
      %centroids1(i,1)=mean(points(clst,1));
      %centroids1(i,2)=mean(points(clst,2));  
      %centroids1(i,3)=mean(points(clst,3));
    endfor
endwhile   
centroids=centroids1;
end