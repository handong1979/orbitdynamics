/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: rdeph.c                                                    **/
/**                                                                          **/
/**      Purpose: This program interpolates a state from data in a JPL       **/
/**               ephemeris file for a given Julian date. It is designed     **/
/**               to be called from a Tcl/Tk script, so it has a very        **/
/**               unfriendly user interface.                                 **/
/**                                                                          **/
/**               For convenience, here is the mapping between the command   **/
/**               line inputs and the global variables found in read.tcl:    **/
/**                                                                          **/
/**                        argv[1]  <-->  EphName                            **/
/**                        argv[2]  <-->  JD                                 **/
/**                        argv[3]  <-->  TN                                 **/
/**                        argv[4]  <-->  FmtOpt                             **/
/**                        argv[5]  <-->  Cnt                                **/
/**                                                                          **/
/**   Programmer: David Hoffman/EG5                                          **/
/**               NASA, Johnson Space Center                                 **/
/**               Houston, TX 77058                                          **/
/**               e-mail: david.a.hoffman1@jsc.nasa.gov                      **/
/**                                                                          **/
/**==========================================================================**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ephem_read.h"
#ifndef TYPES_DEFINED
#include "ephem_types.h"
#endif
                
main (argc, argv)
  char *argv[];
   int argc;
{
  stateType  State;
  char       *tgtName;
  double     Position[3] , Time;
  int        i , Target;

  /* Convert command line arguments to numeric values.........................*/
  
  Time   = atof(argv[2]);
  Time = Time + 2400000.5 + 64.184/86400;
  Target = atoi(argv[3]);

  /* Initialize the ephemeris.................................................*/

  Initialize_Ephemeris(argv[1]);

  /* Compute the desired ephemeris data.......................................*/

  if ( !strcmp(argv[4],"PosVel") ) 
      Interpolate_State( Time , Target , &State );
  else if ( !strcmp(argv[4],"Pos") ) 
      Interpolate_Position( Time , Target , Position );
  else if ( !strcmp(argv[4],"Lib") )
	  Interpolate_Libration(Time, 12, Position);
  else if ( !strcmp(argv[4],"Nut") )
	  Interpolate_Nutation(Time, 11, Position);

  /* Print the answer.........................................................*/

  if ( !strcmp(argv[4],"PosVel") ) 
     {
       /* Print seperator.....................................................*/
       
       for ( i=0 ; i<21 ; i++ ) putchar('-');
       printf("  Case %2s  ",argv[5]);
       for ( i=0 ; i<21 ; i++ ) putchar('-');
       
       /* Print planet name...................................................*/
       
       switch (Target) {
          case  0: tgtName = "Mercury";
                   break;
          case  1: tgtName = "Venus";
                   break;
          case  2: tgtName = "Earth-Moon Barycenter";
                   break;
          case  3: tgtName = "Mars";
                   break;
          case  4: tgtName = "Jupiter";
                   break;
          case  5: tgtName = "Saturn";
                   break;
          case  6: tgtName = "Uranus";
                   break;
          case  7: tgtName = "Neptune";
                   break;
          case  8: tgtName = "Pluto";
                   break;
          case  9: tgtName = "Moon";
                   break;
          case 10: tgtName = "Sun";
                   break;
          default: tgtName = "Mercury";
                   break;
       }
       
       printf("\n\n  Target:  %s",tgtName);
       printf("\n      JD:  %8.2f",Time);

       /* Print position......................................................*/
       
       printf("\n\n  Position (km):     [1] =  % 22.15g",State.Position[0]);
       printf("\n                     [2] =  % 22.15g",State.Position[1]);
       printf("\n                     [3] =  % 22.15g",State.Position[2]);

       /* Print velocity......................................................*/

       printf("\n\n  Velocity (km/sec): [1] =  % 22.15g",State.Velocity[0]);
       printf("\n                     [2] =  % 22.15g",State.Velocity[1]);
       printf("\n                     [3] =  % 22.15g\n\n\n",State.Velocity[2]);
     }
  else if( !strcmp(argv[4],"Pos") )
     {
       /* Print seperator.....................................................*/
       
       for ( i=0 ; i<21 ; i++ ) putchar('-');
       printf("  Case %2s  ",argv[5]);
       for ( i=0 ; i<21 ; i++ ) putchar('-');

       /* Print planet name...................................................*/
       
       switch (Target) {
		 case  0: tgtName = "Mercury";
			 break;
		 case  1: tgtName = "Venus";
			 break;
		 case  2: tgtName = "Earth-Moon Barycenter";
			 break;
		 case  3: tgtName = "Mars";
			 break;
		 case  4: tgtName = "Jupiter";
			 break;
		 case  5: tgtName = "Saturn";
			 break;
		 case  6: tgtName = "Uranus";
			 break;
		 case  7: tgtName = "Neptune";
			 break;
		 case  8: tgtName = "Pluto";
			 break;
		 case  9: tgtName = "Moon";
			 break;
		 case 10: tgtName = "Sun";
			 break;
		 default: tgtName = "Mercury";
			 break;
       }
       
       printf("\n\n  Target:  %s",tgtName);
       printf("\n      JD:  %8.2f",Time);

       /* Print position......................................................*/
       
       printf("\n\n  Position (km):     [1] =  % 22.15g",Position[0]);
       printf("\n                     [2] =  % 22.15g",Position[1]);
       printf("\n                     [3] =  % 22.15g\n\n\n",Position[2]);
     }
  else if( !strcmp(argv[4],"Lib") )
  {
	  /* Print seperator.....................................................*/

	  for ( i=0 ; i<21 ; i++ ) putchar('-');
	  printf("  Case %2s  ",argv[5]);
	  for ( i=0 ; i<21 ; i++ ) putchar('-');
	  printf("\n      JD:  %16.7f",Time);

	  /* Print position......................................................*/
	  printf("\n\n  Target:  %s","Libration");
	  printf("\n\n  Libration (rad):     [1] =  % 22.15g",Position[0]);
	  printf("\n                       [2] =  % 22.15g",Position[1]);
	  printf("\n                       [3] =  % 22.15g\n\n\n",Position[2]);

	  /* Print velocity......................................................*/

	  printf("\n\n  Libration rate (rad/sec): [1] =  % 22.15g",Position[3]);
	  printf("\n                            [2] =  % 22.15g",Position[4]);
	  printf("\n                            [3] =  % 22.15g\n\n\n",Position[5]);
  }
  else if( !strcmp(argv[4],"Nut") )
  {
	  /* Print seperator.....................................................*/

	  for ( i=0 ; i<21 ; i++ ) putchar('-');
	  printf("  Case %2s  ",argv[5]);
	  for ( i=0 ; i<21 ; i++ ) putchar('-');
	  printf("\n      JD:  %16.7f",Time);

	  /* Print position......................................................*/
	  printf("\n\n  Target:  %s","Nutation");
	  printf("\n\n  Nutation (rad):     [1] =  % 22.15g",Position[0]);
	  printf("\n                      [2] =  % 22.15g\n\n\n",Position[1]);

	  /* Print velocity......................................................*/

	  printf("\n\n  Nutation rate (rad/sec): [1] =  % 22.15g",Position[2]);
	  printf("\n                           [2] =  % 22.15g\n\n\n",Position[3]);
  }

  /* Exit normally............................................................*/
  
  exit(0);
}
