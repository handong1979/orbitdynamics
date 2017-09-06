/******************************************************************************/
/**                                                                          **/
/**  SOURCE FILE: ephem_types.h                                              **/
/**                                                                          **/
/**    This file contains C macros and type definitions for this version of  **/
/**    the JPL ephemeris utilities. Note that the macro EPHEMERIS, defined   **/
/**    below, must be consistent with the ephemeris being read in order for  **/
/**    any of the C utilities to work properly. Note also that the addition  **/
/**    of new ephemerides to the FTP site might require new values for the   **/
/**    macro ARRAY_SIZE.                                                     **/
/**                                                                          **/
/**  Programmer: David Hoffman/EG5                                           **/
/**              NASA, Johnson Space Center                                  **/
/**              Houston, TX 77058                                           **/
/**              e-mail: david.a.hoffman1@jsc.nasa.gov                       **/
/**                                                                          **/
/**  History:                                                                **/
/**                                                                          **/
/**    8/11/99  Changed values of macros for planet names and added some     **/
/**             comments there. Thanks to Gary Benstein for pointing out     **/
/**             the need for these changes.                                  **/
/**                                                                          **/
/******************************************************************************/

/**==========================================================================**/
/**  C Macro Definitions                                                     **/
/**                                                                          **/
/**    The name of the ephemeris must be defined by the user. At the time    **/
/**    this software was created, the only  ephemerides available to the     **/
/**    public were DE200 and DE405. The ephemeris name is used to size the   **/ 
/**    records in the binary data file. These records are sized to accom-    **/
/**    odate a complete set of  interpolating coefficients in the form of    **/
/**    an array of double precision numbers. The parameter ARRAY_SIZE gives  **/
/**    the number of such coefficients in each record; it is used to size    **/
/**    the header records defined below. Finally, some C pre-processor       **/
/*     macros are defined for convenience.                                   **/
/**==========================================================================**/

#define EPHEMERIS 421                /* Note the obvious: input XXX for DEXXX */

#if EPHEMERIS==200  
#define ARRAY_SIZE 826
#elif EPHEMERIS==405
#define ARRAY_SIZE 1018
#define CONST_NUM 156
#define CONST_NAME_LENGTH 7
#define CONST_LABEL_LENGTH 60
#elif EPHEMERIS==421
#define ARRAY_SIZE 1018
#define CONST_NUM 228
#define CONST_NAME_LENGTH 7
#define CONST_LABEL_LENGTH 60
#endif

#if _MSC_VER && (_MSC_VER > 1300)
#pragma  warning(disable:4996)
#endif

#define TYPES_DEFINED 0  /* Dummy variable (see ephem_read.h for explanation) */

#define TRUE 1 
#define FALSE 0

#define FAILURE 1              /* Exception handling only needed for failures */
#define SUCCESS 0

//#define MERCURY 0
//#define VENUS 1
//#define EARTH 2                                      /* Earth-Moon Barycenter */
//#define MARS 3
//#define JUPITER 4
//#define SATURN 5
//#define URANUS 6
//#define NEPTUNE 7
//#define PLUTO 8
//#define MOON 9                                      /* Relative to geocenter */
//#define SUN 10
//#define SS_BARY 11
//#define EM_BARY 12
//#define NUTATIONS 13
//#define LIBRATIONS 14

/**==========================================================================**/
/**  C Record Type Definitions                                               **/
/**==========================================================================**/

   /*-------------------------------------------------------------------------*/
   /* Define ephemeris state type                                             */
   /*-------------------------------------------------------------------------*/

   struct stateData{
     double Position[3];
     double Velocity[3];
     };

   typedef struct stateData stateType;

   /*-------------------------------------------------------------------------*/
   /* Define the content of binary header records                             */
   /*-------------------------------------------------------------------------*/

   struct recOneData {
         char label[3][CONST_LABEL_LENGTH];
         char constName[CONST_NUM][CONST_NAME_LENGTH];
       double timeData[3];
     long int numConst;
       double AU;
       double EMRAT;
     long int coeffPtr[12][3];
     long int DENUM;
     long int libratPtr[3];
     };

   struct recTwoData {
     double constValue[CONST_NUM];
     }; 

   typedef struct recOneData recOneType;
   typedef struct recTwoData recTwoType;

   /*-------------------------------------------------------------------------*/
   /* Define binary header record formats                                     */
   /*-------------------------------------------------------------------------*/
/*
   struct headerOne {
       recOneType data;
       //char pad[ARRAY_SIZE*sizeof(double) - sizeof(recOneType)];
     };
  
   struct headerTwo {
       recTwoType data;
       //char pad[ARRAY_SIZE*sizeof(double) - sizeof(recTwoType)];
     };

   typedef struct headerOne headOneType;
   typedef struct headerTwo headTwoType;
*/
/******************************************************** END: ephem_types.h **/
