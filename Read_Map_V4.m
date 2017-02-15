clear; clc;
% NOTE: Make sure path file and map file in same place
% Selecting path file, with all information
[p_filename,p_pathname] = uigetfile('*.*','Select a path file.');
% Open path file
pathFile = fopen([p_pathname,p_filename]);
path = textscan(pathFile, '%s');
fclose(pathFile);

% Chop up path file name
mapNum = p_filename(6);
sg_pair = p_filename(8);
sgNum = str2double(sg_pair);
w_char = p_filename(10:13);
weight = str2double(p_filename(10:13));

% Load map file
mapPath = [p_pathname,'Map_',mapNum,'.txt'];
mapFile = fopen(mapPath);
map = textscan(mapFile, '%s');
fclose(mapFile);

% Get map data
disp('Loading map...');
MAP = cell2mat(map{1}(57:end));
m_pic = zeros(size(MAP,1),size(MAP,2),3);
f_pic = zeros(size(MAP,1),size(MAP,2),3);

disp('Loading start-goal pair...');
x_s = str2double(cell2mat(map{1}(1+sgNum*4)));
y_s = str2double(cell2mat(map{1}(2+sgNum*4)));
x_g = str2double(cell2mat(map{1}(3+sgNum*4)));
y_g = str2double(cell2mat(map{1}(4+sgNum*4)));
MAP(x_s,y_s) = 'x';
MAP(x_g,y_g) = 'x';

disp('Adding path properties...');
PATH = zeros(size(MAP,1),size(MAP,2),3); % f,g,h
for i = 1:size(path{1},1)/5
    x = path{1}(1+5*(i-1)); % x-value
    y = path{1}(2+5*(i-1)); % y-value
    f = path{1}(3+5*(i-1)); % f-value
    g = path{1}(4+5*(i-1)); % g-value
    h = path{1}(5+5*(i-1)); % g-value
    x = str2double(x{1})+1;
    y = str2double(y{1})+1;
    MAP(x,y) = 'x';
    PATH(x,y,1) = str2double(f{1});
    PATH(x,y,2) = str2double(g{1});
    PATH(x,y,3) = str2double(h{1});
end

% Open fringe file
o_filename = ['openlist_', mapNum, '_', sg_pair,'_',w_char,'0000','.txt'];
openlineFile = fopen([p_pathname,o_filename]);
openline = textscan(openlineFile, '%s');
fclose(openlineFile);
MAP_f = MAP;

for i = 1:size(openline{1},1)/5
    x = openline{1}(1+5*(i-1)); % x-value
    y = openline{1}(2+5*(i-1)); % y-value
    f = openline{1}(3+5*(i-1)); % f-value
    g = openline{1}(4+5*(i-1)); % g-value
    h = openline{1}(5+5*(i-1)); % h-value
    x = str2double(x{1})+1;
    y = str2double(y{1})+1;
    MAP_f(x,y) = 'm';
    PATH(x,y,1) = str2double(f{1});
    PATH(x,y,2) = str2double(g{1});
    PATH(x,y,3) = str2double(h{1});
end

% Open fringe file
f_filename = ['fringe_', mapNum, '_', sg_pair,'_',w_char,'0000','.txt'];
fringeFile = fopen([p_pathname,f_filename]);
fringe = textscan(fringeFile, '%s');
fclose(fringeFile);


for i = 1:size(fringe{1},1)/5
    x = fringe{1}(1+5*(i-1)); % x-value
    y = fringe{1}(2+5*(i-1)); % y-value
    f = fringe{1}(3+5*(i-1)); % f-value
    g = fringe{1}(4+5*(i-1)); % g-value
    h = fringe{1}(5+5*(i-1)); % h-value
    x = str2double(x{1})+1;
    y = str2double(y{1})+1;
    MAP_f(x,y) = 'f';
    PATH(x,y,1) = str2double(f{1});
    PATH(x,y,2) = str2double(g{1});
    PATH(x,y,3) = str2double(h{1});
end



f_pic = addColors(f_pic,MAP_f);
m_pic = addColors(m_pic,MAP);

ui = input('Would you like to select points? (y/n): ','s');
fringe_ui = input('Would you like to see the fringe? (y/n): ','s');
if(ui == 'y' && fringe_ui == 'n')
    imshow(m_pic);
    while(1)
        [y,x] = ginput(1);
        x = floor(abs(x))+1;
        y = floor(abs(y))+1;
        if(x > 120 || y > 160 || x < 1 || y < 1)
            disp('Point out of bounds, select again');
        else
        fprintf('Coordinates: (%3.0f,%3.0f) \n',x-1,y-1);
        fprintf('f(n): %3.2f \n',PATH(x,y,1));
        fprintf('g(n): %3.2f \n',PATH(x,y,2));
        fprintf('h(n): %3.2f \n',PATH(x,y,3));
        end
    end

elseif (ui == 'y' && fringe_ui == 'y')
     imshow(f_pic);
    while(1)
        [y,x] = ginput(1);
        x = floor(abs(x))+1;
        y = floor(abs(y))+1;
        if(x > 120 || y > 160 || x < 1 || y < 1)
            disp('Point out of bounds, select again');
        else
        fprintf('Coordinates: (%3.0f,%3.0f) \n',x-1,y-1);
        fprintf('f(n): %3.2f \n',PATH(x,y,1));
        fprintf('g(n): %3.2f \n',PATH(x,y,2));
        fprintf('h(n): %3.2f \n',PATH(x,y,3));
        end
    end
    
else
    imshow(m_pic);
    disp('Okay!');
end









