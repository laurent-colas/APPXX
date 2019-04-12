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

T = 1/Fs;

t = 0:T:(T*numel(parole_captee))-T;

figure 
plot(t,parole_captee)
hold on 
plot(t,parole_estime)
plot(t,abs(parole_captee - parole_estime).^2)
grid on 
grid minor
legend('parole captee','parole estime','|Erreur^2|')
xlabel('time (s)')
ylabel('Amplitude')

parole_estime_ply = audioplayer(parole_estime,Fs);
parole_captee_ply = audioplayer(parole_captee,Fs);

% play(parole_estime_ply)
% pause(10)
% play(parole_captee_ply)


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
pause(10)
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

% play(player_new_SR)
% pause(10)
% play(son_0bd)
% pause(10)
play(son_5bd)
% pause(10)
% play(son_10bd)
% pause(10)

%% Réduction de bruit filtre FIR (TFSD inverse) 2 filtres en cascade
fmax = 8000;
%1) passe-bas
n = 1:205;
n_negatif = -205:-1;

% Réponse imputlsionnelle filtre passe bas
h_n_pb = 1/2.*sin(n.*(pi./2))./(n.*(pi./2));
h_n0_pb = 1/2;
h_negatif_pb = 1/2*sin(n_negatif.*(pi./2))./(n_negatif.*(pi./2));
h_n_pb = [h_negatif_pb h_n0_pb h_n_pb];

%Reponse impulsionelle filtre passe haut
h_n_ph = -sin(0.11781.*n)./(pi.*n);
h_n0_ph = 0.96249;
h_negatif_ph = -sin(0.11781.*n_negatif)./(pi.*n_negatif);
h_n_ph = [h_negatif_ph h_n0_ph h_n_ph];

% Convolution des deux filtres
h_n_bande = conv(h_n_pb, h_n_ph);
n_negatif_bande = -410:-1;
n_bande = 1:410;

figure
subplot(2,1,1)
plot([n_negatif 0 n], h_n_pb)
title('Réponse impulsionnelle du filtre passe bas')
ylabel('Amplitude')
xlabel('Échantillon (n)')
grid on
xlim([-150 150])

subplot(2,1,2)
plot([n_negatif 0 n], h_n_ph)
title('Réponse impulsionnelle du filtre passe haut')
ylabel('Amplitude')
xlabel('Échantillon (n)')
grid on
xlim([-150 150])

figure('NAME','Réponse impulsionnelle du filtre passe-bande convoluer')
plot([n_negatif_bande 0 n_bande], h_n_bande)
ylabel('Amplitude')
xlabel('Échantillon (n)')
xlim([-200 200])

% estimation de l'ordre
ordre = 145;

% indice pour fenetrage des filtres passe haut et bas 
indice1 = round(numel(h_n_ph)/2) - (round(ordre/2)-1);
indice2 = round(numel(h_n_ph)/2) + (round(ordre/2)-1);
% fenetrage de hamming sur reponse impulsionnelle des filtres
hamming_13_ph = h_n_ph(indice1:indice2) .* hamming(ordre)';
hamming_13_pb = h_n_pb(indice1:indice2) .* hamming(ordre)';
% TFSD passe haut
X_ph = fft(hamming_13_ph);
Xm_ph = 20*log10(abs(X_ph) ./ max(abs(X_ph)));
% TFSD passe haut
X_pb = fft(hamming_13_pb);
Xm_pb = 20*log10(abs(X_pb) ./ max(abs(X_pb)));

% frequence pour graphique de bode
pas_frequence = pi/((numel(X_ph)-1)/2);
frequence = 0 : pas_frequence : pi - pas_frequence;


% indice pour fenetrage du filtre passe bande
indice1_passe_bande = round(numel(h_n_bande)/2) - (round(ordre/2)-1);
indice2_passe_bande = round(numel(h_n_bande)/2) + (round(ordre/2)-1);
% fenetrage de hamming sur reponse impulsionnelle du passe bande
hamming_pbande = h_n_bande(indice1_passe_bande:indice2_passe_bande) .* hamming(ordre)';
% TFSD passe bande
X_pbd = fft(hamming_pbande);
Xm_pbd = 20*log10(abs(X_pbd) ./ max(abs(X_pbd)));

