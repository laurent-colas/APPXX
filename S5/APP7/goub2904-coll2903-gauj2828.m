%% clear
clearvars
close all 
clc

%% Files 

cd('C:\Users\berth\OneDrive\Documents\Git\app\S5\APP7');

[bruit_emis,Fs] = audioread('bruit_emis_32kHz.wav');
[bruit_capte,Fs] = audioread('bruit_capte_32kHz.wav');

h_bert = whitenoiseID(bruit_emis,bruit_capte,Fs,Fs); 
%% Trouver Reponse Impulsionnelle

m = 10000; % Longueur de la réponse impulsionnelle

Rxx = xcorr(bruit_emis, bruit_emis, m); % Valeurs d’autocorrélation pour la matrice A
Rxy = xcorr(bruit_emis,bruit_capte, m); % Valeurs d’autocorrélation pour la matrice b

A = toeplitz(Rxx(m+1:end));             % Création de la matrice de Toeplitz A
b = flip(Rxy(1:m+1));                   % Création de la matrice b



%% Optimisation
h_opt = linsolve(A(1:m,1:m),b(1:m));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2);
%%
i = m-10;
while err > 0.5
h_opt = linsolve(A(1:i,1:i),b(1:i));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2)
i = i-10
end

%%
figure
plot(h_bert)
hold on 
plot(normal(h_opt))

