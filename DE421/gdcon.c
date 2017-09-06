/**==========================================================================**/
/**  Source File: gdcon.c                                                    **/
/**                                                                          **/
/**      Purpose: This program converts a Julian day number to a Gregorian   **/
/**               date. It is designed to be called from a Tcl/Tk script,    **/ 
/**               so it has a very unfriendly user interface.                **/
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
                
main (argc, argv)
  char *argv[];
   int argc;
{
    char  *Month;
  double  A, B, C, D, E, F, T, Z, Alpha, Frac, Second, Total, Whole;
     int  Year, Day, Hour, Minute, Mo;

  /* Add a half day to input Julian date......................................*/
  
  T = atof(argv[1]) + 0.5;
  Z = floor(T);
  F = T - Z;

  /* Preliminary calculations.................................................*/

  Alpha = floor((Z - 1867216.25)/36524.25);
  A     = 1.0 + Z + Alpha - floor(Alpha/4.0);
  B     = A + 1524.0;
  C     = floor((B - 122.1)/365.25);
  D     = floor(365.25*C);
  E     = floor((B - D)/30.6001);

  /* Compute the month........................................................*/

  if (E<14)
     Mo = ((int) E) - 1; 
  else 
     Mo = ((int) E) - 13;  

  switch (Mo) {
       case  1:  Month = "January";    break;
       case  2:  Month = "February";   break;
       case  3:  Month = "March";      break;
       case  4:  Month = "April";      break;
       case  5:  Month = "May";        break;
       case  6:  Month = "June";       break;
       case  7:  Month = "July";       break;
       case  8:  Month = "August";     break;
       case  9:  Month = "September";  break;
       case 10:  Month = "October";    break;
       case 11:  Month = "November";   break;
       case 12:  Month = "December";   break;
       default:  Month = "January";    break;
  }

  /* Compute the year.........................................................*/
  
  if (Mo>2)
     Year = ((int) C) - 4716;
  else
     Year = ((int) C) - 4715;
  
  /* Compute the day..........................................................*/
  
   Total = B - D + F - floor(30.6001*E);
   Whole = floor(Total);
   Frac  = Total - Whole;
   Day   = (int) Whole;
   
  /* Compute the hour.........................................................*/
   
   Total = Frac * 24.0;
   Whole = floor(Total);
   Frac  = Total - Whole;
   Hour  = (int) Whole;
   
  /* Compute the minute.......................................................*/
   
   Total  = Frac * 60.0;
   Whole  = floor(Total);
   Frac   = Total - Whole;
   Minute = (int) Whole;

  /* Compute the second.......................................................*/  

   Second = Frac * 60.0;

  /* Output the result........................................................*/

  printf("%4d %s %2d %2d %2d %2.3f",Year,Month,Day,Hour,Minute,Second);
   exit(0);
}
