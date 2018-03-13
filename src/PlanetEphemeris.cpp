#include "PlanetEphemeris.h"
#include <math.h>
#include <Utility.h>

static bool Debug = false;             /* Generates detailed output if true */

DE405* DE405::theInstance = NULL;

DE405::DE405(void)
	:T_beg(0),T_end(0),T_span(0)
{
	std::string DirName = GetOrbitDynDir(); // 读取环境变量OrbitDyn，即data目录所在位置
#if __APPLE__
    DirName += "/data/planetEph.405";
#else
    DirName += "\\data\\planetEph.405";
#endif
	const char *Name = DirName.c_str();
	Initialize_Ephemeris(Name);
}

DE405::~DE405(void)
{
}


DE405* DE405::Instance()
{
	if(theInstance==NULL)
		theInstance = new DE405;
	return theInstance;
}

/*!将枚举型变量PLANET转化为DE405中对应的整数序号
该整数序号的前12个(0-11)对应recOneType::coeffPtr数组的行
整数序号中的12对应recOneType::libratPtr
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
/**         	11---Nutation                                                **/
/**         	12---Libration                                               **/
int DE405::PLANET2int( PLANET p )
{
	int i;
	switch(p)
	{
	case MERCURY:
	case VENUS:
	case MARS:
	case JUPITER:
	case SATURN:
	case URANUS:
	case NEPTUNE:
	case PLUTO:
	case MOON:
	case SUN:
		i = (int)p;	break;
	case EARTH_MOON_BARYCENTER:
		i = 2;	break;
	case NUTATIONS:
		i = 11;	break;
	case LIBRATIONS:
		i = 12;	break;
	case EARTH:
	case SOLAR_SYSTEM_BARYCENTER:
	default:
		throw DE405Exception("has not this planet in DE405!");
	}
	return i;
}


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
void DE405::Read_Coefficients( double Time )
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
int DE405::Initialize_Ephemeris(const char *fileName )
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
		DE405Exception expt;
		expt.SetDetails("Initialize_Ephemeris: Unable to open ephemeris file: %s",fileName);
		throw expt;
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

		if ( headerID == 405 ) 
		{
			return 0;
		}
		else 
		{
			DE405Exception expt;
			expt.SetDetails("Initialize_Ephemeris: Opened wrong file: %s  for ephemeris: %d.",fileName,405);
			throw expt;
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
/**     Libration -- Pointer to external array to receive the answer.        **/
/**                  Libration[0] : Omega                                    **/
/**                  Libration[1] : i                                        **/
/**                  Libration[2] : u                                        **/
/**                  Libration[3] : Omega_dot                                **/
/**                  Libration[4] : i_dot                                    **/
/**                  Libration[5] : u_dot                                    **/
/**                                                                          **/
/**  Returns: Nothing explicitly.                                            **/
/**==========================================================================**/
void DE405::Interpolate_Libration( double Time , int Target , double Libration[6] )
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function only computes librations.                                  */
	/*--------------------------------------------------------------------------*/

	if ( Target != 12 )             /* Also protects against weird input errors */
	{
		throw DE405Exception("Interpolate_Libration: This function only computes librations.");
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
		throw DE405Exception("Interpolate_Libration: Number of granules must be >= 1: check header data.");
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
void DE405::Interpolate_Nutation( double Time , int Target , double Nutation[4] )
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function only computes nutations.                                   */
	/*--------------------------------------------------------------------------*/

	if ( Target != 11 )             /* Also protects against weird input errors */
	{
		throw DE405Exception("Interpolate_Nutation: This function only computes nutations.");
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
		throw DE405Exception("Interpolate_Nutation: Number of granules must be >= 1: check header data.");
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
void DE405::Interpolate_Position( double Time , int Target , double Position[3] )
{
	double    Cp[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function doesn't "do" nutations or librations.                      */
	/*--------------------------------------------------------------------------*/

	if ( Target >= 11 )             /* Also protects against weird input errors */
	{
		throw DE405Exception("Interpolate_Position: This function does not compute nutations or librations.");
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
		throw DE405Exception("Interpolate_Position: Number of granules must be >= 1: check header data.");
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
/**     Position -- Pointer to external array to receive the position.       **/
/**                                                                          **/
/**  Returns: Nothing (explicitly)                                           **/
/**==========================================================================**/
void DE405::Interpolate_State(double Time , int Target, stateType *Planet)
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub  , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function doesn't "do" nutations or librations.                      */
	/*--------------------------------------------------------------------------*/

	if ( Target >= 11 )             /* Also protects against weird input errors */
	{
		throw DE405Exception("Interpolate_State: This function does not compute nutations or librations.");
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
		throw DE405Exception("Interpolate_State: Number of granules must be >= 1: check header data.");
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


void DE405::PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,double cart[6])
{
	double TDT = t.GetTDT() + 2400000.5;
	if( TDT < R1.timeData[0] || TDT > R1.timeData[1] )
	{
		DE405Exception e;
		e.SetDetails("time %lf is not in ephemeris file!",TDT);
		throw e;
	}
	stateType targetStatus;
	stateType centStatus;

	if(target == cent)
	{
		for(int i = 0; i < 6; ++i)
			cart[i] = 0.;
		return;
	}

	if(cent==EARTH && target==MOON)
	{
		Interpolate_State(TDT,PLANET2int(MOON),&targetStatus);
		for (int i=0;i<3;i++)
		{
			cart[i] = targetStatus.Position[i];
			cart[i+3] = targetStatus.Velocity[i];
		}
		return;
	}
	
	if(cent==MOON && target==EARTH)
	{
		Interpolate_State(TDT,PLANET2int(MOON),&targetStatus);
		for (int i=0;i<3;i++)
		{
			cart[i] = -targetStatus.Position[i];
			cart[i+3] = -targetStatus.Velocity[i];
		}
		return;
	}
	
	// 中心天体在太阳系质心系中的状态
	if(cent==EARTH)
		Earth_SSBARY(TDT,&centStatus);
	else if(cent==MOON)
		Moon_SSBARY(TDT,&centStatus);
	else if(cent!=SOLAR_SYSTEM_BARYCENTER)
		Interpolate_State(TDT,PLANET2int(cent),&centStatus);
	else // center is SOLAR_SYSTEM_BARYCENTER
	{
		for (int i=0;i<3;i++)
		{
			centStatus.Position[i] = 0;
			centStatus.Velocity[i] = 0;
		}
	}

	// 目标天体在太阳系质心系中的状态
	if(target==EARTH)
		Earth_SSBARY(TDT,&targetStatus);
	else if(target==MOON)
		Moon_SSBARY(TDT,&targetStatus);
	else if(target!=SOLAR_SYSTEM_BARYCENTER)
		Interpolate_State(TDT,PLANET2int(target),&targetStatus);
	else // center is SOLAR_SYSTEM_BARYCENTER
	{
		for (int i=0;i<3;i++)
		{
			targetStatus.Position[i] = 0;
			targetStatus.Velocity[i] = 0;
		}
	}

	for (int i=0;i<3;i++)
	{
		cart[i] = targetStatus.Position[i] - centStatus.Position[i];
		cart[i+3] = targetStatus.Velocity[i] - centStatus.Velocity[i];
	}
}

void DE405::PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,vec3& r,vec3& v)
{
	double rrd[6];
	PlanetEphemeris(t,cent,target,rrd);
	r(0) = rrd[0];
	r(1) = rrd[1];
	r(2) = rrd[2];
	v(0) = rrd[3];
	v(1) = rrd[4];
	v(2) = rrd[5];
}

// 地球在地月质心系中的位置速度
void DE405::Earth_ESBARY(double JD,stateType* earth)
{
	stateType em;
	double fac = 1.0/(1.0+R1.EMRAT);
	Interpolate_State(JD,PLANET2int(MOON),&em);
	for (int i=0;i<3;i++)
	{
		earth->Position[i] = -em.Position[i]*fac;
		earth->Velocity[i] = -em.Velocity[i]*fac;
	}
}

// 月球在地月质心系中的位置速度
void DE405::Moon_ESBARY(double JD,stateType* moon)
{
	stateType em;
	double fac = R1.EMRAT/(1.0+R1.EMRAT);
	Interpolate_State(JD,PLANET2int(MOON),&em);
	for (int i=0;i<3;i++)
	{
		moon->Position[i] = em.Position[i]*fac;
		moon->Velocity[i] = em.Velocity[i]*fac;
	}
}

// 地球在太阳系质心系中的位置速度
void DE405::Earth_SSBARY(double JD,stateType* earth)
{
	stateType emb,e;
	// 地心在地月质心系中的状态
	Earth_ESBARY(JD,&e); 
	// 地月质心在太阳质心系中的状态
	Interpolate_State(JD,PLANET2int(EARTH_MOON_BARYCENTER),&emb); 
	for (int i=0;i<3;i++)
	{
		earth->Position[i] = emb.Position[i] + e.Position[i];
		earth->Velocity[i] = emb.Velocity[i] + e.Velocity[i];
	}
}

// 月球在太阳系质心系中的位置速度
void DE405::Moon_SSBARY(double JD,stateType* moon)
{
	stateType m,emb;
	Moon_ESBARY(JD,&m);
	Interpolate_State(JD,PLANET2int(EARTH_MOON_BARYCENTER),&emb);
	for (int i=0;i<3;i++)
	{
		moon->Position[i] = emb.Position[i] + m.Position[i];
		moon->Velocity[i] = emb.Velocity[i] + m.Velocity[i];
	}
}

// 地心月球矢量
vec3 DE405::Moon(const CDateTime& t)
{
	double r[3];
	double TDT = t.GetTDT() + 2400000.5;
	if( TDT < R1.timeData[0] || TDT > R1.timeData[1] )
	{
		DE405Exception e;
		e.SetDetails("time %lf is not in plteph.405 file!",TDT);
		throw e;
	}
	Interpolate_Position(TDT,PLANET2int(MOON),r);
	vec3 v;
	v(0) = r[0];
	v(1) = r[1];
	v(2) = r[2];
	return v;
}

// 地心太阳矢量
vec3 DE405::Sun(const CDateTime& t)
{
	double r[6];
	PlanetEphemeris(t,EARTH,SUN,r);
	vec3 v;
	v(0) = r[0];
	v(1) = r[1];
	v(2) = r[2];
	return v;
}

/*!章动系数
\param t CDatetime时间
\param dksi 黄经章动 ( unit: radian )
\param deps 交角章动 ( unit: radian )
*/
void DE405::Nutations(const CDateTime& t,double& dksi,double& deps)
{
	double pv[4];
	double TDT = t.GetTDT() + 2400000.5;
    Interpolate_Nutation(TDT,PLANET2int(NUTATIONS),pv);
	dksi = pv[0];
	deps = pv[1];
}

/*!月固系相对月心惯性系(地心惯性系)的三个欧拉转角
\param t CDatetime时间
\param Omega Omega(unit: radian)
\param i i(unit: radian)
\param u u(unit: radian)
*/
void DE405::Librations(const CDateTime& t,double& Omega,double& i,double& u)
{
	double pv[6];
	double TDT = t.GetTDT() + 2400000.5;
	Interpolate_Libration(TDT,PLANET2int(LIBRATIONS),pv);
    Omega = pv[0];
	i = pv[1];
	u = pv[2];
}

/************************************************************************/
/* DE421                                                                */
/************************************************************************/
DE421* DE421::theInstance = NULL;

DE421::DE421(void)
	:T_beg(0),T_end(0),T_span(0)
{
	std::string DirName = GetOrbitDynDir(); // 读取环境变量OrbitDyn，即data目录所在位置
#if __APPLE__
    DirName += "/data/planetEph.421";
#else
    DirName += "\\data\\planetEph.421";
#endif
	const char *Name = DirName.c_str();
	Initialize_Ephemeris(Name);
}

DE421::~DE421(void)
{
}


DE421* DE421::Instance()
{
	if(theInstance==NULL)
		theInstance = new DE421;
	return theInstance;
}

/*!将枚举型变量PLANET转化为DE421中对应的整数序号
该整数序号的前12个(0-11)对应recOneType::coeffPtr数组的行
整数序号中的12对应recOneType::libratPtr
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
/**         	11---Nutation                                                **/
/**         	12---Libration                                               **/
int DE421::PLANET2int( PLANET p )
{
	int i;
	switch(p)
	{
	case MERCURY:
	case VENUS:
	case MARS:
	case JUPITER:
	case SATURN:
	case URANUS:
	case NEPTUNE:
	case PLUTO:
	case MOON:
	case SUN:
		i = (int)p;	break;
	case EARTH_MOON_BARYCENTER:
		i = 2;	break;
	case NUTATIONS:
		i = 11;	break;
	case LIBRATIONS:
		i = 12;	break;
	case EARTH:
	case SOLAR_SYSTEM_BARYCENTER:
	default:
		throw DE421Exception("has not this planet in DE421!");
	}
	return i;
}


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
void DE421::Read_Coefficients( double Time )
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
int DE421::Initialize_Ephemeris(const char *fileName )
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
		DE421Exception expt;
		expt.SetDetails("Initialize_Ephemeris: Unable to open ephemeris file: %s",fileName);
		throw expt;
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

		if ( headerID == 421 ) 
		{
			return 0;
		}
		else 
		{
			DE421Exception expt;
			expt.SetDetails("Initialize_Ephemeris: Opened wrong file: %s  for ephemeris: %d",fileName,421);
			throw expt;
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
/**     Libration -- Pointer to external array to receive the answer.        **/
/**                  Libration[0] : Omega                                    **/
/**                  Libration[1] : i                                        **/
/**                  Libration[2] : u                                        **/
/**                  Libration[3] : Omega_dot                                **/
/**                  Libration[4] : i_dot                                    **/
/**                  Libration[5] : u_dot                                    **/
/**                                                                          **/
/**  Returns: Nothing explicitly.                                            **/
/**==========================================================================**/
void DE421::Interpolate_Libration( double Time , int Target , double Libration[6] )
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function only computes librations.                                  */
	/*--------------------------------------------------------------------------*/

	if ( Target != 12 )             /* Also protects against weird input errors */
	{
		throw DE421Exception("Interpolate_Libration: This function only computes librations.");
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
		throw DE421Exception("Interpolate_Libration: Number of granules must be >= 1: check header data.");
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
void DE421::Interpolate_Nutation( double Time , int Target , double Nutation[4] )
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function only computes nutations.                                   */
	/*--------------------------------------------------------------------------*/

	if ( Target != 11 )             /* Also protects against weird input errors */
	{
		throw DE421Exception("Interpolate_Nutation: This function only computes nutations.");
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
		throw DE421Exception("Interpolate_Nutation: Number of granules must be >= 1: check header data.");
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
void DE421::Interpolate_Position( double Time , int Target , double Position[3] )
{
	double    Cp[50] , T_break , T_seg , T_sub , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function doesn't "do" nutations or librations.                      */
	/*--------------------------------------------------------------------------*/

	if ( Target >= 11 )             /* Also protects against weird input errors */
	{
		throw DE421Exception("Interpolate_Position: This function does not compute nutations or librations.");
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
		throw DE421Exception("Interpolate_Position: Number of granules must be >= 1: check header data.");
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
/**     Position -- Pointer to external array to receive the position.       **/
/**                                                                          **/
/**  Returns: Nothing (explicitly)                                           **/
/**==========================================================================**/
void DE421::Interpolate_State(double Time , int Target, stateType *Planet)
{
	double    Cp[50] , Up[50] , T_break , T_seg , T_sub  , Tc;
	int       i , j;
	long int  C , G , N , offset = 0;

	/*--------------------------------------------------------------------------*/
	/* This function doesn't "do" nutations or librations.                      */
	/*--------------------------------------------------------------------------*/

	if ( Target >= 11 )             /* Also protects against weird input errors */
	{
		throw DE421Exception("Interpolate_State: This function does not compute nutations or librations.");
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
		throw DE421Exception("Interpolate_State: Number of granules must be >= 1: check header data.");
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


void DE421::PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,double cart[6])
{
	double TDT = t.GetTDT() + 2400000.5;
	if( TDT < R1.timeData[0] || TDT > R1.timeData[1] )
	{
		DE421Exception e;
		e.SetDetails("time %lf is not in ephemeris file!",TDT);
		throw e;
	}
	stateType targetStatus;
	stateType centStatus;

	if(target == cent)
	{
		for(int i = 0; i < 6; ++i)
			cart[i] = 0.;
		return;
	}

	if(cent==EARTH && target==MOON)
	{
		Interpolate_State(TDT,PLANET2int(MOON),&targetStatus);
		for (int i=0;i<3;i++)
		{
			cart[i] = targetStatus.Position[i];
			cart[i+3] = targetStatus.Velocity[i];
		}
		return;
	}

	if(cent==MOON && target==EARTH)
	{
		Interpolate_State(TDT,PLANET2int(MOON),&targetStatus);
		for (int i=0;i<3;i++)
		{
			cart[i] = -targetStatus.Position[i];
			cart[i+3] = -targetStatus.Velocity[i];
		}
		return;
	}

	// 中心天体在太阳系质心系中的状态
	if(cent==EARTH)
		Earth_SSBARY(TDT,&centStatus);
	else if(cent==MOON)
		Moon_SSBARY(TDT,&centStatus);
	else if(cent!=SOLAR_SYSTEM_BARYCENTER)
		Interpolate_State(TDT,PLANET2int(cent),&centStatus);
	else // center is SOLAR_SYSTEM_BARYCENTER
	{
		for (int i=0;i<3;i++)
		{
			centStatus.Position[i] = 0;
			centStatus.Velocity[i] = 0;
		}
	}

	// 目标天体在太阳系质心系中的状态
	if(target==EARTH)
		Earth_SSBARY(TDT,&targetStatus);
	else if(target==MOON)
		Moon_SSBARY(TDT,&targetStatus);
	else if(target!=SOLAR_SYSTEM_BARYCENTER)
		Interpolate_State(TDT,PLANET2int(target),&targetStatus);
	else // center is SOLAR_SYSTEM_BARYCENTER
	{
		for (int i=0;i<3;i++)
		{
			targetStatus.Position[i] = 0;
			targetStatus.Velocity[i] = 0;
		}
	}

	for (int i=0;i<3;i++)
	{
		cart[i] = targetStatus.Position[i] - centStatus.Position[i];
		cart[i+3] = targetStatus.Velocity[i] - centStatus.Velocity[i];
	}
}

void DE421::PlanetEphemeris(const CDateTime& t,PLANET cent,PLANET target,vec3& r,vec3& v)
{
	double rrd[6];
	PlanetEphemeris(t,cent,target,rrd);
	r(0) = rrd[0];
	r(1) = rrd[1];
	r(2) = rrd[2];
	v(0) = rrd[3];
	v(1) = rrd[4];
	v(2) = rrd[5];
}

// 地球在地月质心系中的位置速度
void DE421::Earth_ESBARY(double JD,stateType* earth)
{
	stateType em;
	double fac = 1.0/(1.0+R1.EMRAT);
	Interpolate_State(JD,PLANET2int(MOON),&em);
	for (int i=0;i<3;i++)
	{
		earth->Position[i] = -em.Position[i]*fac;
		earth->Velocity[i] = -em.Velocity[i]*fac;
	}
}

// 月球在地月质心系中的位置速度
void DE421::Moon_ESBARY(double JD,stateType* moon)
{
	stateType em;
	double fac = R1.EMRAT/(1.0+R1.EMRAT);
	Interpolate_State(JD,PLANET2int(MOON),&em);
	for (int i=0;i<3;i++)
	{
		moon->Position[i] = em.Position[i]*fac;
		moon->Velocity[i] = em.Velocity[i]*fac;
	}
}

// 地球在太阳系质心系中的位置速度
void DE421::Earth_SSBARY(double JD,stateType* earth)
{
	stateType emb,e;
	// 地心在地月质心系中的状态
	Earth_ESBARY(JD,&e); 
	// 地月质心在太阳质心系中的状态
	Interpolate_State(JD,PLANET2int(EARTH_MOON_BARYCENTER),&emb); 
	for (int i=0;i<3;i++)
	{
		earth->Position[i] = emb.Position[i] + e.Position[i];
		earth->Velocity[i] = emb.Velocity[i] + e.Velocity[i];
	}
}

// 月球在太阳系质心系中的位置速度
void DE421::Moon_SSBARY(double JD,stateType* moon)
{
	stateType m,emb;
	Moon_ESBARY(JD,&m);
	Interpolate_State(JD,PLANET2int(EARTH_MOON_BARYCENTER),&emb);
	for (int i=0;i<3;i++)
	{
		moon->Position[i] = emb.Position[i] + m.Position[i];
		moon->Velocity[i] = emb.Velocity[i] + m.Velocity[i];
	}
}

// 地心月球矢量
vec3 DE421::Moon(const CDateTime& t)
{
	double r[3];
	double TDT = t.GetTDT() + 2400000.5;
	if( TDT < R1.timeData[0] || TDT > R1.timeData[1] )
	{
		DE421Exception e;
		e.SetDetails("time %lf is not in plteph.405 file!",TDT);
		throw e;
	}
	Interpolate_Position(TDT,PLANET2int(MOON),r);
	vec3 v;
	v(0) = r[0];
	v(1) = r[1];
	v(2) = r[2];
	return v;
}

// 地心太阳矢量
vec3 DE421::Sun(const CDateTime& t)
{
	double r[6];
	PlanetEphemeris(t,EARTH,SUN,r);
	vec3 v;
	v(0) = r[0];
	v(1) = r[1];
	v(2) = r[2];
	return v;
}

/*!章动系数
\param t CDatetime时间
\param dksi 黄经章动 ( unit: radian )
\param deps 交角章动 ( unit: radian )
*/
void DE421::Nutations(const CDateTime& t,double& dksi,double& deps)
{
	double pv[4];
	double TDT = t.GetTDT() + 2400000.5;
	Interpolate_Nutation(TDT,PLANET2int(NUTATIONS),pv);
	dksi = pv[0];
	deps = pv[1];
}

/*!月固系相对月心惯性系(地心惯性系)的三个欧拉转角
\param t CDatetime时间
\param Omega Omega(unit: radian)
\param i i(unit: radian)
\param u u(unit: radian)
*/
void DE421::Librations(const CDateTime& t,double& Omega,double& i,double& u)
{
	double pv[6];
	double TDT = t.GetTDT() + 2400000.5;
	Interpolate_Libration(TDT,PLANET2int(LIBRATIONS),pv);
	Omega = pv[0];
	i = pv[1];
	u = pv[2];
}

