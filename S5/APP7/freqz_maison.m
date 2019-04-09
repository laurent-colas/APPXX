%   Function prototype:
%       [Xm Xp f] = myfft(b,a,N,fe)
%   Input:
%       1) b  : num�rateur
%       2) a  : d�nominateur (Mettre � 1 s'il n'y a pas de d�nominateur)
%       3) N : Nb de points � afficher
%       4) fs : fr�quence d'�chantillonage

%   Output:
%       1) Affichage d'un lieu de bode

function freqz_maison(b,a,N,fe)

% Donn�es de bases
n = 1:numel(b);
df = pi/N;
f = 0:df:pi-df;

% R�ponse frequeciel
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

% donn�es d'affichage
df = (fe/2)/N;
f = 0:df:(fe/2)-df;

% Affichage
figure('NAME','Lieu de bode maison')
subplot(2,1,1)
plot(f,Hm,'r')
ylabel('Magnitude (dB)')
xlabel('Fr�quence (Hz)')
grid on 
subplot(2,1,2)
plot(f,Hp,'r')
ylabel('Phase (degrees)')
xlabel('Fr�quence (Hz)')
grid on 