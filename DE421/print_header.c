/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: print_header.c                                             **/
/**                                                                          **/
/**      Purpose: This program reads both header records from a binary eph-  **/
/**               emeris file and prints their contents in a format similar  **/ 
/**               that of the ASCII header file  found on the JPL FTP site.  **/
/**               It was  created  primarily to verify  the ASCII-to-binary  **/
/**               conversion performed by convert, but it may be useful as   **/
/**               a stand alone utility program.                             **/
/**                                                                          **/
/**   Programmer: David Hoffman/EG5                                          **/
/**               NASA, Johnson Space Center                                 **/
/**               Houston, TX 77058                                          **/
/**               e-mail: david.a.hoffman1@jsc.nasa.gov                      **/
/**                                                                          **/
/**==========================================================================**/


#include <stdio.h>
#include "ephem_util.h"
#ifndef TYPES_DEFINED
#include "ephem_types.h"
#endif

main (argc, argv)
  char *argv[];
   int argc;
{
  int  cycle = 1;

  recOneType   R1;
  recTwoType   R2;
  int          i ,   j;
  FILE         *inFile;
    
  /*--------------------------------------------------------------------------*/
  /* Open file or files (quit if unsuccessful).                               */
  /*--------------------------------------------------------------------------*/

  if  ( argc == 2 ) 
      {
        inFile  = fopen(argv[1],"rb");
        if ( inFile==NULL ) 
           {
             if (inFile==NULL) Warning(5);
             return 1;
           }
      }
  else
     {
       Warning(1);
       return 1;
     }

  /*--------------------------------------------------------------------------*/
  /* Read header records and extract data.                                    */
  /*--------------------------------------------------------------------------*/
  
  fread(&R1,sizeof(recOneType),1,inFile);
  fread(&R2,sizeof(recTwoType),1,inFile);

 
  /*--------------------------------------------------------------------------*/
  /* Print Group 1010 data.                                                   */
  /*--------------------------------------------------------------------------*/

  printf("\n GROUP   1010\n\n");
  
  for ( i=0 ; i<3 ; i++ ) 
      {
        for ( j=0 ; j<CONST_LABEL_LENGTH  ; j++ ) 
            {
              if ( R1.label[i][j] != '\0' ) 
                   putchar(R1.label[i][j]);
              else
                 {
                   if (i==2) printf("\n");  
                   break;                 
                 }
            }
      }

  /*--------------------------------------------------------------------------*/
  /* Print Group 1030 data.                                                   */
  /*--------------------------------------------------------------------------*/

  printf("\n GROUP   1030\n\n");
  printf("   %8.2lf",R1.timeData[0]);
  printf("  %8.2lf ",R1.timeData[1]);
  printf(" %3.2lf\n",R1.timeData[2]);

  /*--------------------------------------------------------------------------*/
  /* Print Group 1040 data.                                                   */
  /*--------------------------------------------------------------------------*/

  printf("\n GROUP   1040\n\n");
  printf("    %ld \n",R1.numConst);

  for ( i=0 ; i<R1.numConst ; i++ )               /* Outer loop drives name selection */
      {
        for ( j=0 ; j<CONST_NAME_LENGTH ; j++ )              /* Inner loop drives name output */
            {
              putchar(R1.constName[i][j]);
              if (j==7) printf(" ");             /* Two spaces between names */
            }
        if  ( cycle == 10 )                      /* New line every tenth name */
            {
              printf("\n");
              cycle = 0;
            }
        cycle = cycle + 1;
      }
  
  if (cycle > 1) printf("\n");                         /* Flush non-full line */

  /*--------------------------------------------------------------------------*/
  /* Print Group 1041 data.                                                   */
  /*--------------------------------------------------------------------------*/

  cycle = 1;                                                 /* Must be reset */
  
  printf("\n GROUP   1041\n\n");
  printf("    %ld \n",R1.numConst);


  for ( i=0 ; i<R1.numConst ; i++ )            /* Loop drives value selection */
      {
        printf(" % 1.18le",R2.constValue[i]);
        
        if  ( cycle == 3 )                      /* New line every third value */
            {
              printf("\n");
              cycle = 0;
            }
        cycle = cycle + 1;
      }

  if (cycle > 1) printf("\n");                         /* Flush non-full line */

  /*--------------------------------------------------------------------------*/
  /* Print Group 1050 data.                                                   */
  /*--------------------------------------------------------------------------*/

  printf("\n GROUP   1050\n\n");

  for ( i=0 ; i<3 ; i++ )
      {
        for ( j=0 ; j<13 ; j++ )
            {
              if (j<12)
                  printf("   %3ld",R1.coeffPtr[j][i]);
              else
                  printf("   %3ld",R1.libratPtr[i]);
            }
        printf("\n");
      }


  printf("\n OTHERS:\n");
  printf("\n   DENUM = %4ld",R1.DENUM);
  printf("\n   AU    = % 1.18le",R1.AU);
  printf("\n   EMRAT = % 1.18le\n\n",R1.EMRAT);


  return 0;
  
} /**=================================================== End: print_header.c **/
