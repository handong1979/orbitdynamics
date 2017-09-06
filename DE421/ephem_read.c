/******************************************************************************/
/**                                                                          **/
/**  SOURCE FILE: ephem_read.c                                               **/
/**                                                                          **/
/**     This file contains a set of functions and global variables that      **/
/**     implements an ephemeris server program. Client programs can use      **/
/**     use this server to access ephemeris data by calling one of the       **/
/**     following functions:                                                 **/
/**                                                                          **/
/**        Interpolate_Libration -- returns lunar libration angles           **/
/**        Interpolate_Nutation  -- returns (terrestrial) nutation angles    **/
/**        Interpolate_Position  -- returns position of planet               **/
/**        Interpolate_State     -- returns position and velocity of planet  **/
/**                                                                          **/
/**     Note that client programs must make one, and only one, call to the   **/
/**     function Initialize_Ephemeris before any of these other functions    **/ 
/**     be used. After this call any of the above functions can be called    **/
/**     in any sequence.                                                     **/
/**                                                                          **/
/**  Programmer: David Hoffman/EG5                                           **/
/**              NASA, Johnson Space Center                                  **/
/**              Houston, TX 77058                                           **/
/**              e-mail: david.a.hoffman1@jsc.nasa.gov                       **/
/**                                                                          **/
/**  History:                                                                **/
/**                                                                          **/
/**    8/11/99  Changed made to function Read_Coefficients to ensure that    **/
/**             Offset has a negative value when requested time comes        **/
/**             before the beginning of the current record. Thanks to Gary   **/
/**             Bernstien (garyb@astro.lsa.umich.edu) for pointing out this  **/
/**             serious bug.                                                 **/
/**                                                                          **/
/******************************************************************************/

#include <stdio.h>
#include <math.h>
#ifndef TYPES_DEFINED
#include "ephem_types.h"
#endif

/**==========================================================================**/
/**  Global Variables                                                        **/
/**==========================================================================**/

   static recOneType   R1;
   static recTwoType   R2;
   static FILE        *Ephemeris_File;
   static double       Coeff_Array[ARRAY_SIZE] , T_beg , T_end , T_span;

   static int Debug = FALSE;             /* Generates detailed output if true */

/**==========================================================================**/
/**  Read_Coefficients                                                       **/
/**                                                                          **/
/**     This function is used by the functions below to read an array of     **/
/**     Tchebeychev coefficients from a binary ephemeris data file.          **/
/**                                                                          **/
/**  Input: Desired record time.                                             **/
/**                                                                          **/
/**  Output: None.                                                           **/
/**                                                                          **/
/**==========================================================================**/

void Read_Coefficients( double Time )
{
  double  T_delta = 0.0;
  int     Offset  =  0 ;

  /*--------------------------------------------------------------------------*/
  /*  Find ephemeris data that record contains input time. Note that one, and */
  /*  only one, of the following conditional statements will be true (if both */
  /*  were false, this function would not have been called).                  */
  /*--------------------------------------------------------------------------*/

  if ( Time < T_beg )                    /* Compute backwards location offset */
     {
       T_delta = T_beg - Time;
       Offset  = (int) -ceil(T_delta/T_span); 
     }

  if ( Time > T_end )                    /* Compute forewards location offset */
     {
       T_delta = Time - T_end;
       Offset  = (int) ceil(T_delta/T_span);
     }

  /*--------------------------------------------------------------------------*/
  /*  Retrieve ephemeris data from new record.                                */
  /*--------------------------------------------------------------------------*/

  fseek(Ephemeris_File,(Offset-1)*ARRAY_SIZE*sizeof(double),SEEK_CUR);
  fread(&Coeff_Array,sizeof(double),ARRAY_SIZE,Ephemeris_File);
  
  T_beg  = Coeff_Array[0];
  T_end  = Coeff_Array[1];
  T_span = T_end - T_beg;

  /*--------------------------------------------------------------------------*/
  /*  Debug print (optional)                                                  */
  /*--------------------------------------------------------------------------*/

  if ( Debug ) 
     {
       printf("\n  In: Read_Coefficients \n");
       printf("\n      ARRAY_SIZE = %4d",ARRAY_SIZE);
       printf("\n      Offset  = %3d",Offset);
       printf("\n      T_delta = %7.3f",T_delta);
       printf("\n      T_Beg   = %7.3f",T_beg);
       printf("\n      T_End   = %7.3f",T_end);
       printf("\n      T_Span  = %7.3f\n\n",T_span);
     }

}

