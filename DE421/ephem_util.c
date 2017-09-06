/******************************************************************************/
/**                                                                          **/
/**  SOURCE FILE: ephem_util.c                                               **/
/**                                                                          **/
/**    This file contains source code for the utility functions used in the  **/
/**    ephemeris manipulation programs.                                      **/
/**                                                                          **/
/**  Programmer: David Hoffman/EG5                                           **/
/**              NASA, Johnson Space Center                                  **/
/**              Houston, TX 77058                                           **/
/**              e-mail: david.a.hoffman1@jsc.nasa.gov                       **/
/**                                                                          **/
/******************************************************************************/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ephem_types.h"

/**==========================================================================**/
/**  Warning                                                                 **/
/**                                                                          **/
/**    The purpose of this ubiquitous function is to print an error message. **/
/**    The message to be printed is determined by an integer passed from the **/
/**    calling routine.                                                      **/
/**                                                                          **/
/**  Input:   errorCode (integer)                                            **/
/**  Returns: Nothing                                                        **/
/**==========================================================================**/

void Warning(int errorCode)
{
   switch (errorCode)
      {
        case  1: printf("\n Improper number of command line arguments.\n\n");
                 break;

        case  2: printf("\n Unable to open header file.\n\n");
                 break;
                 
        case  3: printf("\n Unable to open ascii ephemeris file.\n\n");
                 break;
                 
        case  4: printf("\n Unable to open binary output ephemeris file.\n\n");
                 break;

        case  5: printf("\n Unable to open binary input ephemeris file.\n\n");
                 break;

        case  6: printf("\n convert: Mismatch in KSIZE & NCOEFF definitions.\n\n");
                 break;

        case  7: printf("\n convert: Improper header: should be Group 1010.\n\n");
                 break;

        case  8: printf("\n convert: Improper header: should be Group 1030.\n\n");
                 break;

        case  9: printf("\n convert: Improper header: should be Group 1040.\n\n");
                 break;

        case 10: printf("\n convert: Name-Variable count mismatch.\n\n");
                 break;
                 
        case 11: printf("\n convert: Improper header: should be Group 1041.\n\n");
                 break;

        case 12: printf("\n convert: Improper header: should be Group 1050.\n\n");
                 break;

        case 13: printf("\n convert: Change in record size detected.\n\n");
                 break;

        case 14: printf("\n extract: Improper value for T_start (too early).\n\n");
                 break;

        case 15: printf("\n extract: Improper value for T_stop (too late).\n\n");
                 break;

        case 16: printf("\n append: Files do not belong to same ephemeris.\n\n");
                 break;

        case 17: printf("\n append: Files do not match value of EPHEMERIS.\n\n");
                 break;

        case 18: printf("\n append: Files do not belong to same ephemeris.\n\n");
                 break;

        case 19: printf("\n append: Gap or overlap between files.\n\n");
                 break;

        case 20: printf("\n Find_Value: Could not find it.\n\n");
                 break;

        case 21: printf("\n READ_GROUP_HEADER: Not enough line breaks.\n\n");
                 break;

        case 22: printf("\n Unexpected EOF encountered. \n\n");
                 break;

        case 23: printf("\n extract: T_start > T_stop is not allowed.\n\n");
                 break;

        default: printf("\n What's wrong? Beats me...\n\n");

      }
}

/**==========================================================================**/
/**  Find_Value                                                              **/
/**                                                                          **/
/**    Given a string, an array of strings, and an array of double precision **/
/**    numbers (both arrays having the same dimension) this function will    **/
/**    scan the string array to find a match and, if a match is found, it    **/
/**    will return the value of the double precision number whose position   **/
/**    in the double precision array corresponds to the location of the      **/
/**    match in the string array (follow all that?).                         **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**    name        - a string                                                **/
/**    name_array  - an array of strings                                     **/
/**    value_array - a double precision array                                **/
/**                                                                          **/
/**  Output:                                                                 **/
/**    A double precision number                                             **/
/**==========================================================================**/

double Find_Value( char    name[], 
                   char    name_array[CONST_NUM][CONST_NAME_LENGTH] , 
                   double  value_array[CONST_NUM]  ) 
{
  double value = 0.0;
  int    found = FALSE;
  char   target[7];
  int    i, j;

  for ( i=0 ; i<CONST_NUM ; i++ ) 
      {
        /*.....................Convert current name array element to a string */
      
        for ( j=0 ; j<CONST_NAME_LENGTH ; j++ )  target[j] = name_array[i][j];
            target[6] = '\0';
      
        /*................................See if it matches the string sought */
      
        if ( !strcmp(name,target) ) 
           {
             value = value_array[i];
             found = TRUE;
             break;                                /* No need to keep looking */
           }                                       /*   once a match is found */
      }

  if ( (i==399) & (found==FALSE) ) Warning(20);             /* Didn't find it */

  return value;
}