% frequence filtre passe bande pour graphique de bode
pas_frequence_pbande = pi/((numel(X_pbd)-1)/2);
frequence_bande = 0 : pas_frequence_pbande : pi - pas_frequence_pbande;

% indice de pour premier moitié du filtre
upper_lim = (numel(Xm_ph)-1)/2;

figure
subplot(2,1,1)
hold on
plot(frequence.*(fmax/pi), Xm_ph(1:upper_lim))
plot(frequence.*(fmax/pi), Xm_pb(1:upper_lim))
hold off
title('Magnitude du filtre passe-haut et passe-bas')
ylabel('Magnitude (dB)')
xlabel('Fréquence (Hz)')
grid on
ylim([-80 10])
legend('Magnitude passe-haut', 'Magnitude passe-bas')

subplot(2,1,2)
plot(frequence_bande.*(fmax/pi), Xm_pbd(1:(end-1)/2))
title('Magnitude du filtre passe-bande convoluer')
ylabel('Magnitude (dB)')
xlabel('Fréquence (Hz)')
grid on
ylim([-80 10])


%% Réduction de bruit filtre FIR (TFSD inverse) filtre passe bande
fmax = 8000;
n = 1:205;
n_negatif = -205:-1;

ordre = 145;

% Angle important
theta_0 = ((pi/2) + (3*pi)/80)/2;
theta_1 = (pi/2) - theta_0;

%Reponse impulsionelle filtre passe bande
h_n_pbande = (2./(pi.*n)) .* sin(theta_1 .* n) .* cos(theta_0.*n);
h_n0_pbande = 2*theta_1 / pi;
h_n_pbande_negatif = (2./(pi.*n_negatif)) .* sin(theta_1 .* n_negatif) .* cos(theta_0.*n_negatif);
h_n_pbande = [h_n_pbande_negatif h_n0_pbande h_n_pbande];


figure('NAME','Réponse impulsionnelle du filtre passe-bande 2')
plot([n_negatif 0 n], h_n_pbande)
ylabel('Amplitude')
xlabel('Échantillon (n)')
xlim([-200 200])


% indice pour fenetrage du filtre passe bande
indice1_passe_bande = round(numel(h_n_pbande)/2) - (round(ordre/2)-1);
indice2_passe_bande = round(numel(h_n_pbande)/2) + (round(ordre/2)-1);
% fenetrage du passe bande
hamming_pbande = h_n_pbande(indice1_passe_bande:indice2_passe_bande) .* hamming(ordre)';

% TFSD
X_pbande = fft(hamming_pbande);
Xm_pbande = 20*log10(abs(X_pbande) ./ max(abs(X_pbande)));

% frequence pour lieu de bode
pas_frequence_pbande = pi/((numel(X_pbande)-1)/2);
frequence_bande2 = 0 : pas_frequence_pbande : pi - pas_frequence_pbande;

upper_lim = (numel(Xm_pbande)-1)/2;

figure
hold on
plot(frequence_bande2.*(fmax/pi), Xm_pbande(1:(end-1)/2));
hold off
title('Magnitude du filtre passe-bande')
ylabel('Magnitude (dB)')
xlabel('Fréquence (Hz)')
grid on
ylim([-100 10])


% Filtre FIR avec fir1
b = fir1((ordre-1)/2, [0.0375 1/2], 'bandpass');
freqz_maison(b,1,16000,16000)

%% Démo filtrage du bruit par filtre FIR

% Filtrage
fe = 16000;
signal_filtre_FIR = filter(hamming_pbande,1,s_5);
dt = 1/fe;
t = 0:dt:(numel(s_5)*dt)-dt;
t = t';

