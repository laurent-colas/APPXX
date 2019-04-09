%   Function prototype:
%       [Xm Xp f] = myfft(b,a,fe)
%   Input:
%       1) b  : numérateur
%       2) a  : dénominateur
%       3) fs : fréquence d'échantillonage
%       4) fs : Nb de points à afficher

%   Output:
%       1) Affichage d'un lieu de bode

function freqz_maison(b,a,N,fe)

% Données de bases
n = 1:numel(a);
df = pi/N;
f = 0:df:pi-df;

% Réponse frequeciel
for i = 1:numel(f)
    e = exp(-1j.*n.*f(i));
    H(i) = sum(e.*b)/sum(e.*a);
end

Hm = abs(H); % Magnitude
Hm = db(Hm);
Hp = unwrap(angle(H));
Hp = rad2deg(Hp);

% données d'affichage
df = (fe/2)/N;
f = 0:df:(fe/2)-df;

% Affichage
figure('NAME','Lieu de bode maison')
subplot(2,1,1)
plot(f,Hm)
ylabel('Magnitude (dB)')
xlabel('Fréquence (Hz)')
grid on 
subplot(2,1,2)
plot(f,Hp)
ylabel('Phase (degré)')
xlabel('Fréquence (Hz)')
grid on 