/**==========================================================================**/
/**  Initialize_Ephemeris                                                    **/
/**                                                                          **/
/**     This function must be called once by any program that accesses the   **/
/**     ephemeris data. It opens the ephemeris data file, reads the header   **/
/**     data, loads the first coefficient record into a global array, then   **/
/**     returns a status code that indicates whether or not all of this was  **/
/**     done successfully.                                                   **/
/**                                                                          **/
/**  Input: A character string giving the name of an ephemeris data file.    **/
/**                                                                          **/
/**  Returns: An integer status code.                                        **/
/**                                                                          **/
/**==========================================================================**/

int Initialize_Ephemeris( char *fileName )
{
  int headerID;
  
  /*--------------------------------------------------------------------------*/
  /*  Open ephemeris file.                                                    */
  /*--------------------------------------------------------------------------*/

  Ephemeris_File = fopen(fileName,"rb");

  /*--------------------------------------------------------------------------*/
  /*  Read header & first coefficient array, then return status code.         */
  /*--------------------------------------------------------------------------*/

  if ( Ephemeris_File == NULL ) /*........................No need to continue */
     {
       printf("\n Unable to open ephemeris file: %s.\n",fileName);
       return FAILURE;
     }
  else 
     { /*.................Read first three header records from ephemeris file */
         
	   fread(&R1,sizeof(recOneType),1,Ephemeris_File);
	   fread(&R2,sizeof(recTwoType),1,Ephemeris_File);
       fread(&Coeff_Array,sizeof(double),ARRAY_SIZE,Ephemeris_File);
            
       /*..........................................Set current time variables */

       T_beg  = Coeff_Array[0];
       T_end  = Coeff_Array[1];
       T_span = T_end - T_beg;

       /*..............................Convert header ephemeris ID to integer */

       headerID = (int) R1.DENUM;
       
       /*..............................................Debug Print (optional) */

       if ( Debug ) 
          {
            printf("\n  In: Initialize_Ephemeris \n");
            printf("\n      ARRAY_SIZE = %4d",ARRAY_SIZE);
            printf("\n      headerID   = %3d",headerID);
            printf("\n      T_Beg      = %7.3f",T_beg);
            printf("\n      T_End      = %7.3f",T_end);
            printf("\n      T_Span     = %7.3f\n\n",T_span);
          }

       /*..................................................Return status code */
       
       if ( headerID == EPHEMERIS ) 
          {
            return SUCCESS;
          }
       else 
          {
            printf("\n Opened wrong file: %s",fileName);
            printf(" for ephemeris: %d.\n",EPHEMERIS);
            return FAILURE;
          }
     }
}

/**==========================================================================**/
/**  Interpolate_Libration                                                   **/
/**                                                                          **/
/**     This function computes an array of libration angles from Chebyshev   **/
/**     coefficients read in from an ephemeris data file. The coefficients   **/
/**     are read in from a file by the function Read_Coefficients (when      **/
/**     necessary).                                                          **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**     Time     -- Time for which position is desired (Julian Date).        **/
/**     Target   -- Solar system body for which position is desired.         **/
/**     Libration -- Pointer to external array to receive the answer.         **/
/**                  Libration[0] : Omega                                    **/
/**                  Libration[1] : i                                        **/
/**                  Libration[2] : u                                        **/
/**                  Libration[3] : Omega_dot                                **/
/**                  Libration[4] : i_dot                                    **/
/**                  Libration[5] : u_dot                                    **/
/**                                                                          **/
/**  Returns: Nothing explicitly.                                            **/
/**==========================================================================**/

