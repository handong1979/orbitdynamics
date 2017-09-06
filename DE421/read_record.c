/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: read_record.c                                              **/
/**                                                                          **/
/**      Purpose: To aid in debugging the ephemeris tools.                   **/
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
  char *planet_name[13] = { "Mercury:  "  ,  "Venus:    "  ,  "E-M Bary: "  ,
                            "Mars:     "  ,  "Jupiter:  "  ,  "Saturn:   "  ,
                            "Uranus:   "  ,  "Neptune:  "  ,  "Pluto:    "  ,
                            "Moon:     "  ,  "Sun:      "  ,  "Nutation: "  ,
                            "Libration:"                                  };
  
  int  count = -1;
  int  cycle =  4;

  recOneType   R1;
  recTwoType   R2;
  
  double       binRec[ARRAY_SIZE];
  FILE        *inFile;
  int          comp, gran, i, j, theRec;
  long         dummy;

  /*--------------------------------------------------------------------------*/
  /* Open input file (quit if unsuccessful).                                  */
  /*--------------------------------------------------------------------------*/

    if ( argc == 3 ) 
       {
         dummy  = strtol(argv[1],NULL,10);                      /* C sucks... */
         theRec = (int) dummy;
         inFile = fopen(argv[2],"rb");
       }
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
  /* Read header records from input file.                                     */
  /*--------------------------------------------------------------------------*/
  
  fread(&R1,sizeof(recOneType),1,inFile);
  fread(&R2,sizeof(recTwoType),1,inFile);

  gran = R1.coeffPtr[0][1];

  /*--------------------------------------------------------------------------*/
  /* Search binary file for desired record.                                   */
  /*--------------------------------------------------------------------------*/

  while (!feof(inFile)) 
      {
        /*..................................................Increment counter */
        
        count = count + 1;
      
        /*.................................................Read binary record */
      
        fread(&binRec,sizeof(double),ARRAY_SIZE,inFile);
      
        /*...................................Choose desired record for output */
      
        if ( count == theRec )
           {
             for ( i=0 ; i<ARRAY_SIZE ; i++ )
                 {
                 
                   /*......................Print start and end times for data */
                   
                   if (i == 0)
                      printf("\n   Start Time = % 9.3f\n",binRec[i]);

                   if (i == 1)
                      printf("   End Time   = % 9.3f\n",binRec[i]);

                   /*...............................Print "granule seperator" */

                   if ( cycle == 0 ) 
                      {
                        printf("\n");
                        cycle = comp*gran - 1;
                      }
                   else
                        cycle = cycle - 1;

                   /*.......................Print planet name (when required) */

                   for ( j=0 ; j<13 ; j++ )
                       {
                          if ( j < 12 )
                             {
								 if ( i == R1.coeffPtr[j][0]-1 )
								 {
									  printf("\n   %10s\n",planet_name[j]);
									   if (j==11)
										  comp = 2;
									   else
										  comp = 3;
									   gran  = R1.coeffPtr[j][1];
									   cycle = comp*gran - 1;
								 }
                             }
                          else
                             {
								 if ( i == R1.libratPtr[0]-1 )
								 {
									   printf("\n   %10s\n",planet_name[j]);
									   gran  = R1.libratPtr[1];
									   cycle = 3*gran - 1;
								 }
                            }
                       }

                   /*........................................Print coefficent */

                   if ( i > 1 )
                      {
                        printf("            [%4d]",i+1);
                        printf("   % 1.18le\n",binRec[i]);
                      }

                 }

             /*................................................Exit from loop */

             break;

           }
      }

  /*--------------------------------------------------------------------------*/
  /* Close files & quit.                                                      */
  /*--------------------------------------------------------------------------*/

  printf("\n");
  fclose(inFile);
  
  return 0;

}
