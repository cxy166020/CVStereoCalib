function ImSplitter(CalibWidth, CalibHeight)

if nargin < 2
    CalibWidth = 0;
    CalibHeight = 0;
end

im = dir('*.jpg');

NumOfIm = size(im, 1);


for i=1:NumOfIm
    
    pic = imread(im(i).name);
    
    [height, width, chanel] = size(pic); %#ok<NASGU>
    
    if CalibWidth~=0 && CalibHeight~=0
        ResizeWidth = CalibWidth/2;
        ResizeHeight = CalibHeight/2;
    else
        ResizeWidth = width/2;
        ResizeHeight = height/2;
    end
    
    left  = imresize(pic(:, 1:width/2, :), [ResizeHeight ResizeWidth]);
    right = imresize(pic(:, width/2+1:end, :), [ResizeHeight ResizeWidth]);
    
    imwrite(left,  ['l_' im(i).name]);
    imwrite(right, ['r_' im(i).name]);
    
end