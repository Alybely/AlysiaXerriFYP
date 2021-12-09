clc;
clear;
close all;

filename = 'testingData.xlsx';

sheet2 = 2;
sheet3 = 3;
sheet4 = 4;
sheet5 = 5;
sheet6 = 6;

xlRange = 'A1:A400';

[num1,txt1,raw1] = xlsread(filename,sheet2,xlRange);
[num2,txt2,raw2] = xlsread(filename,sheet3,xlRange);
[num3,txt3,raw3] = xlsread(filename,sheet4,xlRange);
[num4,txt4,raw4] = xlsread(filename,sheet5,xlRange);
[num5,txt5,raw5] = xlsread(filename,sheet6,xlRange);

AllData1={txt1;num1};
AllData2={txt2;num2};
AllData3={txt3;num3};
AllData4={txt4;num4};
AllData5={txt5;num5};

matfileName1 = 'matfile1';
matfileName2 = 'matfile2';
matfileName3 = 'matfile3';
matfileName4 = 'matfile4';
matfileName5 = 'matfile5';

save(matfileName1,'AllData1');
save(matfileName2,'AllData2');
save(matfileName3,'AllData3');
save(matfileName4,'AllData4');
save(matfileName5,'AllData5');

mean1 = mean(num1);
mean2 = mean(num2);
mean3 = mean(num3);
mean4 = mean(num4);
mean5 = mean(num5);

d1 = 1.14703;
d2 = 1.2126;
d3 = 1.30373;
d4 = 1.94522;
d5 = 2.19172;

hm = 2;
hb = 32;
f = 806;
pt = 20;
ag = 16;

cf1 = (3.2*(log10(11.75*hm))^2) - 4.97;
hata1 = 69.55 + (26.16*log10(f)) - (13.82 * log10(hb)) - cf1 + (44.9 - (6.55 * log10(hb)))*log10(d1);
rssi_hata1 = pt + ag - hata1;
rssi_hata1_2 = pt + ag - hata1 + (-2*(log10(f/28))^2 - 5.4);
rssi_hata1_3 = pt + ag - hata1 + (-4.78*(log10(f))^2 + (18.33*log10(f)) - 40.98);

cf2 = (3.2*(log10(11.75*hm))^2) - 4.97;
hata2 = 69.55 + (26.16*log10(f)) - (13.82 * log10(hb)) - cf2 + (44.9 - (6.55 * log10(hb)))*log10(d2);
rssi_hata2 = pt + ag - hata2;
rssi_hata2_2 = pt + ag - hata2 + (-2*(log10(f/28))^2 - 5.4);
rssi_hata2_3 = pt + ag - hata2 + (-4.78*(log10(f))^2 + (18.33*log10(f)) - 40.98);

cf3 = (3.2*(log10(11.75*hm))^2) - 4.97;
hata3 = 69.55 + (26.16*log10(f)) - (13.82 * log10(hb)) - cf3 + (44.9 - (6.55 * log10(hb)))*log10(d3);
rssi_hata3 = pt + ag - hata3;
rssi_hata3_2 = pt + ag - hata3 + (-2*(log10(f/28))^2 - 5.4);
rssi_hata3_3 = pt + ag - hata3 + (-4.78*(log10(f))^2 + (18.33*log10(f)) - 40.98);

cf4 = (3.2*(log10(11.75*hm))^2) - 4.97;
hata4 = 69.55 + (26.16*log10(f)) - (13.82 * log10(hb)) - cf4 + (44.9 - (6.55 * log10(hb)))*log10(d4);
rssi_hata4 = pt + ag - hata4;
rssi_hata4_2 = pt + ag - hata4 + (-2*(log10(f/28))^2 - 5.4);
rssi_hata4_3 = pt + ag - hata4 + (-4.78*(log10(f))^2 + (18.33*log10(f)) - 40.98);

cf5 = (3.2*(log10(11.75*hm))^2) - 4.97;
hata5 = 69.55 + (26.16*log10(f)) - (13.82 * log10(hb)) - cf5 + (44.9 - (6.55 * log10(hb)))*log10(d5);
rssi_hata5 = pt + ag - hata5;
rssi_hata5_2 = pt + ag - hata5 + (-2*(log10(f/28))^2 - 5.4);
rssi_hata5_3 = pt + ag - hata5 + (-4.78*(log10(f))^2 + (18.33*log10(f)) - 40.98);

