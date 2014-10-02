/* CODEGEN1.C: Code Generator Program 1

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	May 22, 1993
	Description:	Updating Remarks and Documentation
	Modified by:	Kevin Rattai

	Program update requests:
		When defining script files for plates and buttons:
			- Update application to include funtions which will control
			  the displaying of plates and buttons during appropriate
			  movement within the application.
			  ie.  When a sub-menu is selected, jump to appropriate funtion
				   which displays the appropriate plates, etc.  As in the
				   farm application.

*/

#include<stdio.h>
#include<conio.h>
#include<dir.h>
#include<string.h>
#include"keyio.h"

void main(void)
{
	FILE *fin,*fout,*fp1;
	int x=0,z=0;
	char namein[12]="environ.src";
	char file1[12]="buttons.src",
		file2[12]="plates.src",
		file3[12]="forms.src";
	char queue[4];
	char tn[40];
	char nameout[12];
	char line[80];
	char desc[60];
	char test;
	char temp;
	char path[MAXPATH],
		newpath[MAXPATH];

	/* Remove old files if exist */
	remove(file1);
	remove(file2);
	remove(file3);

	clrscr();

	/* Define working paths/directories */
	strcpy(path,"\\");
	getcurdir(0,path+1);
	strcpy(newpath,path);
	strncat(newpath,"\\ccslsrce",strlen("\\ccslsrce"));

	/* Get program name and description */
	printf("Enter new program name:");
	scanf("%12s",nameout);
	printf("\n");
	printf("Enter program description (max 60 chars)\n");
	while(x<60 && temp!=ENTER)
	{
		temp=getch();
		switch(temp)
		{
			case BACKSPACE: desc[x]=NULL;
							if(x>0) x--;
							break;
			case ENTER:		desc[x]=NULL;
							break;
			default:		desc[x]=temp;
							x++;
		}
		putch(temp);
	}
	printf("\n\n");
	
	/* Update all paths and filenames to UPPERCASE */
	for(x=0;x<=12;x++) namein[x]=toupper(namein[x]);
	for(x=0;x<=12;x++) nameout[x]=toupper(nameout[x]);
	for(x=0;x<=MAXPATH;x++) path[x]=toupper(path[x]);
	for(x=0;x<=MAXPATH;x++) newpath[x]=toupper(newpath[x]);

	/* Display paths and files */
	printf("new file: %s\\%s\n",path,nameout);
	printf("old file: %s\\%s\n",newpath,namein);
	printf("Press the ANY key to continue");
	getch();
	clrscr();

	/* Define buttons */
	printf("Define Buttons in the form:\n");
	printf("<button name> <left pos> <top pos> <required button> ");
	printf("<button action>\n");
	printf("Press F2 when done.\n");
	temp = NULL;
	for(x=0;x<=79;x++) line[x]=NULL;
	x=0;
	fp1=fopen(file1,"a");
	while(temp != F2)
	{
		temp=get_key();
		if(temp!=F2)
		{
			switch(temp)
			{
				case BACKSPACE: line[x]=NULL;
								if(x>0) x--;
								putch(temp);
								break;
				case ENTER:		printf("\n");
								fprintf(fp1,"%s\n",line);
								for(x=0;x<=79;x++) line[x]=NULL;
								x=0;
								break;
				default:		line[x]=temp;
								putch(temp);
								x++;
			}
		}
	}
	if(x!=0) fprintf(fp1,"%s\n",line);
	fclose(fp1);
	clrscr();

	/* Define plates */
	printf("Define Plates in the form:\n");
	printf("<plate name> <left pos> <top pos> <required button> ");
	printf("<button action>\n");
	printf("Press F2 when done.\n");
	temp = NULL;
	for(x=0;x<=79;x++) line[x]=NULL;
	x=0;
	fp1=fopen(file2,"a");
	while(temp != F2)
	{
		temp=get_key();
		if(temp!=F2)
		{
			switch(temp)
			{
				case BACKSPACE: line[x]=NULL;
								if(x>0) x--;
								putch(temp);
								break;
				case ENTER:		printf("\n");
								fprintf(fp1,"%s\n",line);
								for(x=0;x<=79;x++) line[x]=NULL;
								x=0;
								break;
				default:		line[x]=temp;
								putch(temp);
								x++;
			}
		}
	}
	if(x!=0) fprintf(fp1,"%s\n",line);
	fclose(fp1);

	/* Define forms */
/*	clrscr();
	printf("Design Forms in the form:\n");
	printf("<button name> <left pos> <top pos> <required button> ");
	printf("<button action>\n");
	printf("Press F2 when done.\n");
	temp = NULL;
	for(x=0;x<=79;x++) line[x]=NULL;
	x=0;
	fp1=fopen(file1,"a");
	while(temp != F2)
	{
		temp=get_key();
		if(temp!=F2)
		{
			switch(temp)
			{
				case BACKSPACE: line[x]=NULL;
								if(x>0) x--;
								putch(temp);
								break;
				case ENTER:		printf("\n");
								fprintf(fp1,"%s\n",line);
								for(x=0;x<=79;x++) line[x]=NULL;
								x=0;
								break;
				default:		line[x]=temp;
								putch(temp);
								x++;
			}
		}
	}
	if(x!=0) fprintf(fp1,"%s\n",line);
	fclose(fp1); */
	remove(nameout);
	clrscr();

	/* Open files and start creating application */
	fout=fopen(nameout,"a");
	fprintf(fout,"/* %s: %s */\n\n",nameout,desc);
	fprintf(fout,"/* Be sure to create a project files which contains the");
	fprintf(fout," appropiate\n\tsource files, including this one */\n");
	printf("/* %s: %s */\n\n",nameout,desc);
	printf("/* Be sure to create a project file which contains the");
	printf(" appropriate\n    source files, including this one */\n\n");
	chdir(newpath);

	fin=fopen(namein,"r");
	test = fgetc(fin);
	while(test!=EOF)
	{
		switch(test)
		{
			case 10: printf("\n");
					break;
			case 9: printf("    ");
					break;
			default: putch(test);
		}
		chdir(path);
		fputc(test,fout);

		/* Search for occurance of:  p"); */
		for(z=0;z<=2;z++) queue[z]=queue[z+1];
		queue[3]=test;
		if(queue[0]=='p' && queue[1]=='"' &&
			queue[2]==')' && queue[3]==';')
		{
			/* Write new button definitions */
			fp1=fopen(file1,"r");
			test=fgetc(fp1);
			while(test!=EOF)
			{
				fprintf(fout,"\n\tdefbutton(");
				printf("\n    defbutton(");
				for(x=0;x<=39;x++) tn[x]=NULL;
				x=0;
				while(test!=EOF && test!=' ')
				{
					tn[x]=test;
					x++;
					test=fgetc(fp1);
				}
				if(test!=EOF) test=fgetc(fp1);
				while(test!=EOF && test!=' ')
				{
					fputc(test,fout);
					putch(test);
					test=fgetc(fp1);
				}
				fputc(',',fout);
				putch(',');
				if(test!=EOF) test=fgetc(fp1);
				while(test!=EOF && test!=' ')
				{
					fputc(test,fout);
					putch(test);
					test=fgetc(fp1);
				}
				fputc(',',fout);
				putch(',');
				fprintf(fout,"\"%s\");",tn);
				printf("\"%s\");",tn);
				while(test!=EOF && test!='\n') test=fgetc(fp1);
				if(test!=EOF) test=fgetc(fp1);
			}
			fputc('\n',fout);
			printf("\n");
			fclose(fp1);
			fp1=fopen(file2,"r");
			test=fgetc(fp1);
			if(test!=EOF)
			{
				fputc('\n',fout);
				printf("\n");
				fprintf(fout,"\t/* Plate */\t");
				printf("    /* Plate */    ");
			}

			/* Write new plate definitions */
			while(test!=EOF)
			{
				fprintf(fout,"\n\tdefplate(");
				printf("\n    defplate(");
				for(x=0;x<=39;x++) tn[x]=NULL;
				x=0;
				while(test!=EOF && test!=' ')
				{
					tn[x]=test;
					x++;
					test=fgetc(fp1);
				}
				if(test!=EOF) test=fgetc(fp1);
				while(test!=EOF && test!=' ')
				{
					fputc(test,fout);
					putch(test);
					test=fgetc(fp1);
				}
				fputc(',',fout);
				putch(',');
				if(test!=EOF) test=fgetc(fp1);
				while(test!=EOF && test!=' ')
				{
					fputc(test,fout);
					putch(test);
					test=fgetc(fp1);
				}
				fputc(',',fout);
				putch(',');
				fprintf(fout,"\"%s\");",tn);
				printf("\"%s\");",tn);
				while(test!=EOF && test!='\n') test=fgetc(fp1);
				if(test!=EOF) test=fgetc(fp1);
			}
			fputc('\n',fout);
			printf("\n");
			fclose(fp1);
		}

		/* Search for occurance of:  rti; */
		if(queue[0]=='r' && queue[1]=='t' &&
			queue[2]=='i' && queue[3]==';')
		{
			test=fgetc(fin);
			fputc(test,fout);
			putch(test);
			test=fgetc(fin);
			fputc(test,fout);
			putch(test);
			fp1=fopen(file1,"r");
			test=fgetc(fp1);
			z=2;

			/* Write new checks for button actions */
			while(test!=EOF)
			{
				fprintf(fout,"\t\tif((rat.buttons == ");
				printf("\r        if((rat.buttons == ");
				for(x=0;x<=39;x++) tn[x]=NULL;
				x=0;
				while(test!=EOF && x <= 2)
				{
					if(test==' ') x++;
					test=fgetc(fp1);
				}
				if(test!=EOF)
				{
					switch(test)
					{
						case 'L': fprintf(fout,"LBUTTON");
								printf("LBUTTON");
								break;
						case 'M':
						case 'C': fprintf(fout,"CBUTTON");
								printf("CBUTTON");
								break;
						case 'R': fprintf(fout,"RBUTTON");
								printf("RBUTTON");
								break;
					}
				}
				while(test!=EOF && test!=' ') test=fgetc(fp1);
				if(test!=EOF) test=fgetc(fp1);
				fprintf(fout," && key == ");
				printf(" && key == ");
				if(test!=EOF)
				{
					switch(test)
					{
						case 'P': fprintf(fout,"PRESS");
								printf("PRESS");
								break;
						case 'R': fprintf(fout,"RELEASE");
								printf("RELEASE");
								break;
						case 'H': fprintf(fout,"HELD");
								printf("HELD");
								break;
					}
				}
				fprintf(fout,") && (checkhit(%d,h,v)))\n\t\t{\n\t\t}",z);
				printf(") && (checkhit(%d,h,v)))\n\r        {\n\r        }",
					z);
				while(test!=EOF && test!='\n') test=fgetc(fp1);
				if(test!=EOF)
				{
					fputc(test,fout);
					putch(test);
				}
				if(test!=EOF) test=fgetc(fp1);
				z++;
			}
			fclose(fp1);
			fp1=fopen(file2,"r");
			test=fgetc(fp1);
			z=0;

			/* Write new checks for plate actions */
			while(test!=EOF)
			{
				fprintf(fout,"\t\tif((rat.buttons == ");
				printf("\r        if((rat.buttons == ");
				for(x=0;x<=39;x++) tn[x]=NULL;
				x=0;
				while(test!=EOF && x <= 2)
				{
					if(test==' ') x++;
					test=fgetc(fp1);
				}
				if(test!=EOF)
				{
					switch(test)
					{
						case 'L': fprintf(fout,"LBUTTON");
								printf("LBUTTON");
								break;
						case 'M':
						case 'C': fprintf(fout,"CBUTTON");
								printf("CBUTTON");
								break;
						case 'R': fprintf(fout,"RBUTTON");
								printf("RBUTTON");
								break;
					}
				}
				while(test!=EOF && test!=' ') test=fgetc(fp1);
				if(test!=EOF) test=fgetc(fp1);
				fprintf(fout," && key == ");
				printf(" && key == ");
				if(test!=EOF)
				{
					switch(test)
					{
						case 'P': fprintf(fout,"PRESS");
								printf("PRESS");
								break;
						case 'R': fprintf(fout,"RELEASE");
								printf("RELEASE");
								break;
						case 'H': fprintf(fout,"HELD");
								printf("HELD");
								break;
					}
				}
				fprintf(fout,") && (platehit(%d,h,v)))\n\t\t{\n\t\t}",z);
				printf(") && (platehit(%d,h,v)))\n\r        {\n\r        }",
					z);
				while(test!=EOF && test!='\n') test=fgetc(fp1);
				if(test!=EOF)
				{
					fputc(test,fout);
					putch(test);
				}
				if(test!=EOF) test=fgetc(fp1);
				z++;
			}
			printf("\r");
			fclose(fp1);
		}
		chdir(newpath);
		test = fgetc(fin);
	}
	fclose(fin);
	fclose(fout);
	chdir(path);

	/* Remove files if exist */
	remove(file1);
	remove(file2);
	remove(file3);
	printf("\n\nPress the ANY key to continue");
	getch();
}