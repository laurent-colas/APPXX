%   Function prototype:
%       [Xm Xp f] = myfft(b,a,N,fe)
%   Input:
%       1) b  : numérateur
%       2) a  : dénominateur (Mettre à 1 s'il n'y a pas de dénominateur)
%       3) N : Nb de points à afficher
%       4) fs : fréquence d'échantillonage

%   Output:
%       1) Affichage d'un lieu de bode

function freqz_maison(b,a,N,fe)

% Données de bases
n = 1:numel(b);
df = pi/N;
f = 0:df:pi-df;

% Réponse frequeciel
if numel(a) == 1 
    if a == 1
        for i = 1:numel(f)
            e = exp(-1j.*n.*f(i));
            H(i) = sum(e.*b);
        end
    end
else
    for i = 1:numel(f)
        e = exp(-1j.*n.*f(i));
        H(i) = sum(e.*b)/sum(e.*a);
    end
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
plot(f,Hm,'r')
ylabel('Magnitude (dB)')
xlabel('Fréquence (Hz)')
grid on 
subplot(2,1,2)
plot(f,Hp,'r')
ylabel('Phase (degrees)')
xlabel('Fréquence (Hz)')
grid on 