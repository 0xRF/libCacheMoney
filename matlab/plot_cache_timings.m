% 0xRF

SAMPLES = 10000;
SET_SIZE = 64;
SET_COUNT = 64;
ASSOC = 8;
DATA = zeros(SAMPLES, ASSOC, SET_COUNT);

AVERAGE = zeros(ASSOC, SET_COUNT);

for sample=1:SAMPLES
    name = "samples/sample_" + (sample-1) + ".dat";
    data = dlmread(name, ' ');
    for j=1:ASSOC
        for k=1:SET_COUNT
            DATA(sample, j,k) = data(j,k);
        end
    end
end

OUT = zeros(ASSOC, SET_COUNT);
for j=1:ASSOC
    for k=1:SET_COUNT
        OUT(j,k) = mean(DATA(:, j,k));
    end
end

figure;
maxval= max(OUT(:));
img = uint8((double(OUT) ./ maxval) .* 255);
image(img);
ylabel('Assoc Block');
xlabel('Cache Set');
colormap(gray(255));
