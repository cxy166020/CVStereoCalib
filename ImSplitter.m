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
        CalibWidth = CalibWidth/2;
        CalibHeight = CalibHeight/2;
    else
        CalibWidth = width/2;
        CalibHeight = height/2;
    end
    
    left  = imresize(pic(:, 1:width/2, :), [CalibHeight CalibWidth]);
    right = imresize(pic(:, width/2+1:end, :), [CalibHeight CalibWidth]);
    
    imwrite(left,  ['l_' im(i).name]);
    imwrite(right, ['r_' im(i).name]);
    
end