% Affichage temporel 
figure 
plot(t,s_5)
hold on 
plot(t,signal_filtre_FIR)
grid on 
grid minor
legend('Signal RSB = 10 dB ','Signal filtré (FIR ordre 145)')
xlabel('time (s)')
ylabel('Amplitude')

% Affichage frequenciel 
df = (fe/2)/numel(s_10);
f = 0:df:(fe/2)-df;
Xbruit = fft(s_10,2*numel(s_10));
Xbruit = Xbruit(1:end/2);
Xmbruit = abs(Xbruit);
Xmbruit = 20*log10(Xmbruit./max(abs(Xmbruit)));
Xpbruit = angle(Xbruit);

Xfiltre = fft(signal_filtre_FIR,2*numel(s_5));
Xfiltre = Xfiltre(1:end/2);
Xmfiltre = abs(Xfiltre);
Xmfiltre = 20*log10(Xmfiltre./max(abs(Xmfiltre)));
Xpfiltre = angle(Xfiltre);

figure
plot(f,Xmbruit)
hold on 
plot(f,Xmfiltre)
grid on 
grid minor
legend('Signal RSB = 10 dB ','Signal filtré (FIR ordre 145)')
ylabel('Magnitude (dB)')
xlabel('Frequence (Hz)')

% verification audio 
signal_filtre_FIR_player = audioplayer(signal_filtre_FIR,fe);
% play(son_10bd)
% pause(10)
play(signal_filtre_FIR_player)

%% Réduction de bruit filtre IIR

%1) Ordre le plus petit possible pour rencontrer les tolérances => elliptique

fe = 16000;
Wp = [300 4000]/(fe/2);
Ws = [150 5000]/(fe/2);
Rp = 0.5;
Rs = 40;

[n_elli,Wp_elli] = ellipord(Wp,Ws,Rp,Rs);
[b_elli,a_elli] = ellip(n_elli,Rp,Rs,Wp_elli);

fe = 16000;
Wp = [300 4000]/(fe/2);
Ws = [150 5000]/(fe/2);
Rp = 0.5;
Rs = 40;

%2) Moins d’oscillations possibles dans la bande => butterworth

[n_butt,Wp_butt] = buttord(Wp,Ws,Rp,Rs);
[b_butt,a_butt] = butter(n_butt,Wp_butt);

%% Affichage lieux de bode maison
freqz_maison(b_elli,a_elli,fe,fe);

freqz_maison(b_butt,a_butt,fe,fe);

%% Démo filtrage du bruit par filtre IIR

% Filtrage
signal_filtre_IIR = filter(h_n_pbande,1,s_10);
dt = 1/fe;
t = 0:dt:(numel(s_10)*dt)-dt;
t = t';

% Affichage temporel 
figure 
plot(t,s_10)
hold on 
plot(t,signal_filtre_IIR)
grid on 
grid minor
legend('Signal RSB = 10 dB ','Signal filtré (IIR)')
xlabel('time (s)')
ylabel('Amplitude')

% Affichage frequenciel 
df = (fe/2)/numel(s_10);
f = 0:df:(fe/2)-df;
Xbruit = fft(s_10,2*numel(s_10));
Xbruit = Xbruit(1:end/2);
Xmbruit = abs(Xbruit);
Xmbruit = 20*log10(Xmbruit./max(abs(Xmbruit)));
Xpbruit = angle(Xbruit);

Xfiltre = fft(signal_filtre_IIR,2*numel(s_10));
Xfiltre = Xfiltre(1:end/2);
Xmfiltre = abs(Xfiltre);
Xmfiltre = 20*log10(Xmfiltre./max(abs(Xmfiltre)));
Xpfiltre = angle(Xfiltre);

figure
plot(f,Xmbruit)
hold on 
plot(f,Xmfiltre)
grid on 
grid minor
legend('Signal RSB = 10 dB ','Signal filtré (IIR)')
ylabel('Magnitude (dB)')
xlabel('Frequence (Hz)')

% verification audio 
signal_filtre_IIR_player = audioplayer(signal_filtre_IIR,fe);
play(son_10bd)
pause(10)
play(signal_filtre_IIR_player)

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
