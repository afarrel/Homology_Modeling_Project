#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

	char filename[200];
	char filename_output[220];
	char Line[200];

	sprintf(filename,"%s",argv[1]);
	sprintf(filename_output,"Build_profile_%s.py",argv[1]);

	FILE *BProfile_input;
	FILE *BProfile_output;
	
	BProfile_input = fopen("build_profile_Holo_All.py","r");
	BProfile_output = fopen(filename_output,"w");

	while(!feof(BProfile_input))
	{
		Line[0] = '\0';
		Line[4] = '\0';
		fgets(Line,200,BProfile_input);
		
		if((Line[4]=='a')&&(Line[5]=='p')&&(Line[6]=='p')&&(Line[7]=='e')&&(Line[8]=='n')&&(Line[9]=='d'))
		fprintf(BProfile_output,"aln.append(file='%s', alignment_format='PIR', align_codes='ALL')\n",filename);
		else
		fprintf(BProfile_output,"%s",Line);
			
				
	}
	fclose(BProfile_output);
	fclose(BProfile_input);


	return 0;
}
