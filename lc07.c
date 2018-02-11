#include <stdio.h>
#include <ctype.h>
#define LINESIZE 32768	
#define ERRSIZE  256
main(argc, argv)
int 	argc;
char 	*argv[];
{
unsigned char	D[3];
unsigned char	C[10];
unsigned char	BUF[LINESIZE];
short		ERR[ERRSIZE][2];
unsigned char	LINE[LINESIZE];
FILE 	*fpi, *fpo, *errpo;
int 	c;
int	ca,cb;
int 	i,j,k,l,ch_cnt;
int	cnt;
int	errors;
j = 0;
k = 0;
cnt = 1;
D[0] = '\x03';/*ASCII(3)*/
D[1] = '\x7C';/*|*/
D[2] = '\x02';/*ASCII(2)*/
C[1] = '\xFF';
C[2] = '\x81';/*Modified by lichao 20160104,GBK Character High part*/
C[3] = '\x40';/*Modified by lichao 20160104,GBK Character Low part*/
/*** input file ***/
if (strncmp(argv[1], "-", 1) ==0)
    fpi = stdin;
else if ( (fpi = fopen(argv[1], "r")) == NULL)
    {
    fprintf(stderr, "Can't open input file\n");
    return 0;
    }
/*** output data file ***/
if (argc ==3)
 {
    if (strncmp(argv[2], "-", 1) ==0)
			fpo = stdout;
    else if ( (fpo = fopen(argv[2], "w+")) == NULL)
		{
			fprintf(stderr, "Can't open output file\n");
			return 0;
		}
 }
/*** output error data file ***/
if (argc ==4)
 {
    if (strncmp(argv[2], "-", 1) ==0)
			fpo = stdout;
    else if (strncmp(argv[3], "-", 1) ==0)
			errpo = stdout;	
    else if ( (fpo = fopen(argv[2], "w+")) == NULL)
			{
			fprintf(stderr, "Can't open output file\n");
			return 0;
			}
    else if ( (errpo = fopen(argv[3], "w+")) == NULL)
			{
			fprintf(stderr, "Can't open output file\n");
			return 0;
			}
 }
/**************************************************/
for (i=0; i<LINESIZE; i++)
    BUF[i] = ' ';
while ( (c=getc(fpi)) != EOF)
 {
    BUF[k++] = c;
    if (c == '\n')
		{
			errors = 0;
			ch_cnt = 0;
		for (i=0; i<ERRSIZE; i++)
	    {
	    	ERR[i][0] = 0;
	    	ERR[i][1] = 0;
	    }
		for (i=0; i<k-1; i++)
	    {
	    	if (BUF[i] == D[0] || BUF[i] == D[1] || BUF[i] == D[2])
					{LINE[j++] = BUF[i];}
	    	/*** Deal with chinese characters coding with ,0X8140~0XFEFE -XX7F ***/
	    	else if (BUF[i] >= C[2] && BUF[i+1] >= C[3] && BUF[i+1] != C[1])/*\x81,129,modified by lichao,for looking for right chinese character coding with GBK and exporting the delimiters*/
					{LINE[j++] = BUF[i];LINE[j++] = BUF[i+1];i++;}
	    	else if (BUF[i] >= C[2] && BUF[i+1] == D[0])
					{ERR[errors][0] = i + 1;
		 				ERR[errors][1] = 5;
		 				errors++;
					}
	    	else LINE[j++] = BUF[i];
	    } /*for (i=0; i<k-1; i++)*/
        	if (errors && argc == 3)
	    		{
	    			fprintf(stderr, "%d error(s) in line %d.Error data will not be write down.\n", errors, cnt);
	    			errors = 0;
	    		}
		if (errors && argc == 4)
	    	{
	    	fprintf(stderr, "%d error(s) in line %d.Please check the error data file:%s.\n", errors, cnt,argv[3]);
	    	for (i=0; i<k; i++)
				putc(BUF[i], errpo);
	    	errors = 0;
	    	}
		if (argc == 3)
	    	{
	    	for (i=0; i<j; i++)
	    	putc(LINE[i], fpo);
				if (putc('\n', fpo) == EOF)
		  		{
		  		fprintf(stderr, "Can't write outputfile %s\n", argv[2]);
		  		return 0;
		  		}
	    	}
	if (argc == 4)
	    {
	    	for (i=0; i<j; i++)
	    	putc(LINE[i], fpo);
				if (putc('\n', fpo) == EOF)
		  		{
		  		fprintf(stderr, "Can't write outputfile %s\n", argv[2]);
		  		return 0;
		  		}
	    }
	for (i=0; i<k; i++) BUF[i] = ' ';
			j = 0;
			k = 0;
			cnt++;
		}	/*** if (c=='\n') ***/
 }		/*** while ***/
if (strncmp(argv[1], "-", 1) !=0)
    fclose(fpi);
if ( (argc == 3) && strncmp(argv[2], "-", 1) !=0)
    fclose(fpo);
if ( (argc == 4) && strncmp(argv[2], "-", 1) !=0  && strncmp(argv[3], "-", 1) !=0)
    fclose(errpo);
}
