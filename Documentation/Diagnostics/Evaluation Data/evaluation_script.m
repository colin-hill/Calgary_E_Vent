clear
clc

dataFilePath = 'C:\Users\ColinHill\Documents\GitHub\Calgary_E_Vent\Documentation\Diagnostics\Evaluation Data\VC_mode_26_bpm.txt';



fileID = fopen(dataFilePath);

tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);
tline = fgetl(fileID);

i = 1;

%unit1 = zeros(5e06, 1);
%unit2 = zeros(5e06, 1);
%unit3 = zeros(5e06, 1);
%unit4 = zeros(5e06, 1);
%unit5 = zeros(5e06, 1);


while ischar(tline)
    
    if mod(i,100) == 0
        disp(i)
    end
    
    thisLineData = strsplit(tline, ':');
    
    unit1(i) = GetSpecifiedValue(thisLineData, 6);
    unit2(i) = GetSpecifiedValue(thisLineData, 7);
    unit3(i) = GetSpecifiedValue(thisLineData, 8);
    unit4(i) = GetSpecifiedValue(thisLineData, 9);
    unit5(i) = GetSpecifiedValue(thisLineData, 10);
    
    tline = fgetl(fileID);
    i = i + 1;
end

fclose(fileID);

time = 0:length(unit1)-1;
time = time./13.5;

plot(time, unit1)

function value = GetSpecifiedValue(inputCellRow, index)

    thisCell = inputCellRow(index);
    splitCell = strsplit(thisCell{1}, ',');
    value = str2double(splitCell{1});  
    
end