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

play(parole_estime_ply)
pause(10)
play(parole_captee_ply)


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

play(player_new_SR)
pause(10)
play(son_0bd)
pause(10)
play(son_5bd)
pause(10)
play(son_10bd)
pause(10)

%% Réduction de bruit filtre FIR (TFSD inverse) 2 filtres en cascade
fmax = 8000;
%1) passe-bas
n = 1:205;
n_negatif = -205:-1;

h_n_pb = 1/2.*sin(n.*(pi./2))./(n.*(pi./2));
h_n0_pb = 1/2;
h_negatif_pb = 1/2*sin(n_negatif.*(pi./2))./(n_negatif.*(pi./2));
h_n_pb = [h_negatif_pb h_n0_pb h_n_pb];

h_n_ph = -sin(0.11781.*n)./(pi.*n);
h_n0_ph = 0.96249;
h_negatif_ph = -sin(0.11781.*n_negatif)./(pi.*n_negatif);
h_n_ph = [h_negatif_ph h_n0_ph h_n_ph];

h_n_bande = conv(h_n_pb, h_n_ph);

figure
stem(h_n_bande)

ordre = 29;

indice1 = round(numel(h_n_ph)/2) - (round(ordre/2)-1);
indice2 = round(numel(h_n_ph)/2) + (round(ordre/2)-1);
hamming_13_ph = h_n_ph(indice1:indice2) .* hamming(ordre)';
hamming_13_pb = h_n_pb(indice1:indice2) .* hamming(ordre)';
X_ph = fft(hamming_13_ph);
Xm_ph = 20*log10(abs(X_ph) ./ max(abs(X_ph)));
X_pb = fft(hamming_13_pb);
Xm_pb = 20*log10(abs(X_pb) ./ max(abs(X_pb)));

pas_frequence = pi/((numel(X_ph)-1)/2);
frequence = 0 : pas_frequence : pi - pas_frequence;


indice1_passe_bande = round(numel(h_n_bande)/2) - (round(ordre/2)-1);
indice2_passe_bande = round(numel(h_n_bande)/2) + (round(ordre/2)-1);
hamming_pbande = h_n_bande(indice1_passe_bande:indice2_passe_bande) .* hamming(ordre)';

X_pbd = fft(hamming_pbande);
Xm_pbd = 20*log10(abs(X_pbd) ./ max(abs(X_pbd)));

pas_frequence_pbande = pi/((numel(X_pbd)-1)/2);
frequence_bande = 0 : pas_frequence_pbande : pi - pas_frequence_pbande;

upper_lim = (numel(Xm_ph)-1)/2;

figure
subplot(3,1,1)
plot(frequence, Xm_ph(1:upper_lim));
subplot(3,1,2)
plot(frequence, Xm_pb(1:upper_lim));
subplot(3,1,3)
plot(frequence_bande.*(fmax/pi), Xm_pbd(1:(end-1)/2));

%% Réduction de bruit filtre FIR (TFSD inverse) filtre passe bande
fmax = 8000;
n = 1:205;
n_negatif = -205:-1;
ordre = 31;

theta_0 = ((pi/2) + (3*pi)/80)/2;
theta_1 = (pi/2) - theta_0;
h_n_pbande = (2./(pi.*n)) .* sin(theta_1 .* n) .* cos(theta_0.*n);
h_n0_pbande = 2*theta_1 / pi;
h_n_pbande_negatif = (2./(pi.*n_negatif)) .* sin(theta_1 .* n_negatif) .* cos(theta_0.*n_negatif);

h_n_pbande = [h_n_pbande_negatif h_n0_pbande h_n_pbande];

figure
stem(h_n_pbande)

indice1_passe_bande = round(numel(h_n_pbande)/2) - (round(ordre/2)-1);
indice2_passe_bande = round(numel(h_n_pbande)/2) + (round(ordre/2)-1);
hamming_pbande = h_n_pbande(indice1_passe_bande:indice2_passe_bande) .* hamming(ordre)';

X_pbande = fft(hamming_pbande);
Xm_pbande = 20*log10(abs(X_pbande) ./ max(abs(X_pbande)));

pas_frequence_pbande = pi/((numel(X_pbande)-1)/2);
frequence_bande = 0 : pas_frequence_pbande : pi - pas_frequence_pbande;

upper_lim = (numel(Xm_pbande)-1)/2;

figure
plot(frequence_bande.*(fmax/pi), Xm_pbande(1:(end-1)/2));


%% Réduction de bruit filtre IIR

%1) Ordre le plus petit possible pour rencontrer les tolérances => elliptique

fe = 16000;
Wp = [300 4000]/(fe/2);
Ws = [150 5000]/(fe/2);
Rp = 0.5;
Rs = 40;

[n_elli,Wp_elli] = ellipord(Wp,Ws,Rp,Rs);
[b_elli,a_elli] = ellip(n_elli,Rp,Rs,Wp_elli);


%2) Moins d’oscillations possibles dans la bande => butterworth

[n_butt,Wp_butt] = buttord(Wp,Ws,Rp,Rs);
[b_butt,a_butt] = butter(n_butt,Wp_butt);

%% Affichage lieux de bode maison
freqz_maison(b_elli,a_elli,fe,fe);

freqz_maison(b_butt,a_butt,fe,fe);

% Vérification 
figure 
freqz(b_elli,a_elli,fe,fe)

figure
freqz(b_butt,a_butt,fe,fe)

%% Démo supression de bruit 



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


freqz_maison(b_but,a_but,16000,16000)

figure
freqz(b_bi,a_bi,16000,16000)
