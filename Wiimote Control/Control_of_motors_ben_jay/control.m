%% Introduction
%**************************************************************************
%                                                                          
% Script: control.m                   Orgin Programmer: Ben Brand       
%                                                                          
% Description:                                                             
%   This is the M file for balencing roll pitch and yaw of RoboSub. It also
%   takes in the control values from the computer for depth, heading, and
%   speed. 
%
% Functions Called:
%   
%   direction.m - inputs are the three waveforms and the coords of the
%   hydrophones, output is the direction of the acoustic source
%
% Modification History:
%
% Ben - 8 October 2011, Start. Code does not have any communication
% protocals, just variables and code flow for calculations. This code is in
% MatLab for now, incase I do some simulations, but will be perfectly
% capable of running on an Arduino Mego in the actual implementation.
%
%**************************************************************************

clear all;  % clear the workspace
close all;  % close figures
clc;        % clear the command window

%% Variables

Thr1Spd = 0  % Thruster Speed
Thr2Spd = 0  % Thruster Speed
Thr3Spd = 0  % Thruster Speed
Thr4Spd = 0  % Thruster Speed
Thr5Spd = 0  % Thruster Speed

KXp = 1      % x axis porportional constant (accelerometer)
KYp = 1      % y axis porportional constant (accelerometer)
KZp = 1      % z axis porportional constant (compas)

KXd = 0      % x axis derivative constant (gyroscope)
KYd = 0      % y axis derivative constant (gyroscope)
KZd = 0      % z axis derivative constant (gyroscope)

%% Main Code

while (1) %this means forever <-- Joe
    %% Read Hardware
    
    % This section will read in Accelerometer, Gyroscope, Compass data.
    % Communication protocall unknown.
    
    %% Computer Control
    
    % This section will read values for direction and speed control from
    % the computer. Values read will be:
    
    % Speed
    % Depth
    % Heading
    
    % Communication protocall unknown.
    
    %% Convertions
    
    % This section will take data from "%% Read Hardware" and "%% Computer
    % Control" and do the neccessary x y z convertions and scalings. The
    % variables created will be:
    
    % RollPos  % x axis (accelerometer)
    % PitchPos % y axis (accelerometer)
    % YawPos   % z axis (compas)
    
    % RollSpd  % x axis (gyroscope)
    % PitchSpd % y axis (gyroscope)
    % YawSpd   % z axis (gyroscope)
    
    %% Pitch and Roll
    
    Thr1Spd = -PitchPos*XKp - PitchSpd*XKd
    Thr2Spd = .5*(PitchPos*YKp + PitchSpd*XKd) - RollPos*YKp - RollSpd*YKd
    Thr2Spd = .5*(PitchPos*YKp + PitchSpd*XKd) + RollPos*YKp + RollSpd*YKd
        
    %% Depth and Heading Control
    
    Thr1Spd = Thr1Spd + Depth
    Thr2Spd = Thr2Spd + Depth
    Thr3Spd = Thr3Spd + Depth
    
    YawPos = YawPos - Heading
    
    %% Yaw
    
    Thr4Spd = -YawPos*ZKp - YawSpd*ZKd
    Thr5Spd = -Thr4Spd
    
    %% Y Speed
    
    Thr4Spd = Thr4Spd + Speed
    Thr5Spd = Thr5Spd + Speed
    
    %% Output
    
    % This section will output the five motor speeds to the motor
    % controllers. Conunnication protocall unknown.
        
end
    
    