gradient = (30-1)/(-53+111);

rssi1 = -55 + (mean1-29)/gradient;
rssi2 = -55 + (mean2-29)/gradient;
rssi3 = -55 + (mean3-29)/gradient;
rssi4 = -55 + (mean4-29)/gradient;
rssi5 = -55 + (mean5-29)/gradient;

Y1 = [rssi1, rssi2, rssi3, rssi4, rssi5];
Y2 = [rssi_hata1, rssi_hata2, rssi_hata3, rssi_hata4, rssi_hata5];
Y3 = [rssi_hata1_2, rssi_hata2_2, rssi_hata3_2, rssi_hata4_2, rssi_hata5_2];
Y4 = [rssi_hata1_3, rssi_hata2_3, rssi_hata3_3, rssi_hata4_3, rssi_hata5_3];

X = [d1, d2, d3, d4, d5];
figure;
plot(X, Y1, 'b-x', X, Y2, 'm-x', 'LineWidth', 1.5);
title('Comparison of Theoretical vs Actual RSS Values: Large City Model');
xlabel('Distance (km)');
ylabel('RSSI (dBm)');
legend('Actual', 'Large City Model');

figure;
plot(X, Y1, 'b-x', X, Y3, 'y-x', 'LineWidth', 1.5);
title('Comparison of Theoretical vs Actual RSS Values: Suburban Environment');
xlabel('Distance (km)');
ylabel('RSSI (dBm)');
legend('Actual', 'Suburban Model');

figure;
plot(X, Y1, 'b-x', X, Y4, 'r-x', 'LineWidth', 1.5);
title('Comparison of Theoretical vs Actual RSS Values: Rural Environment');
xlabel('Distance (km)');
ylabel('RSSI (dBm)');
legend('Actual', 'Rural Model');

figure;
plot(X, Y1, 'b-x', X, Y2, 'm-x', X, Y3, 'y-x', X, Y4, 'r-x', 'LineWidth', 1.5);
title('Comparison of Theoretical vs Actual RSS Values');
xlabel('Distance (km)');
ylabel('RSSI (dBm)');
legend('Actual', 'Large City', 'Suburban', 'Rural');




figure;
plot(num1, '-r', 'LineWidth',1.5);
ylim([0 18]);
title('Readings From Site 1');
xlabel('Number of Readings');
ylabel('Signal Strength');

figure;
plot(num2, '-r', 'LineWidth',1.5);
ylim([0 18]);
title('Readings From Site 2');
xlabel('Number of Readings');
ylabel('Signal Strength');

figure;
plot(num3, '-r', 'LineWidth',1.5);
ylim([0 18]);
title('Readings From Site 3');
xlabel('Number of Readings');
ylabel('Signal Strength');

figure;
plot(num4, '-r', 'LineWidth',1.5);
ylim([0 18]);
title('Readings From Site 4');
xlabel('Number of Readings');
ylabel('Signal Strength');

figure;
plot(num5, '-r', 'LineWidth',1.5);
ylim([0 18]);
title('Readings From Site 5');
xlabel('Number of Readings');
ylabel('Signal Strength');

latency1 = 0.518;
latency2 = 0.583;
latency3 = 0.712;
latency4 = 0.870;
latency5 = 0.961;

Z1 = [latency1, latency2, latency3, latency4, latency5];
X = [d1, d2, d3, d4, d5];
figure;
plot(X, Z1, 'g-x', 'LineWidth',1.5);

title('Latency With Respect to Distance');
xlabel('Distance (km)');
ylabel('Latency (ms)');


error = [11.51 23.5 46.37; 1.7 12.54 33.2; 6.35 3.51 22.31; 5.4 3.92 21.74; 7.86 1.07 18.11];

figure;

b = bar(error);
title('% Error Between Acutal and Predicted RSS Readings');
xlabel('Sites');
ylabel('% Error');
b(1).FaceColor = 'm';
b(2).FaceColor = 'y';
b(3).FaceColor = 'r';
legend('Large City', 'Suburban', 'Rural');

