/**==========================================================================**/
/**  Source File: jdcon.c                                                    **/
/**                                                                          **/
/**      Purpose: This program converts a Gregorian calendar date to a       **/
/**               Julian day number. It is designed to be called from a      **/
/**               Tcl/Tk script, so it has a very unfriendly user interface. **/
/**                                                                          **/
/**   Programmer: David Hoffman/EG5                                          **/
/**               NASA, Johnson Space Center                                 **/
/**               Houston, TX 77058                                          **/
/**               e-mail: david.a.hoffman1@jsc.nasa.gov                      **/
/**                                                                          **/
/**==========================================================================**/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ephem_util.h"
                
main (argc, argv)
  char *argv[];
   int argc;
{
   double  juldat, sec;
      int  year, month, day, hour, min;

   /* Quit if inputs are not provided.........................................*/
   
   if ( argc != 7 )
      {
        printf("Try again.");
        exit(1);
      }
   
   /* Set calling arguments...................................................*/
   
   year  = atoi(argv[1]);
   month = atoi(argv[2]);
   day   = atoi(argv[3]);
   hour  = atoi(argv[4]);
   min   = atoi(argv[5]);
   sec   = atof(argv[6]);
   
   /* Call conversion function................................................*/
   
   juldat = Gregorian_to_Julian(year, month, day, hour, min, sec );
   
   /* Return result...........................................................*/
   
   printf("%9.3f",juldat);
   exit(0);
}