/**==========================================================================**/
/**  Gregorian_to_Julian                                                     **/
/**                                                                          **/
/**     This function takes a time,  specified in terms of a Gregorian date  **/
/**     (i.e. a modern calander date) plus a time of day (given in terms of  **/
/**     hours, minutes, and seconds since midnight) and  returns the corre-  **/
/**     sponding Julian date.  The algorithm used to do this calculation is  **/
/**     based on pp 60-61 of: Astronomical Algorithms, Jean Meeus, Willman-  **/
/**     Bell, Richmond VA, 1991.  It has been modified  here to allow users  **/
/**     to input hours, minutes, and seconds instead of fractional days.     **/
/**                                                                          **/
/**     IMPORTANT NOTE: This algorithm, despite the claim made in Meeus, is  **/
/**     only valid for years later than 0 A.D. (or ACE for the politically   **/
/**     correct).                                                            **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**      year (integer: positive for AD, negative for BC)                    **/
/**     month (integer: 1 through 12)                                        **/
/**       day (integer: 1 through 31)                                        **/
/**      hour (integer: 0 through 24)                                        **/
/**       min (integer: 0 through 60)                                        **/
/**       sec ( double: less than 60.0)                                      **/
/**                                                                          **/
/**  Output:                                                                 **/
/**     Julian date (double)                                                 **/
/**==========================================================================**/

double Gregorian_to_Julian( int     year ,  int     month   , 
                            int     day  ,  int     hour    , 
                            int     min  ,  double  seconds )
{
  double  A, B, D, H, JD, M, N, Y;

  /*--------------------------------------------------------------------------*/
  /*  Adjust year & month values (if necessary).                              */
  /*--------------------------------------------------------------------------*/

  if ( month < 3 ) {                                      /* See Muess, p. 61 */
       month = month + 12;
       year  = year - 1;
     }

  /*--------------------------------------------------------------------------*/
  /*  Perform type casts and adjust the day count.                            */
  /*--------------------------------------------------------------------------*/

  Y = (double) year;
  M = (double) month;
  D = (double) day;
  H = (double) hour;
  N = (double) min;

  /*--------------------------------------------------------------------------*/
  /*  Compute the day fraction:                                               */
  /*--------------------------------------------------------------------------*/

  D = D  +  (H / 24.0)  +  (N / 1440.0)  +  (seconds / 86400.0);

  /*--------------------------------------------------------------------------*/
  /*  Compute the Julian date.                                                */
  /*--------------------------------------------------------------------------*/

  A  = floor(Y/100.0);
  B  = 2.0 - A + floor(A/4.0);  
  JD = floor(365.25*(Y+4716.0)) + floor(30.6001*(M+1.0)) + D + B - 1524.5;
  
  return JD;
}

/**==========================================================================**/
/**  Source File: gdcon.c                                                    **/
/**                                                                          **/
/**      Purpose: This program converts a Julian day number to a Gregorian   **/
/**               date. It is designed to be called from a Tcl/Tk script,    **/ 
/**               so it has a very unfriendly user interface.                **/
/**                                                                          **/
/**==========================================================================**/
void gdcon(double JED,char* Gregorian)
{
    char  *Month;
  double  A, B, C, D, E, F, T, Z, Alpha, Frac, Second, Total, Whole;
     int  Year, Day, Hour, Minute, Mo;

  /* Add a half day to input Julian date......................................*/
  
  T = JED + 0.5;
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
       case  1:  Month = "Jan";   break;
       case  2:  Month = "Feb";   break;
       case  3:  Month = "Mar";   break;
       case  4:  Month = "Apr";   break;
       case  5:  Month = "May";   break;
       case  6:  Month = "Jun";   break;
       case  7:  Month = "Jul";   break;
       case  8:  Month = "Aug";   break;
       case  9:  Month = "Sep";   break;
       case 10:  Month = "Oct";   break;
       case 11:  Month = "Nov";   break;
       case 12:  Month = "Dec";   break;
       default:  Month = "XXX";   break;
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

   sprintf(Gregorian,"%7.1f  %2d %s %4d %2d:%2d:%2.3f",JED,Day,Month,Year,Hour,Minute,Second);
}


/**==========================================================================**/
/**  Integer modulo function (self-explanatory: returns x mod y)             **/
/**==========================================================================**/

int mod(int x, int y)
{
  double rf, rx, ry;
     int f;
  
  /*--------------------------------------------------------------------------*/
  /* The basic formula can be found on p.38 of Knuth's The Art of Computer    */
  /* Programming, volume 1. I still can't believe that C lacks this function, */
  /* but I can find no mention of it in any of my C books...                  */
  /*--------------------------------------------------------------------------*/

  if ( y != 0 ) 
     {
       rx = (double) x;
       ry = (double) y;
       rf = floor(rx/ry);
       f  = (int) rf;
       return x - y*f;
     }
  else 
       return x;
}

