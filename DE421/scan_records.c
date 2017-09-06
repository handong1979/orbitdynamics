/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: scan_records.c                                             **/
/**                                                                          **/
/**      Purpose: This program reads a binary ephemeris file and prints out  **/
/**               the start and stop times of every Tchebeyshev coefficient  **/
/**               record it contains.                                        **/
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
  double       binRec[ARRAY_SIZE];
  FILE        *inFile;
  recOneType  R1;
  recTwoType  R2;
  int          count = 0;

  /*--------------------------------------------------------------------------*/
  /* Open input file (quit if unsuccessful).                                  */
  /*--------------------------------------------------------------------------*/

    if ( argc == 2 ) 
         inFile = fopen(argv[1],"rb");
    else 
       { 
         Warning(1);
         exit(1);
       }
 
   if ( inFile==NULL ) 
      {
        Warning(5);
        exit(1);
      }

  /*--------------------------------------------------------------------------*/
  /* Print table header.                                                      */
  /*--------------------------------------------------------------------------*/

  printf("\n\n   Record       Start        Stop    ");
  printf("\n   -----------------------------------\n");
 
  /*--------------------------------------------------------------------------*/
  /* Read header records from input file.                                     */
  /*--------------------------------------------------------------------------*/
  
  fread(&R1,sizeof(recOneType),1,inFile);
  fread(&R2,sizeof(recTwoType),1,inFile);

  /*--------------------------------------------------------------------------*/
  /* Read data records then print the start & stop times.                     */
  /*--------------------------------------------------------------------------*/

  while (!feof(inFile)) 
      {
        /*..................................................Increment counter */
        
        count = count + 1;
      
        /*.................................................Read binary record */
      
        fread(&binRec,sizeof(double),ARRAY_SIZE,inFile);

       /*..................................Print start and end times for data */

       printf("   %4d       %8.2lf   %8.2lf\n",count,binRec[0],binRec[1]);

      }      

  /*--------------------------------------------------------------------------*/
  /* Close files & quit.                                                      */
  /*--------------------------------------------------------------------------*/

  printf("\n");
  fclose(inFile);
  
  return 0;

}
