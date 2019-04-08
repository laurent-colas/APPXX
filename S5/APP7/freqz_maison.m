%   Function prototype:
%       [Xm Xp f] = myfft(b,a,fe)
%   Input:
%       1) b  : 
%       2) a  : 
%       3) fs : fr�quence d'�chantillonage
%       4) fs : Nb de points � afficher

%   Output:
%       1) Affichage d'un lieu de bode

function freqz_maison(b,a,N,fe)

% Donn�es de bases
dt = 1/fe;
df = (fe/2)/N;
f = 0:df:(fe/2)-df;
N = 2*N;
t = dt*(0:N-1);
rad2deg = 180/pi; 
u = [1 zeros(1,N-1)];



% R�ponse impultionnelle (domaine temporelle)
h = filter(b,a,u);

% Fenetrage 

wind = hamming(numel(h));
wind = wind';
h = h.*wind;

% Domaine fr�quenciel
H = fft(h);
H = H(1:end/2);
Hm = abs(H);
Hm = 20*log10(Hm./max(abs(Hm)));
Hp = angle(H);
Hp = Hp.*rad2deg;


% Affichage
figure('NAME','Lieu de bode maison')
subplot(2,1,1)
plot(f,Hm)
title('abs(H)')
ylabel('Magnitude (dB)')
xlabel('Fr�quence (Hz)')
% ylim([-300,0]);
grid on 
subplot(2,1,2)
plot(f,Hp)
title('angle(H)')
ylabel('Phase (degr�)')
xlabel('Fr�quence (Hz)')
grid on 