/**==========================================================================**/
/**  Read_File_Line                                                          **/
/**                                                                          **/
/**     Reads in one line from an ASCII file. Specifically, it reads in 80   **/
/**     characters unless it encounters an end-of-line or an end-of-file.    **/
/**     If requested, it also converts FORTRAN scientific notation to the    **/
/**     equivalent C representation (in which case it converts any 'D' in    **/
/**     the line to an 'E').                                                 **/
/**                                                                          **/
/**  Inputs:                                                                 **/
/**     inFile     - pointer to an ASCII file                                **/
/**     filter     - Boolean flag (OK, an integer)                           **/
/**     lineBuffer - an externally defined character array                   **/
/**                                                                          **/
/**  Returns:                                                                **/
/**     Nothing.                                                             **/
/**==========================================================================**/

int Read_File_Line( FILE *inFile, int filter, char lineBuffer[82])
{
  char ignore[40];
  char *code;
  int  i, status = 0;

  /*--------------------------------------------------------------------------*/
  /* Read one line (81 characters max) from header file.                      */
  /*--------------------------------------------------------------------------*/
  
  code = fgets(lineBuffer,82,inFile);

  /*--------------------------------------------------------------------------*/
  /*  Protect against lines over 80 characters long.                          */
  /*--------------------------------------------------------------------------*/

  if ( (strlen(lineBuffer) == 81) && (lineBuffer[80] != '\n') )
     {
       fgets(ignore,40,inFile);                 /* Read past next end of line */
       lineBuffer[81] = '\0';
     }

  /*--------------------------------------------------------------------------*/
  /* Convert FORTRAN exponential representation to C representation.          */
  /*--------------------------------------------------------------------------*/
  
  if ( filter == TRUE ) 
     {
       for ( i=0 ; i<82 ; i++ )
           {
             if (lineBuffer[i] == '\0') break;
             if (lineBuffer[i] ==  'D') lineBuffer[i] = 'E';
           }
     }

  /*--------------------------------------------------------------------------*/
  /* Quit when EOF encountered.                                               */
  /*--------------------------------------------------------------------------*/

  if ( code == NULL ) status = EOF;

  return status;

}

/**==========================================================================**/
/**  Read_Group_Header                                                       **/
/**                                                                          **/
/**    Each data group in the header file is preceded by a group header      **/
/**    which consists of a blank line, a line of text, and another blank     **/
/**    line. The line of text consists of the string: GROUP   10XX (where    **/
/**    XX could be 10, 30, 40, 41, or 50). This function reads the group     **/
/**    header and returns an integer that signifies which group was read.    **/
/**                                                                          **/
/**  Input:                                                                  **/
/**    inFile - File pointer                                                 **/
/**                                                                          **/
/**  Returns:                                                                **/
/**    Integer = 1 for GROUP 1010                                            **/
/**            = 2 for GROUP 1030                                            **/
/**            = 3 for GROUP 1040                                            **/
/**            = 4 for GROUP 1041                                            **/
/**            = 5 for GROUP 1050                                            **/
/**==========================================================================**/

int Read_Group_Header(FILE *inFile)
{
   int  count   = 0;                               /* Initial values required */
   int  tripCnt = 1;
  char  appStr[2], charIn, headStr[10];
   int  group;

  /*--------------------------------------------------------------------------*/
  /* Build the group header string.                                           */
  /*--------------------------------------------------------------------------*/

  headStr[0] = '\0';                               /* Start with empty string */

  do { 
       /*................................................. Read one character */
       
       charIn = fgetc(inFile);
       
       /*..........................................Do the right thing with it */
       
       if ( isgraph(charIn) )          /* Build sting of non-blank characters */
          {
            appStr[0] = charIn;
            appStr[1] = '\0';
            strcat(headStr,appStr);
          }
       else                                              /* Count line breaks */
          {
            if (charIn == '\n') count = count + 1;
          }
            
       /*.......................................Protect against strange input */
       
       if (feof(inFile)) break;

       if ( ++tripCnt > 247 ) 
          {
            Warning(21);
            break;
          }
       
     } while (count < 3);

  /*--------------------------------------------------------------------------*/
  /* Determine the group whose data follows.                                  */
  /*--------------------------------------------------------------------------*/

       if (!strcmp(headStr,"GROUP1010")) group = 1;
  else if (!strcmp(headStr,"GROUP1030")) group = 2;
  else if (!strcmp(headStr,"GROUP1040")) group = 3;
  else if (!strcmp(headStr,"GROUP1041")) group = 4;
  else if (!strcmp(headStr,"GROUP1050")) group = 5;
  else                                   group = 0;
  
  return group;
}

/********************************************************* End: ephem_util.c **/

