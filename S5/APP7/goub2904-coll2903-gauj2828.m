%% clear
clearvars
close all 
clc

%% Trouver Reponse Impulsionnelle
cd('C:\Users\berth\OneDrive\Documents\Git\app\S5\APP7');

% signaux de départ
[bruit_emis,Fs] = audioread('bruit_emis_32kHz.wav');
[bruit_capte,Fs] = audioread('bruit_capte_32kHz.wav');

m = 10000; % Longueur de la réponse impulsionnelle

Rxx = xcorr(bruit_emis, bruit_emis, m); % Valeurs d’autocorrélation pour la matrice A
Rxy = xcorr(bruit_emis,bruit_capte, m); % Valeurs d’autocorrélation pour la matrice b

A = toeplitz(Rxx(m+1:end));             % Création de la matrice de Toeplitz A
b = flip(Rxy(1:m+1));                   % Création de la matrice b


% Résolution du système matriciel 
h_opt = linsolve(A(1:m,1:m),b(1:m));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2);

% Boucle pour trouver rapidement la longueur minimale pour une erreur de < 0.5
pas = 1000;
i = 1;
while err > 0.5
h_opt = linsolve(A(1:i,1:i),b(1:i));  
bruit_estime = filter(h_opt,[1,0],bruit_emis);
err = sum((bruit_estime - bruit_capte).^2)
    if err < 0.5
        if pas == 1
            i = i-pas
            h_opt = linsolve(A(1:i,1:i),b(1:i));  
            break;
        else
        i = i-pas;
        err = 1;
        pas = pas/10;
        end
    end
i = i+pas
end

%% Vérification 
[parole_emise,Fs] = audioread('parole_emise_32khz.wav');
[parole_captee,Fs] = audioread('parole_captee_32khz.wav');

parole_estime = filter(h_opt,[1,0],parole_emise);

err_parole = sum((parole_captee - parole_estime).^2)

figure 
plot(parole_captee)
hold on 
plot(parole_estime)
plot(abs(parole_captee - parole_estime).^2)
grid on 
grid minor
legend('parole captee','parole estime','|Erreur^2|')



%% Changement de cadence
cd('C:\Users\berth\OneDrive\Documents\Git\app\S5\APP7'); 

% Signaux de départ
[parole_propre,fe1] = audioread('parole_propre_12kHz.wav');
[bruit_ambiant,fe2] = audioread('bruit_ambiant_16kHz.wav');
[L,K] = rat(fe2/fe1);
x = parole_propre;
u1 = zeros(L*numel(x),1);

% Ajout de L 0 entre chaque échantillons et gain de L
for i = 1:numel(x)
    u1(L*i) = L*x(i);
end

% Filter à pi/L (pi/L > pi/k)
[b,a] = fir1(10000,1/L,'low');
u2 = filter(b,a,u1);

% Garder une données sur K
y = u2(1:K:end);

% Vérification
player_parole_propre = audioplayer(parole_propre,12000);
player_new_SR = audioplayer(y,16000);

play(player_new_SR)
play(player_parole_propre)
%% Ajout de bruit

% Signaux de départ
parole = y;
bruit = bruit_ambiant(1:numel(parole));
fs = 16000;
T = 1/fs;
time = 0:T:(T*numel(parole))-T;
time =time';

% Calcul du gain 
gain_0 = sqrt((sum(parole.^2))./((10.^(-0/10))*(sum(bruit.^2))));
gain_5 = sqrt((sum(parole.^2))./((10.^(-5/10))*(sum(bruit.^2))));
gain_10 = sqrt((sum(parole.^2))./((10.^(-10/10))*(sum(bruit.^2))));

% Vérification
RSB_0 = -10*log10(sum(parole.^2)/sum((gain_0.*bruit).^2));
RSB_5 = -10*log10(sum(parole.^2)/sum((gain_5.*bruit).^2));
RSB_10 = -10*log10(sum(parole.^2)/sum((gain_10.*bruit).^2));

% Signaux bruités
s_0 = parole + gain_0.*bruit;
s_5 = parole + gain_5.*bruit;
s_10 = parole + gain_10.*bruit;

% Affichage
figure 
subplot(3,1,1)
plot(time,s_0)
title('Signal bruité à RSB = 0dB')
xlabel('temps (s)');
ylabel('Amplitude')
subplot(3,1,2)
plot(time,s_5)
title('Signal bruité à RSB = 5dB')
xlabel('temps (s)');
ylabel('Amplitude')
subplot(3,1,3)
plot(time,s_10)
title('Signal bruité à RSB = 10dB')
xlabel('temps (s)');
ylabel('Amplitude')
ylim([-1,1])

% Vérification audio
son_0bd = audioplayer(s_0,fs);
son_5bd = audioplayer(s_5,fs);
son_10bd = audioplayer(s_10,fs);

play(player_new_SR)
pause(10)
play(son_0bd)
pause(10)
play(son_5bd)
pause(10)
play(son_10bd)
pause(10)

%% Réduction de bruit filtre FIR (TFSD inverse)

%1)

%2)

%% Réduction de bruit filtre IIR

%1)

%2)

%% Déterminer le filtre butterworth par transformation bilinéaire
clc
clearvars
close all

fe = 16e3;
wc_ini = 2*pi*2000;
wc = 2.*fe.*tan(wc_ini./(2.*fe));
T = 1/fe;

% En suivant les équations mathématiques développées à la main,
% Vecteur a:
a0 = 4 + 2.*sqrt(2).*wc.*T + T.^2.*wc.^2;
a1 = -8 + 2.*T.^2.*wc.^2;
a2 = 4 - 2.*sqrt(2).*wc.*T + T.^2.*wc.^2;

a_bi = [a0,a1,a2]./a0;

% Vecteur b:
b0 = wc.^2.*T.^2;
b1 = b0.*2;
b2 = b0;

b_bi = [b0,b1,b2]./a0;

% Par la suite, on détermine avec matlab les coefficients du filtre
[b_but,a_but] = butter(2,2000./8000);

figure
freqz(b_but,a_but)

figure
freqz(b_bi,a_bi)
