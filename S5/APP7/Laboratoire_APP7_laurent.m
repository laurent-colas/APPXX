%% Clean
clc
clear all
close all


%% Probleme 1

fc = 1000;
fe = 16000;
fmax = 8000;

n = 1:105; 
negatif_n = -105:-1;

theta_c = 2*pi*(fc/fe);
h_n0 = theta_c / pi;
h_n = (theta_c/pi) * sin(theta_c * n)./(pi.*n);
h_neg_n = (theta_c/pi) * sin(theta_c * negatif_n)./(pi.*negatif_n);
h_n = [h_neg_n h_n0 h_n];

figure(1)
stem([negatif_n 0 n], h_n)
xlim([-105 105])

fen_9 = h_n(((end/2)-4):((end/2)+4));
fen_15 = h_n(end/2-7:end/2+7);
fen_45 = h_n(end/2-22:end/2+22);
fen_101 = h_n(end/2-50:end/2+50);

X_9 = fft(fen_9);
Xm_9 = abs(X_9);
Xp_9 = angle(X_9);

%%
X_15 = fft(fen_15);
Xm_15 = abs(X_15);
Xp_15 = angle(X_15);

X_45 = fft(fen_45);
Xm_45 = abs(X_45);
Xp_45 = angle(X_45);

X_101 = fft(fen_101);
Xm_101 = abs(X_101);
Xp_101 = angle(X_101);

freq_9 = 0:pi/((numel(Xm_9)-1)/2):pi-(pi/(numel(Xm_9)-1)/2);
freq_15 = 0:pi/((numel(Xm_15)-1)/2):pi-(pi/(numel(Xm_15)-1)/2);
freq_45 = 0:pi/((numel(Xm_45)-1)/2):pi-(pi/(numel(Xm_45)-1)/2);
freq_101 = 0:pi/((numel(Xm_101)-1)/2):pi-(pi/(numel(Xm_101)-1)/2);

%%

x_9 = ifft(X_9);
x_15 = ifft(X_15);
x_45= ifft(X_45);
x_101 = ifft(X_101);

figure
plot(x_9)
hold on 
plot(x_15)
plot(x_45)
plot(x_101)

%% b fenetre de hamming
hamming_9 = h_n(end/2-4:end/2+4) .* hamming(9)';
hamming_15 = h_n(end/2-7:end/2+7) .* hamming(15)';
hamming_45 = h_n(end/2-22:end/2+22) .* hamming(45)';
hamming_101 = h_n(end/2-50:end/2+50) .* hamming(101)';

X_9 = fft(hamming_9);
Xm_9 = abs(X_9);
Xp_9 = angle(X_9);

X_15 = fft(hamming_15);
Xm_15 = abs(X_15);
Xp_15 = angle(X_15);

X_45 = fft(hamming_45);
Xm_45 = abs(X_45);
Xp_45 = angle(X_45);

X_101 = fft(hamming_101);
Xm_101 = abs(X_101);
Xp_101 = angle(X_101);


%% affichage pour joel et berhie

figure
subplot(2,1,1)
plot(freq_9,Xm_9(1:end/2))
subplot(2,1,2)
plot(freq_9, Xp_9(1:end/2))


figure
subplot(2,1,1)
plot(freq_15, Xm_15(1:end/2))
subplot(2,1,2)
plot(freq_15, Xp_15(1:end/2))


figure
subplot(2,1,1)
plot(freq_45, Xm_45(1:end/2))
subplot(2,1,2)
plot(freq_45, Xp_45(1:end/2))


figure
subplot(2,1,1)
plot(freq_101, Xm_101(1:end/2))
subplot(2,1,2)
plot(freq_101, Xp_101(1:end/2))

%% Prob 2 
clc
clearvars
close all

fc = 250;
fe = 24000;
fmax = 12000;

n = -1000:1000;

theta_c = 2*pi*(fc/fe);
theta0 = 2*pi*4000/fe;

h_n = (theta_c/pi) * sinc(theta_c * n/pi);
hpb_n = 2.*h_n.*cos(theta0.*n);


fc = 500;
fe = 24000;
fmax = 12000;

dirac = zeros(1,numel(n));
dirac(round(end/2)) = 1;

theta_c = 2*pi*(fc/fe);
theta0 = 2*pi*4000/fe;