void Interpolate_Libration( double Time , int Target , double Libration[6] )
{
  double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
  int       i , j;
  long int  C , G , N , offset = 0;

  /*--------------------------------------------------------------------------*/
  /* This function only computes librations.                                  */
  /*--------------------------------------------------------------------------*/

  if ( Target != 12 )             /* Also protects against weird input errors */
     {
       printf("\n This function only computes librations.\n");
       return;
     }
 
  /*--------------------------------------------------------------------------*/
  /* Determine if a new record needs to be input (if so, get it).             */
  /*--------------------------------------------------------------------------*/
    
  if ( Time < T_beg || Time > T_end ) Read_Coefficients(Time);

  /*--------------------------------------------------------------------------*/
  /* Read the coefficients from the binary record.                            */
  /*--------------------------------------------------------------------------*/
  
  C = R1.libratPtr[0] - 1;                   /* Coefficient array entry point */
  N = R1.libratPtr[1];                       /*        Number of coefficients */
  G = R1.libratPtr[2];                       /*    Granules in current record */

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  In: Interpolate_Libration\n");
       printf("\n  Target = %2d",Target);
       printf("\n  C      = %4d (before)",C);
       printf("\n  N      = %4d",N);
       printf("\n  G      = %4d\n",G);
     }

  /*--------------------------------------------------------------------------*/
  /*  Compute the normalized time, then load the Tchebeyshev coefficients     */
  /*  into array A[]. If T_span is covered by a single granule this is easy.  */
  /*  If not, the granule that contains the interpolation time is found, and  */
  /*  an offset from the array entry point for the libration angles is used   */
  /*  to load the coefficients.                                               */
  /*--------------------------------------------------------------------------*/

  if ( G == 1 ) 
     {
       Tc = 2.0*(Time - T_beg) / T_span - 1.0;       
     }
  else if ( G > 1 )
     {
       T_sub = T_span / ((double) G);          /* Compute subgranule interval */
       
       for ( j=G ; j>0 ; j-- ) 
           {
             T_break = T_beg + ((double) j-1) * T_sub;
             if ( Time > T_break ) 
                {
                  T_seg  = T_break;
                  offset = j-1;
                  break;
                }
            }
            
       Tc = 2.0*(Time - T_seg) / T_sub - 1.0;
       C  = C + 3 * offset * N;
     }
  else                                   /* Something has gone terribly wrong */
     {
       printf("\n Number of granules must be >= 1: check header data.\n");
     }

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  C      = %4d (after)",C);
       printf("\n  offset = %4d",offset);
       printf("\n  Time   = %12.7f",Time);
       printf("\n  T_sub  = %12.7f",T_sub);
       printf("\n  T_seg  = %12.7f",T_seg);
       printf("\n  Tc     = %12.7f\n",Tc);
       printf("\n  Array Coefficients:\n");
       for ( i=0 ; i<3*N ; i++ )
           {
             printf("\n  A[%2d] = % 22.15e",i,Coeff_Array[C+i]);
           }
       printf("\n\n");
     }

  /*..........................................................................*/

  /*--------------------------------------------------------------------------*/
  /* Compute interpolated the libration.                                      */
  /*--------------------------------------------------------------------------*/
  
	for ( i=0 ; i<3 ; i++ )                /* Compute interpolating polynomials */
	{
		Cp[0] = 1.0;           
		Cp[1] = Tc;
		Cp[2] = 2.0 * Tc*Tc - 1.0;

		Up[0] = 0.0;
		Up[1] = 1.0;
		Up[2] = 4.0 * Tc;

		for ( j=3 ; j<N ; j++ )
		{
			Cp[j] = 2.0 * Tc * Cp[j-1] - Cp[j-2];
			Up[j] = 2.0 * Tc * Up[j-1] + 2.0 * Cp[j-1] - Up[j-2];
		}

		Libration[i] = 0.0;           /* Compute interpolated position & velocity */
		Libration[i+3] = 0.0;

		for ( j=N-1 ; j>-1 ; j-- )  Libration[i] = Libration[i] + Coeff_Array[C+j+i*N] * Cp[j];
		for ( j=N-1 ; j>0  ; j-- )  Libration[i+3] = Libration[i+3] + Coeff_Array[C+j+i*N] * Up[j];

		Libration[i+3] = Libration[i+3] * 2.0 * ((double) G) / (T_span * 86400.0);
	}
      
  return;
}


