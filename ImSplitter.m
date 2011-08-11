im = dir('*.jpg');

NumOfIm = size(im, 1);


for i=1:NumOfIm
    
    pic = imread(im(i).name);
    
    [height width channel] = size(pic);
    
    left  = imresize(pic(:, 1:width/2, :), [height/2 width/2]);
    right = imresize(pic(:, width/2+1:end, :), [height/2 width/2]);
    
    imwrite(left,  ['l_' im(i).name]);
    imwrite(right, ['r_' im(i).name]);
    
end