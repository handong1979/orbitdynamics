% draw a rectangle
function [p,faces,verts] = boxpatch(x,y,z,c,R,fc)
if nargin == 0 % self test
    warning('self test');
    x = 8805;
    y = 1224.6;
    z = 1395;
    c = [0;0;0];
    R = eye(3);
    fc = 'b';
end

if nargin == 3
    c = [0;0;0];
    R = eye(3);
    fc = 'b';
elseif nargin == 4;
    R = eye(3);
    fc = 'b';
elseif nargin == 5
    fc = 'b';
end

verts = [1 -1 -1; ...
         1  1 -1; ...
        -1  1 -1; ...
        -1 -1 -1; ...
         1 -1  1; ...
         1  1  1; ...
        -1  1  1; ...
        -1 -1  1];
verts = verts.*(ones(8,1)*[x/2,y/2,z/2])*R';
verts = verts + ones(8,1)*c(:)'*R';
verts = verts;
faces = [1 2 3 4; ...
         2 3 7 6;...
         6 7 8 5;...
         5 8 4 1;...
         1 2 6 5;...
         3 4 8 7];
if nargout < 2
    p = patch('Faces',faces,'Vertices',verts,'FaceColor',fc,'facealpha',0.7);
    axis equal;
    view([30,30]);
else
    p = get(gca,'Children');
end
