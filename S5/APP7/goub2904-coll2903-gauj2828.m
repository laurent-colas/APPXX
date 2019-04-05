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
i = 1;
while err > 0.5
h_opt = linsolve(A(1:i,1:i),b(1:i));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2)
    if err < 0.5
        if pas == 1
            i = i-pas
            break;
        else
        i = i-pas;
        err = 1;
        pas = pas/10;
        end
    end
i = i+pas
end


%% Changement de cadance
cd('C:\Users\berth\OneDrive\Documents\Git\app\S5\APP7'); 

[parole_propre,fe1] = audioread('parole_propre_12kHz.wav');
[bruit_ambiant,fe2] = audioread('bruit_ambiant_16kHz.wav');

[L,K] = rat(fe2/fe1);
x = parole_propre;
u1 = zeros(L*numel(x),1);

for i = 1:numel(x)
    u1(L*i) = x(i);
end

[b,a] = fir1(10000,1/L,'low');

u2 = filter(b,a,u1);

figure
freqz(u1)
hold on 
freqz(u2)

y = u2(1:K:end);


%% Ajout de bruit

parole = y;
bruit = bruit_ambiant(1:numel(parole));

gain_0 = sqrt((sum(parole.^2))./((10.^(0/10))*(sum(bruit.^2))));
gain_5 = sqrt((sum(parole.^2))./((10.^(5/10))*(sum(bruit.^2))));
gain_10 = sqrt((sum(parole.^2))./((10.^(10/10))*(sum(bruit.^2))));

s_0 = parole + gain_0*bruit;
s_5 = parole + gain_5*bruit;
s_10 = parole + gain_10*bruit;

RSB_0 = 10*log10(sum(parole.^2)/sum(s_0.^2));
RSB_5 = 10*log10(sum(parole.^2)/sum(s_5.^2));
RSB_10 = 10*log10(sum(parole.^2)/sum(s_10.^2));



