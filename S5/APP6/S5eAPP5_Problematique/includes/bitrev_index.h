/* ======================================================================== */
/*                                                                          */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      bitrev_index                                                        */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function has the prototype:                                    */
/*                                                                          */
/*      void bitrev_index(short *index, int n);                             */
/*                                                                          */
/*      index[sqrt(n)] : Pointer to index table that is returned by the     */
/*                       routine.                                           */
/*      n              : Number of complex array elements to bit-reverse.   */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This routine calculates the index table for the DSPLIB function     */
/*      sp_bitrev_cplx which performs a complex bit reversal of an array of */
/*      length n. The length of the index table is 2^(2*ceil(k/2)) where    */
/*      n = 2^k. In other words, the length of the index table is sqrt(n)   */
/*      if n is an even power of 2 and sqrt(2*n) otherwise.                 */
/*                                                                          */
/* ======================================================================== */

void bitrev_index(short *index, int n);
