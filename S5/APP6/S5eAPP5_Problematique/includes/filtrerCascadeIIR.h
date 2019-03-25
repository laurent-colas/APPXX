/*
 * filtrerCascadeIIR.h
 *
 *  Created on: 25 mars 2019
 *      Author: laure
 */

#ifndef INCLUDES_FILTRERCASCADEIIR_H_
#define INCLUDES_FILTRERCASCADEIIR_H_

/* Description : Appeler des filtres IIR d'ordre 2 pour faire une cascade
                 de filtres et appliquer le gain global */
short filtrerCascadeIIR(int noCorde, short x);

// Description : Initialisation des variables intermédiaires w(n-i)
void init_w();

// Description : Permet de faire une saturation à 16bits
short sat_16bits(int x);




#endif /* INCLUDES_FILTRERCASCADEIIR_H_ */
