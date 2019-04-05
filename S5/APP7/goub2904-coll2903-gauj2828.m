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

m = 10000; % Longueur de la r�ponse impulsionnelle

Rxx = xcorr(bruit_emis, bruit_emis, m); % Valeurs d�autocorr�lation pour la matrice A
Rxy = xcorr(bruit_emis,bruit_capte, m); % Valeurs d�autocorr�lation pour la matrice b

A = toeplitz(Rxx(m+1:end));             % Cr�ation de la matrice de Toeplitz A
b = flip(Rxy(1:m+1));                   % Cr�ation de la matrice b


%% Optimisation
h_opt = linsolve(A(1:m,1:m),b(1:m));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2);
%%
pas = 1000;
i = m-pas;
while err > 0.5
h_opt = linsolve(A(1:i,1:i),b(1:i));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2)

    if err < 0.5
        if pas == 1
            break;
        else
        i = i+pas;
        err = 1;
        pas = pas/10;
        end
    end
i = i-pas
end


%%
figure
plot(h_bert(1:numel(h_opt)))
hold on 
plot(normal(h_opt))
plot(abs(h_bert(1:numel(h_opt))-normal(h_opt)))


