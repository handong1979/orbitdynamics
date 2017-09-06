/**==========================================================================**/
/**                                                                          **/
/**  HEADER FILE: ephem_util.h                                               **/
/**                                                                          **/
/**     This file contains C prototype declarations for the functions that   **/
/**     are defined in the source file ephem_util.c; the purpose of each of  **/
/**     them is explained there.                                             **/
/**                                                                          **/
/**  Programmer:  David Hoffman/EG5                                          **/
/**               NASA, Johnson Space Center                                 **/
/**               Houston, TX 77058                                          **/
/**               e-mail: david.a.hoffman1@jsc.nasa.gov                      **/
/**                                                                          **/
/**==========================================================================**/

#include "ephem_types.h"

/*----------------------------------------------------------------------------*/
/*  Find_Value                                                                */
/*----------------------------------------------------------------------------*/

    extern double Find_Value( char    name[]             , 
							  char    name_array[CONST_NUM][CONST_NAME_LENGTH] , 
                              double  value_array[CONST_NUM]   );

/*----------------------------------------------------------------------------*/
/**  Gregorian_to_Julian                                                     **/
/*----------------------------------------------------------------------------*/

     extern double Gregorian_to_Julian( int     year ,   int     month   , 
                                        int     day  ,   int     hour    , 
                                        int     min  ,   double  seconds );

 /*----------------------------------------------------------------------------*/
 /**  Julian_to_Gregorian                                                     **/
 /*----------------------------------------------------------------------------*/

	 extern void gdcon(double JED,char* Gregorian);
/*----------------------------------------------------------------------------*/
/*  Integer modulo function.                                                  */
/*----------------------------------------------------------------------------*/

    extern int mod( int x, int y );

/*----------------------------------------------------------------------------*/
/*  Read_File_Line                                                            */
/*----------------------------------------------------------------------------*/

    extern int Read_File_Line( FILE *inFile, int filter, char lineBuffer[82]);

/*----------------------------------------------------------------------------*/
/*  Read_Group_Header                                                         */
/*----------------------------------------------------------------------------*/

    extern int Read_Group_Header( FILE *inFile );

/*----------------------------------------------------------------------------*/
/*  Warning                                                                   */
/*----------------------------------------------------------------------------*/

    extern void Warning( int errorCode );
