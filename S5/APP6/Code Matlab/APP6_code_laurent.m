%% clean and clear
clc 
close all
clearvars

%% Spécification des filtres IIR
% Frequence d'echantillonnage
fe = 8000;

% frequence fondamental des notes
MI2 = 82.407;
LA2 = 110.000;
RE3 = 146.832;
SOL3 = 195.998;
SI3 = 246.942;
MI4 = 329.628;

%For digital filters, the passband edge 
% frequencies must lie between 0 and 1,
% where 1 corresponds to the Nyquist rate—half
% the sample rate or ? rad/sample.
f_coupure_MI2 = (MI2 * 1.5)/ (fe/2);
f_coupure_LA2 = (LA2 * 1.5) / (fe/2);
f_coupure_RE3 = (RE3 * 1.5) / (fe/2);
f_coupure_SOL3 = (SOL3 * 1.5) / (fe/2);
f_coupure_SI3 = (SI3 * 1.5) / (fe/2);
f_coupure_MI4 = (MI4 * 1.5) / (fe/2);

% oscillation dans la bande passante ronflement
oscillation_dB = 3;


%% Filtre d’ordre 4 MI2
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le MI2
[A_MI2, B_MI2, C_MI2, D_MI2] = cheby1(4, oscillation_dB, f_coupure_MI2);
% Deux sections d’ordre 2 en cascade
[sos_MI2, gain_global_MI2] = ss2sos(A_MI2, B_MI2, C_MI2, D_MI2, 'up', 'inf');

% Format Q2.13
sos_MI2_Q2_13 = round(sos_MI2 * 2^13);
gain_global_MI2_Q2_13 = round(gain_global_MI2 .* 2^13);

% Visualiser la réponse des filtres
[b_MI2, a_MI2] = sos2tf(sos_MI2, gain_global_MI2); 
[b_MI2_Q213, a_MI2_Q213] = sos2tf(sos_MI2_Q2_13, gain_global_MI2_Q2_13); 

figure(1)
zplane(b_MI2,a_MI2); 
figure(2)
zplane(b_MI2_Q213,a_MI2_Q213); 
figure(3)
freqz(b_MI2,a_MI2);



%% Filtre d’ordre 4 LA2
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le LA2
[A_LA2, B_LA2, C_LA2, D_LA2] = cheby1(4, oscillation_dB, f_coupure_LA2);
% Deux sections d’ordre 2 en cascade
[sos_LA2, gain_global_LA2] = ss2sos(A_LA2, B_LA2, C_LA2, D_LA2, 'up', 'inf');

% Format Q2.13
sos_LA2_Q2_13 = round(sos_LA2 * 2^13); 
gain_global_LA2_Q2_13 = round(gain_global_LA2 .* 2^13);

% Visualiser la réponse des filtres
[b_LA2, a_LA2] = sos2tf(sos_LA2, gain_global_LA2); 
[b_LA2_Q213, a_LA2_Q213] = sos2tf(sos_LA2_Q2_13, gain_global_LA2_Q2_13); 

figure(1)
zplane(b_LA2,a_LA2); 
figure(2)
zplane(b_LA2_Q213,a_LA2_Q213); 
figure(3)
freqz(b_LA2,a_LA2);



%% Filtre d’ordre 4 RE3
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le RE3
[A_RE3, B_RE3, C_RE3, D_RE3] = cheby1(4, oscillation_dB, f_coupure_RE3);
% Deux sections d’ordre 2 en cascade
[sos_RE3, gain_global_RE3] = ss2sos(A_RE3, B_RE3, C_RE3, D_RE3, 'up', 'inf');

% Format Q2.13
sos_RE3_Q2_13 = round(sos_RE3 * 2^13); 
gain_global_RE3_Q2_13 = round(gain_global_RE3 .* 2^13);

% Visualiser la réponse des filtres
[b_RE3, a_RE3] = sos2tf(sos_RE3, gain_global_RE3); 
[b_RE3_Q213, a_RE3_Q213] = sos2tf(sos_RE3_Q2_13, gain_global_RE3_Q2_13); 

figure(1)
zplane(b_RE3,a_RE3); 
figure(2)
zplane(b_RE3_Q213,a_RE3_Q213); 
figure(3)
freqz(b_RE3,a_RE3);



%% Filtre d’ordre 4 SOL3
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le SOL3
[A_SOL3, B_SOL3, C_SOL3, D_SOL3] = cheby1(4, oscillation_dB, f_coupure_SOL3);
% Deux sections d’ordre 2 en cascade
[sos_SOL3, gain_global_SOL3] = ss2sos(A_SOL3, B_SOL3, C_SOL3, D_SOL3, 'up', 'inf');

% Format Q2.13
sos_SOL3_Q2_13 = round(sos_SOL3 * 2^13);
gain_global_SOL3_Q2_13 = round(gain_global_SOL3 .* 2^13);

% Visualiser la réponse des filtres
[b_SOL3, a_SOL3] = sos2tf(sos_SOL3, gain_global_SOL3); 
[b_SOL3_Q213, a_SOL3_Q213] = sos2tf(sos_SOL3_Q2_13, gain_global_SOL3_Q2_13); 

figure(1)
zplane(b_SOL3,a_SOL3); 
figure(2)
zplane(b_SOL3_Q213,a_SOL3_Q213); 
figure(3)
freqz(b_SOL3,a_SOL3);


%% Filtre d’ordre 4 SI3
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le SI3
[A_SI3, B_SI3, C_SI3, D_SI3] = cheby1(4, oscillation_dB, f_coupure_SI3);
% Deux sections d’ordre 2 en cascade
[sos_SI3, gain_global_SI3] = ss2sos(A_SI3, B_SI3, C_SI3, D_SI3, 'up', 'inf');

% Format Q2.13
sos_SI3_Q2_13 = round(sos_SI3 * 2^13); 
gain_global_SI3_Q2_13 = round(gain_global_SI3 .* 2^13);

% Visualiser la réponse des filtres
[b_SI3, a_SI3] = sos2tf(sos_SI3, gain_global_SI3); 
[b_SI3_Q213, a_SI3_Q213] = sos2tf(sos_SI3_Q2_13, gain_global_SI3_Q2_13); 

figure(1)
zplane(b_SI3,a_SI3); 
figure(2)
zplane(b_SI3_Q213,a_SI3_Q213); 
figure(3)
freqz(b_SI3,a_SI3);



%% Filtre d’ordre 4 MI4
% Creation d'un filtre cheby d'ordre 4 avec les specifications demandes
% pour le MI4
[A_MI4, B_MI4, C_MI4, D_MI4] = cheby1(4, oscillation_dB, f_coupure_MI4);
% Deux sections d’ordre 2 en cascade
[sos_MI4, gain_global_MI4] = ss2sos(A_MI4, B_MI4, C_MI4, D_MI4, 'up', 'inf');

% Format Q2.13
sos_MI4_Q2_13 = round(sos_MI4 * 2^13); 
gain_global_MI4_Q2_13 = round(gain_global_MI4 .* 2^13);

% Visualiser la réponse des filtres
[b_MI4, a_MI4] = sos2tf(sos_MI4, gain_global_MI4); 
[b_MI4_Q213, a_MI4_Q213] = sos2tf(sos_MI4_Q2_13, gain_global_MI4_Q2_13); 

figure(1)
zplane(b_MI4,a_MI4); 
figure(2)
zplane(b_MI4_Q213,a_MI4_Q213); 
figure(3)
freqz(b_MI4,a_MI4);
