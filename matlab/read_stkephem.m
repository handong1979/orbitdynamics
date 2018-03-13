function [t,x,hdrInfo] = read_stkephem(fn)

% READ_STKEPHEM Read in STK Ephemeris (formatted ASCII text file)
%
%  [t,x,hdrInfo] = read_stkephem(fn)
%
%  Input:
%     fn          name of STK Ephemeris data file
%  Outputs:
%     t           Seconds from Epoch (1xN)
%     x           State of the satellite (6xN)
%     hdrInfo     Structure containing info from file header
%
% For a full list of acceptable keywords, see the STK help for "Ephemeris
% File Format."
% The list below shows the possible values contained in hdrInfo.
%
%   PARAMETER         DESCRIPTION
%   Num               Required: Number of Ephem Points
%   Epoch             Required: Epoch in Matlab datenum format
%   EpochStr          Required: Epoch in a string
%   CentralBody       Optional: Default = Earth
%   CoordinateSystem  Optional: Default = Fixed
%   CoordSysEpoch     Only required with coordinate systems that reference
%                       an epoch. Epoch in Matlab datenum format.
%   CoordSysEpochStr  Only required with coordinate systems that reference
%                       an epoch. Epoch in a string.
%   DistanceUnit      Optional: Default = Meters
%   Format            Required: See STK Help for descriptions
%                       Most Common = EphemerisTimePosVel
%
% (This file is part of ODTBX, The Orbit Determination Toolbox, and is
%  distributed under the NASA Open Source Agreement.  See file source for
%  more details.)

% ODTBX: Orbit Determination Toolbox
% 
% Copyright (c) 2003-2011 United States Government as represented by the
% administrator of the National Aeronautics and Space Administration. All
% Other Rights Reserved.
% 
% This file is distributed "as is", without any warranty, as part of the
% ODTBX. ODTBX is free software; you can redistribute it and/or modify it
% under the terms of the NASA Open Source Agreement, version 1.3 or later.
% 
% You should have received a copy of the NASA Open Source Agreement along
% with this program (in a file named License.txt); if not, write to the 
% NASA Goddard Space Flight Center at opensource@gsfc.nasa.gov.

%  REVISION HISTORY
%   Author      		Date         	Comment
%   Kevin Berry         12/29/2008      Original
%   Kevin Berry         06/23/2009      Added file existence check

%% Open file for reading
fid = fopen(fn,'r');
if fid==-1
    if exist(fn)
        error(['Fopen cannot open the file: ',fn])
    else
        error(['There is no file by this name in the current directory or Matlab path: ',fn])
    end
end

%% Read the header info
n=0;
check=0;
check0=0;
check1=0;
check2=0;
check3=0;
while check==0
    n = n+1;   
    tline = fgetl(fid);    
    if isempty(tline)
        % Do nothing... empty lines are ignored
    elseif findstr(tline,'NumberOfEphemerisPoints') %Required
        hdrInfo.Num=strread(tline, '%*s %n');
        check0=1;
    elseif findstr(tline,'ScenarioEpoch') %Required
        [A(1) A(2) A(3) A(4)]=strread(tline, '%*s %s %s %s %s');
        check1=1;
        hdrInfo.EpochStr = [A{1} ' ' A{2} ' ' A{3} ' ' A{4}];
        hdrInfo.Epoch = datenum(hdrInfo.EpochStr);
    elseif findstr(tline,'CentralBody') %Not Required. Default = Earth
        [A(1)]=strread(tline, '%*s %s');
        hdrInfo.CentralBody = A{1};
    elseif findstr(tline,'CoordinateSystem') %Not Required. Default = Fixed
        [A(1)]=strread(tline, '%*s %s');
        hdrInfo.CoordinateSystem = A{1};        
    elseif findstr(tline,'CoordinateSystemEpoch') %Only required with coordinate systems that reference an epoch.
        [A(1) A(2) A(3) A(4)]=strread(tline, '%*s %s %s %s %s');
        hdrInfo.CoordSysEpochStr = [A{1} ' ' A{2} ' ' A{3} ' ' A{4}];
        hdrInfo.CoordSysEpoch = datenum(hdrInfo.CoordSysEpochStr);
    elseif findstr(tline,'DistanceUnit') %Not Required. Default = Meters
        [A(1)]=strread(tline, '%*s %s');
        hdrInfo.DistanceUnit = A{1};
    elseif strncmpi(tline, 'Ephemeris', 9) %If the first 9 letters are Ephemeris
        [A(1)]=strread(tline, '%s');
        check2=1;
        hdrInfo.Format = A{1};
    elseif ~isempty(str2num(tline)) %the line is all numeric
        if check2 %The format must be given before the data can begin
            hlines = n-1;
            check3=1;
        end
    end
    
    check=check0*check1*check2*check3;
    if n>200
        if check0==0
            error(['Could not find the number of points of ' fn ' within the first 200 lines'])
        end        
        if check1==0
            error(['Could not find the epoch of ' fn ' within the first 200 lines'])
        end
        if check2==0
            error(['Could not find the format of ' fn ' within the first 200 lines'])
        end
        if check3==0
            error(['Could not find the starting line for the ephem data of ' fn ' within the first 200 lines'])
        end
    end
