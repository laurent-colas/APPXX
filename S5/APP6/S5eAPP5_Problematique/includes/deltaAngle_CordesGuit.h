/*********************************************
 DONN�ES POUR G�N�RER LES FR�QUENCES DES CORDES 
 � L'AIDE DE LA FONCTION genFrq_FctCos()
 NOTE : Permet r�duire le nombre de calculs 
*********************************************/

#ifndef DELTAANGLE_CORDEGUIT_H_
#define DELTAANGLE_CORDEGUIT_H_

//  2*PI*frq/fs     o�   fs = 8000 Hz
float DeltaAngle[6] =
{
	0.0647223,	// corde #1 @ f=82.407 Hz
	0.0863938,	// corde #2 @ f=110.000 Hz
	0.1153216,	// corde #3 @ f=146.832 Hz
	0.1539365,	// corde #4 @ f=195.998 Hz
	0.1939478,	// corde #5 @ f=246.942 Hz
	0.2588892	// corde #6 @ f=329.628 Hz
};

#endif /* DELTAANGLE_CORDEGUIT_H */
