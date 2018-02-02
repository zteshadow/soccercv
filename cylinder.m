
printf("hello\n");
source = imread('1.jpg');
printf("load image ok\n");
[height, width, depth] = size(I)

image = source;
centerX = width / 2;
centerY = height / 2;
% alpha = pi / 4;
f = width / (2 * tan(pi/4/2));
for i = 1 : width,
  for j = 1 : height,
    theta = asin((i - centerX) / f);
    pointX = int32(f * tan((i - centerX) / f) + centerX);
    pointY = int32((j - centerY) / cos(theta) + centerY);
    for k = 1 : depth,
      if pointX >= 1 && pointX <= width && pointY >= 1 && pointY <= height,
        image(j, i, k) = source(pointY, pointX, k);
      else
        image(j, i, k) = 0;
      end;
    end;
  end;
end;

imwrite(image, 'cylinder1.jpg');
printf("write image ok\n");
pause;

% take too much time to show 4k * 3k image
% subplot(1, 2, 1);
% imshow(source);
% subplot(1, 2, 2);
% imshow(image);