/**==========================================================================**/
/**  Interpolate_Nutation                                                    **/
/**                                                                          **/
/**     This function computes an array of nutation angles from Chebyshev    **/
/**     coefficients read in from an ephemeris data file. The coefficients   **/
/**     are read in from a file by the function Read_Coefficients (when      **/
/**     necessary).                                                          **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**     Time     -- Time for which position is desired (Julian Date).        **/
/**     Target   -- Solar system body for which position is desired.         **/
/**     Nutation -- Pointer to external array to receive the angles.         **/
/**                 Nutation[0] : dksi                                       **/
/**                 Nutation[1] : deps                                       **/
/**                 Nutation[2] : dksi_dot                                   **/
/**                 Nutation[3] : deps_dot                                   **/
/**                                                                          **/
/**  Returns: Nothing explicitly.                                            **/
/**                                                                          **/
/**==========================================================================**/

void Interpolate_Nutation( double Time , int Target , double Nutation[4] )
{
  double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
  int       i , j;
  long int  C , G , N , offset = 0;

  /*--------------------------------------------------------------------------*/
  /* This function only computes nutations.                                   */
  /*--------------------------------------------------------------------------*/

  if ( Target != 11 )             /* Also protects against weird input errors */
     {
       printf("\n This function only computes nutations.\n");
       return;
     }

  /*--------------------------------------------------------------------------*/
  /* Determine if a new record needs to be input (if so, get it).             */
  /*--------------------------------------------------------------------------*/
    
  if (Time < T_beg || Time > T_end)  Read_Coefficients(Time);

  /*--------------------------------------------------------------------------*/
  /* Read the coefficients from the binary record.                            */
  /*--------------------------------------------------------------------------*/
  
  C = R1.coeffPtr[Target][0] - 1;            /* Coefficient array entry point */
  N = R1.coeffPtr[Target][1];                /*        Number of coefficients */
  G = R1.coeffPtr[Target][2];                /*    Granules in current record */

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  In: Interpolate_Nutation\n");
       printf("\n  Target = %2d",Target);
       printf("\n  C      = %4d (before)",C);
       printf("\n  N      = %4d",N);
       printf("\n  G      = %4d\n",G);
     }

  /*--------------------------------------------------------------------------*/
  /*  Compute the normalized time, then load the Tchebeyshev coefficients     */
  /*  into array A[]. If T_span is covered by a single granule this is easy.  */
  /*  If not, the granule that contains the interpolation time is found, and  */
  /*  an offset from the array entry point for the nutation angles is used    */
  /*  to load the coefficients.                                               */
  /*--------------------------------------------------------------------------*/

  if ( G == 1 )
     {
       Tc = 2.0*(Time - T_beg) / T_span - 1.0;
     }
  else if ( G > 1 )
     {
       T_sub = T_span / ((double) G);          /* Compute subgranule interval */
       
       for ( j=G ; j>0 ; j-- ) 
           {
             T_break = T_beg + ((double) j-1) * T_sub;
             if ( Time > T_break ) 
                {
                  T_seg  = T_break;
                  offset = j-1;
                  break;
                }
            }
            
       Tc = 2.0*(Time - T_seg) / T_sub - 1.0;
       C  = C + 2 * offset * N;
     }
  else                                   /* Something has gone terribly wrong */
     {
       printf("\n Number of granules must be >= 1: check header data.\n");
     }

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  C      = %4d (after)",C);
       printf("\n  offset = %4d",offset);
       printf("\n  Time   = %12.7f",Time);
       printf("\n  T_sub  = %12.7f",T_sub);
       printf("\n  T_seg  = %12.7f",T_seg);
       printf("\n  Tc     = %12.7f\n",Tc);
       printf("\n  Array Coefficients:\n");
       for ( i=0 ; i<2*N ; i++ )
           {
			   printf("\n  A[%2d] = % 22.15e",i,Coeff_Array[C+i]);
           }
       printf("\n\n");
     }

  /*..........................................................................*/

  /*--------------------------------------------------------------------------*/
  /* Compute interpolated the nutation.                                       */
  /*--------------------------------------------------------------------------*/
  
	for ( i=0 ; i<2 ; i++ )                /* Compute interpolating polynomials */
	{
		Cp[0] = 1.0;           
		Cp[1] = Tc;
		Cp[2] = 2.0 * Tc*Tc - 1.0;

		Up[0] = 0.0;
		Up[1] = 1.0;
		Up[2] = 4.0 * Tc;

		for ( j=3 ; j<N ; j++ )
		{
			Cp[j] = 2.0 * Tc * Cp[j-1] - Cp[j-2];
			Up[j] = 2.0 * Tc * Up[j-1] + 2.0 * Cp[j-1] - Up[j-2];
		}

		Nutation[i] = 0.0;           /* Compute interpolated position & velocity */
		Nutation[i+2] = 0.0;

		for ( j=N-1 ; j>-1 ; j-- )  Nutation[i] = Nutation[i] + Coeff_Array[C+j+i*N] * Cp[j];
		for ( j=N-1 ; j>0  ; j-- )  Nutation[i+2] = Nutation[i+2] + Coeff_Array[C+j+i*N] * Up[j];

		Nutation[i+2] = Nutation[i+2] * 2.0 * ((double) G) / (T_span * 86400.0);
	}
      
  return;
}

