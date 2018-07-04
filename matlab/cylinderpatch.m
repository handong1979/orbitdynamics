% 圆柱
function mdl = cylinderpatch(radius,height,c,R,bt)
if nargin == 0
    warning('self test');
    radius = 50;
    height = 200;
    c = [0;0;0];
    R = eye(3);
    bt = 1;
end

if nargin == 4
    bt = 1;
end
if nargin == 3
    R = eye(3);
    bt = 1;
end

theta = 0:15:360-1;
x = cosd(theta)*radius;
y = sind(theta)*radius;
verts = [x' y' ones(length(x),1)*height/2;
         x' y' ones(length(x),1)*-height/2];
len = length(verts);
verts = verts*R' + ones(len,1)*c(:)';
faces = [1:24;[2:24 1];[(2:24)+24 25];(1:24)+24]';
faces(1:24,5:24) = nan;
if bt == 1
    faces(25,1:24) = 1:24;
    faces(26,1:24) = 25:48;
end
patch('Faces',faces,'Vertices',verts,'FaceColor','w','EdgeColor','g','EraseMode','normal');

nface = size(faces,1);
norms = nan(nface,3);
for i=1:size(faces,1)
    norms(i,:) = vcross(verts(faces(i,2),:)-verts(faces(i,1),:),verts(faces(i,3),:)-verts(faces(i,2),:));
    norms(i,:) = norms(i,:)/norm(norms(i,:));
end
mdl = struct('Type','Cylinder',...
    'Vertices',verts,'Faces',faces,...
    'Norms',norms,...
    'Reflectivity',0.9*ones(nface,1),...
    'Diffusivity',0.5*ones(nface,1),...
    'Transmissivity',zeros(size(faces,1),1));


% fp = fopen('mdl','a');
% fprintf(fp,'[cylinder]\n');
% fprintf(fp,'verts:\n');
% fprintf(fp,'%f\t%f\t%f\n',verts);
% fprintf(fp,'faces:\r\n');
% fprintf(fp,'%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n',faces');
% fprintf(fp,'\n');
% fclose(fp);