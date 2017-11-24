#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

	char filename[200];
	char sysCommand[300];

	sprintf(filename,"%s",argv[1]);
	
	sprintf(sysCommand,"./PDB_convert_fasta.o %s",filename);
	system(sysCommand);

	sprintf(sysCommand,"./PDB_build_profile.o %s.ali",filename);
        system(sysCommand);

	sprintf(sysCommand,"modpy.sh python Build_profile_%s.ali.py",filename);
	system(sysCommand);

	//sprintf(sysCommand,"./PDB_get_template.o %s",filename);
	sprintf(sysCommand,"./PDB_get_MB_template.o %s",filename);
	system(sysCommand);
	
	sprintf(sysCommand,"./PDB_make_model_125_5best.o %s",filename);
        system(sysCommand);

	return 0;
}