/**==========================================================================**/
/**  Interpolate_Position                                                    **/
/**                                                                          **/
/**     This function computes a position vector for a selected planetary    **/
/**     body from Chebyshev coefficients read in from an ephemeris data      **/
/**     file. These coefficients are read from the data file by calling      **/
/**     the function Read_Coefficients (when necessary).                     **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**     Time     -- Time for which position is desired (Julian Date).        **/
/**     Target   -- Solar system body for which position is desired.         **/
/**     Position -- Pointer to external array to receive the position.       **/
/**                                                                          **/
/**  Returns: Nothing explicitly.                                            **/
/**                                                                          **/
/**==========================================================================**/

void Interpolate_Position( double Time , int Target , double Position[3] )
{
  double   Cp[50] , T_break , T_seg , T_sub , Tc;
  int       i , j;
  long int  C , G , N , offset = 0;

  /*--------------------------------------------------------------------------*/
  /* This function doesn't "do" nutations or librations.                      */
  /*--------------------------------------------------------------------------*/

  if ( Target >= 11 )             /* Also protects against weird input errors */
     {
       printf("\n This function does not compute nutations or librations.\n");
       return;
     }

  /*--------------------------------------------------------------------------*/
  /* Determine if a new record needs to be input (if so, get it).             */
  /*--------------------------------------------------------------------------*/
    
  if (Time < T_beg || Time > T_end)  Read_Coefficients(Time);

  /*--------------------------------------------------------------------------*/
  /* Read the coefficients from the binary record.                            */
  /*--------------------------------------------------------------------------*/
  
  C = R1.coeffPtr[Target][0] - 1;          /*   Coefficient array entry point */
  N = R1.coeffPtr[Target][1];              /* Number of coeff's per component */
  G = R1.coeffPtr[Target][2];              /*      Granules in current record */

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  In: Interpolate_Position\n");
       printf("\n  Target = %2d",Target);
       printf("\n  C      = %4d (before)",C);
       printf("\n  N      = %4d",N);
       printf("\n  G      = %4d\n",G);
     }

  /*--------------------------------------------------------------------------*/
  /*  Compute the normalized time, then load the Tchebeyshev coefficients     */
  /*  into array A[]. If T_span is covered by a single granule this is easy.  */
  /*  If not, the granule that contains the interpolation time is found, and  */
  /*  an offset from the array entry point for the ephemeris body is used to  */
  /*  load the coefficients.                                                  */
  /*--------------------------------------------------------------------------*/

  if ( G == 1 )
     {
       Tc = 2.0*(Time - T_beg) / T_span - 1.0;
     }
  else if ( G > 1 )
     {
       T_sub = T_span / ((double) G);          /* Compute subgranule interval */
       
       for ( j=G ; j>0 ; j-- ) 
           {
             T_break = T_beg + ((double) j-1) * T_sub;
             if ( Time > T_break ) 
                {
                  T_seg  = T_break;
                  offset = j-1;
                  break;
                }
            }
            
       Tc = 2.0*(Time - T_seg) / T_sub - 1.0;
       C  = C + 3 * offset * N;
     }
  else                                   /* Something has gone terribly wrong */
     {
       printf("\n Number of granules must be >= 1: check header data.\n");
     }

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  C      = %4d (after)",C);
       printf("\n  offset = %4d",offset);
       printf("\n  Time   = %12.7f",Time);
       printf("\n  T_sub  = %12.7f",T_sub);
       printf("\n  T_seg  = %12.7f",T_seg);
       printf("\n  Tc     = %12.7f\n",Tc);
       printf("\n  Array Coefficients:\n");
       for ( i=0 ; i<3*N ; i++ )
           {
			   printf("\n  A[%2d] = % 22.15e",i,Coeff_Array[C+i]);
           }
       printf("\n\n");
     }

  /*..........................................................................*/

  /*--------------------------------------------------------------------------*/
  /* Compute interpolated the position.                                       */
  /*--------------------------------------------------------------------------*/
  
	for ( i=0 ; i<3 ; i++ ) 
	{                           
		Cp[0]  = 1.0;                                 /* Begin polynomial sum */
		Cp[1]  = Tc;
		Position[i] = Coeff_Array[C+i*N] + Coeff_Array[C+1+i*N]*Tc;

		for ( j=2 ; j<N ; j++ )                                  /* Finish it */
		{
			Cp[j]  = 2.0 * Tc * Cp[j-1] - Cp[j-2];
			Position[i] = Position[i] + Coeff_Array[C+j+i*N] * Cp[j];
		}
	}

  return;
}

