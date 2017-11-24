#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

	FILE *input_file;
	FILE *output_file;
	
	char Line[300];
	char Line1[300];
	char Line2[300];
	char Name[200];

	char input_filename[100];
	char output_filename[105];

	int i,j;	
	int Last_line_Length=0;

	sprintf(input_filename,"%s",argv[1]);
	sprintf(output_filename,"%s.ali",argv[1]);

	input_file = fopen(input_filename,"r");
	output_file = fopen(output_filename,"w");
	
	Line1[0] = '\0';
	Line2[0] = '\0';
	while(!feof(input_file))
	{
		Line[0] = '\0';
		Line1[0] = '\0';
		fgets(Line,300,input_file);

		sprintf(Line1,"%s",Line2);
		sprintf(Line2,"%s",Line);

		if(Line1[0]=='>')
		{
			fprintf(output_file,">P1;");
			i=1;j=0;
			while((Line1[i]!=' ')&&(Line1[i]!='\t')&&(Line1[i]!='|')&&(Line1[i]!='\n')&&(Line1[i]!='\0'))
			{
				if(Line1[i]!=':')
				{
					Name[j]=Line1[i];
					j++;
				}
				i++;
			}
			Name[j] = '\0';
			
			fprintf(output_file,"%s\nsequence:%s:::::::0.00: 0.00\n",Name,Name);
		}
		else
		{
			if((strlen(Line1)>0)&&(strlen(Line2)<1))
			{
				Line1[strlen(Line1)-1]='*';
				Line1[strlen(Line1)+0]='\0';
				Line1[strlen(Line1)+1]='\0';
			}
			fprintf(output_file,"%s",Line1);
			
		}

	}
	if(strlen(Line2)>1)
        {
        	Line2[strlen(Line2)]='*';
                Line2[strlen(Line2)+1]='\0';
       		fprintf(output_file,"%s",Line2);
	}


	fclose(input_file);
	fclose(output_file);


	return 0;
}
