%   Function prototype:
%       [Xm Xp f] = myfft(b,a,fe)
%   Input:
%       1) b  : num�rateur
%       2) a  : d�nominateur
%       3) fs : fr�quence d'�chantillonage
%       4) fs : Nb de points � afficher

%   Output:
%       1) Affichage d'un lieu de bode

function freqz_maison(b,a,N,fe)

% Donn�es de bases
n = 1:numel(a);
df = pi/N;
f = 0:df:pi-df;

% R�ponse frequeciel
for i = 1:numel(f)
    e = exp(-1j.*n.*f(i));
    H(i) = sum(e.*b)/sum(e.*a);
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
plot(f,Hm)
ylabel('Magnitude (dB)')
xlabel('Fr�quence (Hz)')
grid on 
subplot(2,1,2)
plot(f,Hp)
ylabel('Phase (degr�)')
xlabel('Fr�quence (Hz)')
grid on 