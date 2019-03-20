/********************************************************************************************
Version 1.0
Auteur : Bruno Gagnon
Date de création : 22 juillet 2009
Date de la dernière modification : 25 octobre 2009
Modifé par : Paul Charette
Description : Filtre FIR de type direct 1
Propriété : Université de Sherbrooke, département de génie électrique et informatique

Paramètres:
	cPtr: pointeur de l'échantillon courant dans le tampon
	ech : échantillon à ajouter dans le tampon
	coeffs: coeifficients du filtre FIR, de longueur N
	tampon: tampon contenant les échantillons, de longueur L
	y: adresse du résultat de l'opération de filtrage

********************************************************************************************/

short * direct1FIR(short *cPtr, short ech, const short coeffs[], short N, short tampon[], short L, short *y)

{
	int k;
	int out=0; // Doit être un int pour accepter le Q30
	short *p;

	/******************************************************************************
	Enregistrement du nouvel échantillon dans le tampon
	******************************************************************************/
	cPtr++;
	if (cPtr == tampon+L) // Si adresse de l'éch. courant à la fin du tampon
		cPtr = tampon;
	*cPtr = ech;		  // Enregistrement de l'éch. courant dans le tampon

	/******************************************************************************
	Filtrage du tampon par convolution
	******************************************************************************/
	p = cPtr;
	for (k=0; k<N; k++)	{    // Pour tous les échantillons du tampon
		// Convolution y[n] = sommation(h[k]x[n-k])
		out = out + coeffs[k] * (*p--);	  // cette multiplication donne Q30

		if (p < tampon)   // Si l'adresse de l'éch. courant sortie du tampon
			p = tampon+L-1;    // Remettre l'adresse à la fin du tampon
	}

	out = out >> 15; // décalage arithméthique à droite pour passer de Q30 à Q15
	*y = out;

	return cPtr;
}

