% Բ׶
function mdl = cone(radius,height,c,R)
if nargin == 0
    radius = 350;
    height = 600;
    c = [0;0;-height];
    R = eye(3);%rotx(-45*rad);
end
if nargin == 3
    R = eye(3);
end

theta = 0:18:360-0.1;
x = [cosd(theta)*radius 0];
y = [sind(theta)*radius 0];
verts = [x',y',zeros(length(x),1)];
verts(end,3) = height;
len = length(verts);
verts = verts*R' + ones(len,1)*c(:)';
faces = [[1:len-1;[2:len-1,1];ones(1,len-1)*len]'; [[2:len-1,1];1:len-1;ones(1,len-1)*len]'];
patch('Faces',faces,'Vertices',verts,'FaceColor','y','EraseMode','normal','FaceAlpha',0.5,'edgealpha',0.1);

nface = size(faces,1);
norms = nan(nface,3);
for i=1:size(faces,1)
    norms(i,:) = cross(verts(faces(i,2),:)-verts(faces(i,1),:),verts(faces(i,3),:)-verts(faces(i,2),:));
    norms(i,:) = norms(i,:)/norm(norms(i,:));
end
mdl = struct('Type','Cone',...
    'Vertices',verts,'Faces',faces,...
    'Norms',norms,...
    'Reflectivity',0.8*ones(nface,1),...
    'Diffusivity',0.8*ones(nface,1),...
    'Transmissivity',zeros(size(faces,1),1));

% fp = fopen('mdl','a');
% fprintf(fp,'[cone]\n');
% fprintf(fp,'verts:\n');
% fprintf(fp,'%f\t%f\t%f\n',verts);
% fprintf(fp,'faces:\r\n');
% fprintf(fp,'%d\t%d\t%d\n',faces');
% fprintf(fp,'\n');
% fclose(fp);

if nargin == 0
    view([25 10]);
    axis equal;
%     keyboard
end