/**==========================================================================**/
/**  Interpolate_State                                                       **/
/**                                                                          **/
/**     This function computes position and velocity vectors for a selected  **/
/**     planetary body from Chebyshev coefficients that are read in from an  **/
/**     ephemeris data file. These coefficients are read from the data file  **/
/**     by calling the function Read_Coefficients (when necessary).          **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**     Time     -- Time for which position is desired (Julian Date).        **/
/**     Target   -- Solar system body for which position is desired.         **/
/**     Position -- Pointer to external array to receive the position.       **/
/**         	 0---Mercury                                                 **/
/**         	 1---Venus                                                   **/
/**         	 2---E-M Bary                                                **/
/**         	 3---Mars                                                    **/
/**         	 4---Jupiter                                                 **/
/**         	 5---Saturn                                                  **/
/**         	 6---Uranus                                                  **/
/**         	 7---Neptune                                                 **/
/**         	 8---Pluto                                                   **/
/**         	 9---Moon                                                    **/
/**         	10---Sun                                                     **/
/**                                                                          **/
/**  Returns: Nothing (explicitly)                                           **/
/**==========================================================================**/

void Interpolate_State(double Time , int Target, stateType *Planet)
{
  double    Cp[50] , Up[50] , T_break , T_seg , T_sub  , Tc;
  int       i , j;
  long int  C , G , N , offset = 0;

  /*--------------------------------------------------------------------------*/
  /* This function doesn't "do" nutations or librations.                      */
  /*--------------------------------------------------------------------------*/

  if ( Target >= 11 )             /* Also protects against weird input errors */
     {
       printf("\n This function does not compute nutations or librations.\n");
       return;
     }

  /*--------------------------------------------------------------------------*/
  /* Determine if a new record needs to be input.                             */
  /*--------------------------------------------------------------------------*/
  
  if (Time < T_beg || Time > T_end)  Read_Coefficients(Time);

  /*--------------------------------------------------------------------------*/
  /* Read the coefficients from the binary record.                            */
  /*--------------------------------------------------------------------------*/
  
  C = R1.coeffPtr[Target][0] - 1;               /*    Coeff array entry point */
  N = R1.coeffPtr[Target][1];                   /*          Number of coeff's */
  G = R1.coeffPtr[Target][2];                   /* Granules in current record */

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  In: Interpolate_State\n");
       printf("\n  Target = %2d",Target);
       printf("\n  C      = %4d (before)",C);
       printf("\n  N      = %4d",N);
       printf("\n  G      = %4d\n",G);
     }

  /*--------------------------------------------------------------------------*/
  /*  Compute the normalized time, then load the Tchebeyshev coefficients     */
  /*  into array A[]. If T_span is covered by a single granule this is easy.  */
  /*  If not, the granule that contains the interpolation time is found, and  */
  /*  an offset from the array entry point for the ephemeris body is used to  */
  /*  load the coefficients.                                                  */
  /*--------------------------------------------------------------------------*/

  if ( G == 1 )
     {
       Tc = 2.0*(Time - T_beg) / T_span - 1.0;
     }
  else if ( G > 1 )
     {
       T_sub = T_span / ((double) G);          /* Compute subgranule interval */
       
       for ( j=G ; j>0 ; j-- ) 
           {
             T_break = T_beg + ((double) j-1) * T_sub;
             if ( Time > T_break ) 
                {
                  T_seg  = T_break;
                  offset = j-1;
                  break;
                }
            }
            
       Tc = 2.0*(Time - T_seg) / T_sub - 1.0;
       C  = C + 3 * offset * N;
     }
  else                                   /* Something has gone terribly wrong */
     {
       printf("\n Number of granules must be >= 1: check header data.\n");
     }

  /*...................................................Debug print (optional) */

  if ( Debug )
     {
       printf("\n  C      = %4d (after)",C);
       printf("\n  offset = %4d",offset);
       printf("\n  Time   = %12.7f",Time);
       printf("\n  T_sub  = %12.7f",T_sub);
       printf("\n  T_seg  = %12.7f",T_seg);
       printf("\n  Tc     = %12.7f\n",Tc);
       printf("\n  Array Coefficients:\n");
       for ( i=0 ; i<3*N ; i++ )
           {
			   printf("\n  A[%2d] = % 22.15e",i,Coeff_Array[C+i]);
           }
       printf("\n\n");
     }

  /*..........................................................................*/

  /*--------------------------------------------------------------------------*/
  /* Compute the interpolated position & velocity                             */
  /*--------------------------------------------------------------------------*/
  
	for ( i=0 ; i<3 ; i++ )                /* Compute interpolating polynomials */
	{
		Cp[0] = 1.0;           
		Cp[1] = Tc;
		Cp[2] = 2.0 * Tc*Tc - 1.0;

		Up[0] = 0.0;
		Up[1] = 1.0;
		Up[2] = 4.0 * Tc;

		for ( j=3 ; j<N ; j++ )
		{
			Cp[j] = 2.0 * Tc * Cp[j-1] - Cp[j-2];
			Up[j] = 2.0 * Tc * Up[j-1] + 2.0 * Cp[j-1] - Up[j-2];
		}

		Planet->Position[i] = 0.0;           /* Compute interpolated position & velocity */
		Planet->Velocity[i] = 0.0;

		for ( j=N-1 ; j>=0 ; j-- )  Planet->Position[i] = Planet->Position[i] + Coeff_Array[C+j+i*N] * Cp[j];
		for ( j=N-1 ; j>=1 ; j-- )  Planet->Velocity[i] = Planet->Velocity[i] + Coeff_Array[C+j+i*N] * Up[j];

		Planet->Velocity[i] = Planet->Velocity[i] * 2.0 * ((double) G) / (T_span * 86400.0);
	}	
  return;
}

/********************************************************* END: ephem_read.c **/

