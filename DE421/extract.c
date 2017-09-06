/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: extract.c                                                  **/
/**                                                                          **/
/**      Purpose: This program  extracts asegment from a binary ephemeris    **/
/**               file. The file name is input from the command line; the    **/
/**               program prompts the user for the start and end times of    **/
/**               the desired data segment.                                  **/
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
  int writeRec = FALSE;

  recOneType  R1;
  recTwoType  R2;
  char         keyIn;
  long int     begData;
  FILE         *inFile,  *outFile;
  double       binRec[ARRAY_SIZE],  sec,  T_start,  T_stop;
  int          day,          hour,  min,  month,    year;
  int FirstTimeWrite = TRUE;
  double T_Begin, T_End;
  char Gregorian[CONST_LABEL_LENGTH];
  char BeginDate[CONST_LABEL_LENGTH] = "Start Epoch: JED= ";
  char FinalDate[CONST_LABEL_LENGTH] = "Final Epoch: JED= ";
  /*--------------------------------------------------------------------------*/
  /* Open input file (quit if unsuccessful).                                  */
  /*--------------------------------------------------------------------------*/


    if ( argc == 3 ) { 
         inFile = fopen(argv[1],"rb");
       }
    else { 
         Warning(1);
         exit(1);
       }
 
   if ( inFile==NULL ) {
       Warning(5);
       exit(1);
     }

  /*--------------------------------------------------------------------------*/
  /* Prompt user for start & stop times.                                      */
  /*--------------------------------------------------------------------------*/

  printf("\n");
  printf("   This program  extracts a segment from a binary ephemeris \n");
  printf("   file. It requires start and stop times for this segment. \n");
  printf("   These can  be input either as a pair of Julian  dates or \n");
  printf("   as a Gregorian  date (in the form day, month, year) plus \n");
  printf("   a Universal Time (in the form  hours, minutes, seconds). \n");
  printf("\n   Input a Julian date (y/n) ?  ");
  
  keyIn = getchar();
  
  if ( keyIn == 'y' || keyIn == 'Y' )     /* This is the easy one (for me...) */
     {
      printf("\n\n   Input Start Time:  "); 
      scanf("%f",&T_start);
      printf("\n\n   Input  Stop Time:  ");
      scanf("%f",&T_stop);
     }
  else                                /*...but this one takes a bit more work */
     {
       /*---------------------------------------------------------------------*/
       /* Input start time & convert to a Julian Date.                        */
       /*---------------------------------------------------------------------*/

       printf("\n\n   NOTE: Seconds is a real number (and thus requires ");
       printf("\n   a decimal point) while everthing else is an integer.");
       printf("\n\n   Input Start Time:    Year ?  ");
       scanf("%d",&year);
       printf("                       Month ?  ");
       scanf("%d",&month);
       printf("                         Day ?  ");
       scanf("%d",&day);
       printf("                        Hour ?  ");
       scanf("%d",&hour);
       printf("                      Minute ?  ");
       scanf("%d",&min);
       printf("                     Seconds ?  ");
       scanf("%lf",&sec);
       
       T_start = Gregorian_to_Julian(year, month, day, hour, min, sec );
       
       /*---------------------------------------------------------------------*/
       /* Input start time & convert to a Julian Date.                        */
       /*---------------------------------------------------------------------*/

       printf("\n\n   Input Stop Time:    Year ?  ");
       scanf("%d",&year);
       printf("                      Month ?  ");
       scanf("%d",&month);
       printf("                        Day ?  ");
       scanf("%d",&day);
       printf("                       Hour ?  ");
       scanf("%d",&hour);
       printf("                     Minute ?  ");
       scanf("%d",&min);
       printf("                    Seconds ?  ");
       scanf("%lf",&sec);

       T_stop = Gregorian_to_Julian(year, month, day, hour, min, sec );

     } /*................................................END: if keyIn == yes */

  /*--------------------------------------------------------------------------*/
  /* Read header records from input file.                                     */
  /*--------------------------------------------------------------------------*/
  
  fread(&R1,sizeof(recOneType),1,inFile);
  fread(&R2,sizeof(recTwoType),1,inFile);

  /*--------------------------------------------------------------------------*/
  /* Check validity of T_start & T_stop.                                      */
  /*--------------------------------------------------------------------------*/

  if ( T_start > T_stop ) {
       Warning(23);
       exit(1);
     }

  begData = ftell(inFile);                      /* Ephemeris data starts here */

  fread(&binRec,sizeof(double),ARRAY_SIZE,inFile);       /* Read first record */
  
  if ( T_start < binRec[0] ) {             /* Not valid; print warning & quit */
       Warning(14);
       exit(1);
     }

  fseek(inFile,-ARRAY_SIZE*(long)sizeof(double),SEEK_END);   /* Move to last record */
  fread(&binRec,sizeof(double),ARRAY_SIZE,inFile);                 /* Read it */
  
  if ( T_stop > binRec[1] ) {              /* Not valid; print warning & quit */
       Warning(15);
       exit(1);
     }

  fseek(inFile,begData,SEEK_SET);        /* Return to start of ephemeris data */

  /*--------------------------------------------------------------------------*/
  /* Open output file, then write header records to it.                       */
  /*--------------------------------------------------------------------------*/

  outFile = fopen(argv[2],"wb");

  if (outFile==NULL) {                                /* Quit if unsuccessful */
       Warning(4);
       exit(1);
     }

  fwrite(&R1,sizeof(recOneType),1,outFile);
  fwrite(&R2,sizeof(recTwoType),1,outFile);

  /*--------------------------------------------------------------------------*/
  /* Extract data segment (Main Loop).                                        */
  /*--------------------------------------------------------------------------*/

  while (!feof(inFile)) 
      {
        /*...................................................Read next record */
      
        fread(&binRec,sizeof(double),ARRAY_SIZE,inFile);
        
        /*.................If record interval brackets T_start, begin writing */
        
        if ((T_start >= binRec[0]) & (T_start <= binRec[1])) writeRec = TRUE;
        
        /*..........................................Write record if requested */

        if (writeRec)
		{
			if(FirstTimeWrite)
			{
				T_Begin = binRec[0];
				FirstTimeWrite = FALSE;
			}
			fwrite(&binRec,sizeof(double),ARRAY_SIZE,outFile);
			T_End = binRec[1];
		}

        /*...................If record interval brackets T_stop, stop writing */

        if ((T_stop >= binRec[0]) & (T_stop <= binRec[1])) writeRec = FALSE;

      }

  /*重新写文件头，将数据的起止时间更新*/
  R1.timeData[0] = T_Begin;
  R1.timeData[1] = T_End;
  gdcon(R1.timeData[0],Gregorian);
  strcat(BeginDate,Gregorian);
  strcpy(R1.label[1],BeginDate);
  gdcon(R1.timeData[1],Gregorian);
  strcat(FinalDate,Gregorian);
  strcpy(R1.label[2],FinalDate);
  fseek(outFile,0,SEEK_SET);
  fwrite(&R1,sizeof(recOneType),1,outFile);

  /*--------------------------------------------------------------------------*/
  /* Close files & quit.                                                      */
  /*--------------------------------------------------------------------------*/

  fclose(inFile);
  fclose(outFile);
  
  return 0;

} /**======================================================== END: extract.c **/