end

%% Close the file
fclose(fid);

%% Read in the data for correct format
if strcmpi(hdrInfo.Format, 'EphemerisTimePos')
    [t(1,:), x(1,:), x(2,:), x(3,:)] = textread(fn,...
        '%n %n %n %n',hdrInfo.Num,'headerlines',hlines);
elseif strcmpi(hdrInfo.Format, 'EphemerisTimePosVel')
    [t(1,:), x(1,:), x(2,:), x(3,:), x(4,:), x(5,:), x(6,:)] = textread(fn,...
        '%n %n %n %n %n %n %n',hdrInfo.Num,'headerlines',hlines);
elseif strcmpi(hdrInfo.Format, 'EphemerisLLATimePos')
    [t(1,:), x(1,:), x(2,:), x(3,:)] = textread(fn,...
        '%n %n %n %n',hdrInfo.Num,'headerlines',hlines);    
elseif strcmpi(hdrInfo.Format, 'EphemerisLLATimePosVel')
    [t(1,:), x(1,:), x(2,:), x(3,:), x(4,:), x(5,:), x(6,:)] = textread(fn,...
        '%n %n %n %n %n %n %n',hdrInfo.Num,'headerlines',hlines);
elseif strcmpi(hdrInfo.Format, 'EphemerisLLRTimePos')
    [t(1,:), x(1,:), x(2,:), x(3,:)] = textread(fn,...
        '%n %n %n %n',hdrInfo.Num,'headerlines',hlines);    
elseif strcmpi(hdrInfo.Format, 'EphemerisLLRTimePosVel')
    [t(1,:), x(1,:), x(2,:), x(3,:), x(4,:), x(5,:), x(6,:)] = textread(fn,...
        '%n %n %n %n %n %n %n',hdrInfo.Num,'headerlines',hlines);    
elseif strcmpi(hdrInfo.Format, 'EphemerisGeocentricLLATimePos')
    [t(1,:), x(1,:), x(2,:), x(3,:)] = textread(fn,...
        '%n %n %n %n',hdrInfo.Num,'headerlines',hlines);    
elseif strcmpi(hdrInfo.Format, 'EphemerisGeocentricLLATimePosVel')
    [t(1,:), x(1,:), x(2,:), x(3,:), x(4,:), x(5,:), x(6,:)] = textread(fn,...
        '%n %n %n %n %n %n %n',hdrInfo.Num,'headerlines',hlines);   
elseif strcmpi(hdrInfo.Format, 'EphemerisGeodeticLLRTimePos')
    [t(1,:), x(1,:), x(2,:), x(3,:)] = textread(fn,...
        '%n %n %n %n',hdrInfo.Num,'headerlines',hlines);    
elseif strcmpi(hdrInfo.Format, 'EphemerisGeodeticLLRTimePosVel')
    [t(1,:), x(1,:), x(2,:), x(3,:), x(4,:), x(5,:), x(6,:)] = textread(fn,...
        '%n %n %n %n %n %n %n',hdrInfo.Num,'headerlines',hlines);
end

