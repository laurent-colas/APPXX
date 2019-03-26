%% cst
clearvars
close all 
clc
%% valeurs initiales
format long
f = [82.407,110.000,146.832,195.998,246.942,329.628];
fe= 8000;
dt = 1/fe;

for k = 1:numel(f)
    nb = ceil(fe/f(k));
    t = 0:dt:(nb*dt)-dt;


    %% vecteur tournant
    C_delta = exp(1j*2*pi*(f(k)/fe));
    C_deltareal(k) = cos((2*pi)*(f(k)/fe));
    C_deltaima(k) = sin((2*pi)*(f(k)/fe));
    VT(1) = 1;

    for i = 2:nb
        VT(i) = VT(i-1)*C_delta;
    end

    VT = real(VT)./sqrt(real(VT).^2+imag(VT).^2);

    %% fonction math.h
    x = 0:2*pi/nb:2*pi-2*pi/nb;
    cos_fct = cos(x);

    %% équiation différences
    teta = 2*pi*f(k)/fe;

    a1 = 2*cos(teta);
    a2 = -1;
    ED(1) = cos(teta); 
    ED(2) = cos(2*teta); 

    for i = 3:nb
        ED(i) = a1*ED(i-1) + a2*ED(i-2);
    end

    ED = [ones(1,1)',ED(1:end-1)];
    %% comparaison graphique

    err_VT = cos_fct-VT;
    err_ED = cos_fct-ED;

    figure('NAME',['Corde ' num2str(k) '(' num2str(f(k)) ' Hz)']) 
    subplot(2,1,1)
    plot(t,VT,'b--')
    hold on
    plot(t,ED,'r--')
    plot(t,cos_fct,'k')
    title(['Comparaison des génération de signaux, ' 'Corde ' num2str(k) '(' num2str(f(k)) ' Hz)' ])
    xlabel('Time (s)')
    legend('Vecteur tournant','Équation différence','Fonction librairie')
    xlim([0,t(end)]);
    grid on 
    grid minor
    subplot(2,1,2)
    plot(t,100.*err_VT)
    hold on
    plot(t,100.*err_ED,'r--')
    title('Erreur (%)')
    ylabel('Erreur(%)')
    xlabel('Time (s)')
    legend('Vecteur tournant','Équation différence')
    xlim([0,t(end)]);
    grid on 
    grid minor
    
    cd('C:\Users\berth\USherbrooke\Actif mais surtout saturé - Documents\General\s5\app6')
    saveas(gcf,['comparaison_methodes corde ' num2str(k) '.png']);
    
    clearvars -except f fe dt C_deltareal C_deltaima
end

%% Lecture fichier .dat
clc
cd('C:\Users\berth\OneDrive\Documents\Git\app\S5\APP6\S5eAPP5_Problematique\Debug'); %path du fichier .dat créé par le code .C

A=importdata('CosM2.dat');
data_c1 = str2double(A.textdata(2:99)');
data_c2 = str2double(A.textdata(102:174)');
data_c3 = str2double(A.textdata(177:231)');
data_c4 = str2double(A.textdata(234:274)');
data_c5 = str2double(A.textdata(277:309)');
data_c6 = str2double(A.textdata(312:336)');
%% Affichage pour confirmer les données généré par le .C
figure 
plot(data_c1)
hold on 
plot(data_c2)
plot(data_c3)
plot(data_c4)
plot(data_c5)
plot(data_c6)
title('Signaux généré par le code en C')
legend('Corde 1','Corde 2','Corde 3','Corde 4','Corde 5','Corde 6')