h_n = (theta_c/pi) * sinc(theta_c * n/pi);
hcb_n = dirac - 2.*h_n.*cos(theta0.*n);

hamming_pb = hpb_n(end/2-15:end/2+15) .* hamming(31)';
hamming_cb = hcb_n(end/2-15:end/2+15) .* hamming(31)';

X_pb = fft(hamming_pb);
Xm_pb = 20*log10(abs(X_pb)./max(abs(X_pb)));
Xp_pb = angle(X_pb);

X_cb = fft(hamming_cb);
Xm_cb = 20*log10(abs(X_cb)./max(abs(X_cb)));
Xp_cb = angle(X_cb);

figure
subplot(2,1,1)
plot(Xm_pb(1:end/2))
hold on 
plot(Xm_cb(1:end/2))
subplot(2,1,2)
plot(Xp_pb(1:end/2))
hold on 
plot(Xp_cb(1:end/2))
%% Prob 3
clc
clearvars
close all

fe = 44.1e3;
f1 = 2e3;
f2 = 4e3;
f3 = 8e3;
f4 = 12e3;

N = 201;

k = 1:100;

H_k = zeros(1,numel(k));
H_k(round(f1./fe.*N):round(f2./fe.*N)) = 1;
H_k(round(f3./fe.*N):round(f4./fe.*N)) = 2;

% Determiner la H[k]
H_km = [H_k 0 flip(H_k(2:end))];
H_k = H_km;

k = 1:N;

figure
stem(H_k)

% Determiner la h[n]

h_n = ifft(H_k);
h_n = ifftshift(h_n);

figure
plot(h_n);
%% Prob 4
clc
clearvars
close all

m = 127; % Longueur de la réponse impulsionnelle
h = fir1(m, 0.2); % Filtre, h, « mystère » de votre choix
h(1:50) = 0; % Rendre h asymétrique pour fins de débogage
[x, Fs] = audioread('bruit_emis_32kHz.wav'); % Entrée des données, x, à l’émetteur
y = filter(h, [1 0], x); % Génération des données correspondantes, y, au récepteur début de l’amnésie temporaire ..

Rxx = xcorr(x, x, m); % Valeurs d’autocorrélation pour la matrice A
Rxx = Rxx(round(end/2):end);


Rxy = xcorr(x,y,m);
Rxy = Rxy(1:round(end/2));

A = toeplitz(Rxx, Rxx);
b = Rxy;

%votre code pour remplir la matrice A avec la fonction toeplitz()
%votre code pour remplir le tableau b

h_opt = linsolve(A,b); % Résolution du système matriciel pour le calcul du filtre optimal
% votre code pour le calcul de l’erreur résultante 

y_tild = sum(h_opt.*x(1:128));
E = sum((y_tild - y(1:128)).^2)


%% Prob 5

clc 
close all 
clearvars

% a)
fe = 48000;
Wp = 2*2500/fe;
Ws = 2*3500/fe;
Rp = 0.2;
Rs = 40;
[n,Wn] = buttord(Wp,Ws,Rp,Rs);
[b,a] = butter(n,Wn);

figure
freqz(b,a)
title(sprintf('n = %d Butterworth Lowpass Filter',n))
figure
pzmap(tf(b,a));

% b)
[n,Wn] = cheb1ord(Wp,Ws,Rp,Rs);
[b,a] = cheby1(n,Rp,Wn);

figure
freqz(b,a)
title(sprintf('n = %d Cheby 1 Lowpass Filter',n))
figure
pzmap(tf(b,a));

% c)
[n,Wn] = cheb2ord(Wp,Ws,Rp,Rs);
[b,a] = cheby2(n,Rs,Wn);

figure
freqz(b,a)
title(sprintf('n = %d Cheby 2 Lowpass Filter',n))
figure
pzmap(tf(b,a));

% d)
[n,Wn] = ellipord(Wp,Ws,Rp,Rs);
[b,a] = ellip(n,Rp,Rs,Wn);

figure
freqz(b,a)
title(sprintf('n = %d ellip Lowpass Filter',n))
figure
pzmap(tf(b,a));

% e)
ordre = 69;
b = fir1(ordre,Ws,'low');

figure
freqz(b,1,512)


