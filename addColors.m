function [inPic] = addColors(inPic,MAP)

for row = 1:size(MAP,1)
    for col = 1:size(MAP,2)
        cel = MAP(row,col);
        switch cel
            case '1' % Unblocked
                inPic(row,col,:) = [255,255,255]; % White
            case '0' % Blocked
                inPic(row,col,:) = [0,0,0]; % Black
            case '2' % Hard to traverse
                inPic(row,col,:) = [255,0,0]; % Red
            case 'a' % Highway on unblocked
                inPic(row,col,:) = [0,0,255]; % Blue
            case 'b' % Highway on hard to traverse
                inPic(row,col,:) = [255,0,255]; % Purple
            case 'x' % Start or goal cell
                inPic(row,col,:) = [0,255,0];
            case 'f' % fringe cell
                inPic(row,col,:) = [255,255,0]; % Yellow
            case 'm' % open cell
                inPic(row,col,:) = [0,255,255]; % Light blue
        end
        
    end
end



end