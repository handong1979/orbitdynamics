/**==========================================================================**/
/**                                                                          **/
/**  SOURCE FILE: convert.c                                                  **/
/**                                                                          **/
/**      Purpose: This program converts  an ASCII ephemeris data file to a   **/
/**               binary data file.  Functionally, it is almost equivalent   **/
/**               to the JPL supplied program ASC2EPH.f; the difference is   **/
/**               that it converts all of the input ASCII file. If a trun-   **/
/**               cated data file is desired,  the converted (binary) file   **/
/**               can be trimmed by the program extract. If a segment that   **/
/**               spans two ASCII files is desired,  they can both be con-   **/
/**               verted into binary files which can then be joined by the   **/
/**               program append.  This combined file can then be trimmed    **/
/**               by the program extract, if necessary.                      **/
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
  int  First_Pass = TRUE;
  int  lastSize   = ARRAY_SIZE;
  int  recCnt     = 0;
  int  rejCnt     = 0;
  int  cycle      = 1;
  
  recOneType   R1;
  recTwoType   R2;

  char    lineBuffer[82] = {'\0'};
  double  D1, D2, D3, DblDenum, T_last,  T_now,  val[ARRAY_SIZE];
  double T_Begin, T_End;
  FILE   *inFile1, *inFile2,  *outFile;
  int     count, i,  j,  KS,  M,  
          group, maxLine, NC,    recNum, recSize, status;
  char Gregorian[CONST_LABEL_LENGTH];
  char BeginDate[CONST_LABEL_LENGTH] = "Start Epoch: JED= ";
  char FinalDate[CONST_LABEL_LENGTH] = "Final Epoch: JED= ";

    /*------------------------------------------------------------------------*/
    /* Open data files; quit if unable to open any of them.                   */
    /*------------------------------------------------------------------------*/

    if ( argc == 4 ) 
       { 
         inFile1 = fopen(argv[1],"r"); 
         inFile2 = fopen(argv[2],"r"); 
         outFile = fopen(argv[3],"wb");
       }
    else 
       { 
         Warning(1);
         exit(1);
       }

    if ( (inFile1==NULL) | (inFile2==NULL) | (outFile==NULL) ) 
       { 
         if (inFile1==NULL) Warning(2);
         if (inFile2==NULL) Warning(3);
         if (outFile==NULL) Warning(4);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* Initialize constant name & value arrays.                               */
    /*------------------------------------------------------------------------*/

    for ( i=0 ; i<CONST_NUM ; i++ ) 
        {
          for (j=0 ; j<CONST_NAME_LENGTH ; j++ ) R1.constName[i][j] = '\0';
          R2.constValue[i] = 0.0;
        }

	for (i=0 ; i<3 ; i++)
		for (j=0 ; j<CONST_LABEL_LENGTH ; j++)
			/*if ( R1.label[i][j] == '\r')*/ R1.label[i][j] = '\0';

    /*------------------------------------------------------------------------*/
    /* Read first line from header file (into dummy variables).               */
    /*------------------------------------------------------------------------*/
  
    status = Read_File_Line(inFile1,FALSE,lineBuffer);
    sscanf(lineBuffer,"KSIZE= %d NCOEFF= %d",&KS,&NC);

    /*------------------------------------------------------------------------*/
    /* Read data from Group 1010                                              */
    /*------------------------------------------------------------------------*/
    
    group = Read_Group_Header(inFile1);

    if ( group == 1 ) 
       {
         for ( i=0 ; i<3 ; i++ ) 
             { 
               status = Read_File_Line(inFile1,FALSE,lineBuffer);
               for (j=0 ; j<CONST_LABEL_LENGTH ; j++) R1.label[i][j] = lineBuffer[j];
             }
       }
    else 
       {
         Warning(7);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* Read data from Group 1030                                              */
    /*------------------------------------------------------------------------*/
    
    group = Read_Group_Header(inFile1);
    
    if ( group == 2 ) 
       {
         status = Read_File_Line(inFile1,FALSE,lineBuffer);
         sscanf(lineBuffer," %10le",&R1.timeData[0]);
         sscanf(lineBuffer+14," %10le",&R1.timeData[1]);
         sscanf(lineBuffer+27," %10le",&R1.timeData[2]);
       }
    else 
       {
         Warning(8);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* Read data from Group 1040                                              */
    /*------------------------------------------------------------------------*/
    
    group = Read_Group_Header(inFile1);
    
    if ( group == 3 ) 
       {
         status = Read_File_Line(inFile1,FALSE,lineBuffer);
         sscanf(lineBuffer," %d",&M);
         count = 0;
         while ( count < (M-1) ) 
               {
                 status = Read_File_Line(inFile1,FALSE,lineBuffer);

                 for ( i=0 ; i<10 ; i++ )
                     {
                       sscanf(lineBuffer+(i*8),"  %s",&R1.constName[count]);
                       if (++count == M) break;
                     }
               }
       }
    else 
       {
         Warning(9);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* Read data from Group 1041                                              */
    /*------------------------------------------------------------------------*/
    
    group = Read_Group_Header(inFile1);

    if ( group == 4 ) 
       {
         status = Read_File_Line(inFile1,FALSE,lineBuffer);
         sscanf(lineBuffer," %d",&NC);                /* NC now means "Number */
         if ( M == NC )                               /* of Constants"; it is */
            {                                         /* no longer a "dummy". */
              count = 0;
              while ( count < (NC-1) ) 
                    {
                      status = Read_File_Line(inFile1,TRUE,lineBuffer);
                      sscanf(lineBuffer," %le %le %le",&D1,&D2,&D3);
                      R2.constValue[count] = D1;
                      if (++count == NC) break;
                      R2.constValue[count] = D2;
                      if (++count == NC) break;
                      R2.constValue[count] = D3;
                      if (++count == NC) break;
                    }
            }
         else 
            {
              Warning(10);
              exit(1);
            }
       }
    else 
       {
         Warning(11);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* Read data from Group 1050                                              */
    /*------------------------------------------------------------------------*/

    group = Read_Group_Header(inFile1); 

    if ( group == 5 ) 
       {
         for ( i=0 ; i<3 ; i++ ) 
             {
               status = Read_File_Line(inFile1,FALSE,lineBuffer); 
               for ( j=0 ; j<13 ; j++ ) 
                   {
                     if (j<12)
                        sscanf(lineBuffer+(j*6),"%3ld",&R1.coeffPtr[j][i]);
                     else
                        sscanf(lineBuffer+(j*6),"%3ld",&R1.libratPtr[i]);
                   }
             }
       }
    else 
       {
         Warning(12);
         exit(1);
       }

    /*------------------------------------------------------------------------*/
    /* The binary header record contains some constants whose values are      */
    /* assigned in the (ASCII) header file. These values are extracted and    */
    /* assigned to local variables here:                                      */
    /*------------------------------------------------------------------------*/

    R1.numConst = (long int) NC;
    R1.AU       = Find_Value("AU",R1.constName,R2.constValue);
    R1.EMRAT    = Find_Value("EMRAT",R1.constName,R2.constValue);
    DblDenum    = Find_Value("DENUM",R1.constName,R2.constValue);
    R1.DENUM    = (long int) DblDenum;

    /*------------------------------------------------------------------------*/
    /* Write binary header records.                                           */
    /*------------------------------------------------------------------------*/

    fwrite(&R1,sizeof(recOneType),1,outFile);
    fwrite(&R2,sizeof(recTwoType),1,outFile);
//	goto close;
    /*------------------------------------------------------------------------*/
    /* Main Loop: Read ASCII data and write binary data.                      */
    /*------------------------------------------------------------------------*/

    while (!feof(inFile2))
          {
          /*.........................................Increment record counter */
          
          recCnt  = recCnt + 1;

          /*..........................Read first line of current ASCII record */

          status = Read_File_Line(inFile2,FALSE,lineBuffer);
          
          if (status == EOF)                          /* Nominal program exit */
             break;
          else
             sscanf(lineBuffer," %d %d ",&recNum,&recSize);
         
          /*................................Check for record size consistancy */
         
          if ( recSize != lastSize ) 
             {
               Warning(13);
               exit(1);
             }
          lastSize = recSize;
         
          /*................Compute the number of lines with coefficient data */
         
          if (mod(recSize,3)==0) maxLine = recSize/3;
          else                   maxLine = recSize/3 + 1;
         
          /*..................................Read coefficients into an array */

          for ( i=0 ; i<maxLine ; i++ )  
              {
                status = Read_File_Line(inFile2,TRUE,lineBuffer);

                if ( status == EOF )
                   {
                     Warning(22);
                     break;
                   }

                sscanf(lineBuffer,"%le",&val[3*i]); 
                if (3*i+1 >= recSize) break; 
                sscanf(lineBuffer+26,"%le",&val[3*i+1]); 
                if (3*i+2 >= recSize) break;
                sscanf(lineBuffer+52,"%le",&val[3*i+2]);
              }

          /*........................................Read start time of record */

          T_now = val[0];

          /*........................Write the coefficients to the binary file */
         
          if ( First_Pass )               /* Will force write of first record */
             {
			   T_Begin = T_now;
               T_last     = val[0];
               First_Pass = FALSE;
             }

          if ( T_now == T_last )      /* Write only if no gap between records */
             {
               cycle  = cycle + 1;
	       
               if ( cycle == 26 )            /* Let the user know all is well */
                  {
                    printf("\n   Writing record: %d",recCnt);
                    cycle = 1;
                  }

               for ( i=0 ; i<recSize ; i++ )      /* Write binary data record */
                   {
                     fwrite(&val[i],sizeof(double),1,outFile);
                   }

               T_last = val[1];                   /* Save stop time of record */
              
             }
          else
             {
               if (rejCnt == 0) printf("\n");
               rejCnt = rejCnt + 1;
               printf("   Record %d rejected:",recCnt);
               printf("  T_last = % 4d , ",T_last);
               printf("T_now = % 4d\n",val[0]);
               if (rejCnt > 10) return 1;              /* Major problem, quit */
             }
         
          } /*.................................................End: Main Loop */
	/*重新写文件头，将数据的起止时间更新*/
	T_End = T_last;
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
    /*------------------------------------------------------------------------*/
    /*  Close files, print summary, and quit.                                 */
    /*------------------------------------------------------------------------*/

    fclose(inFile1);
    fclose(inFile2);
    fclose(outFile);
    
    printf("\n\n   Data Conversion Completed. \n");
    printf("\n      Records Converted: % 3d",recCnt);
    printf("\n       Records Rejected: % 2d\n\n",rejCnt);
   
    return 0;
   
} /**======================================================== End: convert.c **/
