#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

	FILE *Template_List;
	FILE *Align_File;
	FILE *Model_File;
	FILE *Chosen_RMSDs;
	FILE *Template_List_DNA;

	char filename[200];
	char Line[20];
	char Line_DNA[20];
	char RMSD_line[20];
	double RMSD;

	char template[20];
	char template_DNA[20];
	
	char FileLine[200];
	
	char sysCommand[250];	


	Template_List = fopen("Chosen_Templates.txt","r");
	Chosen_RMSDs = fopen("Chosen_RMSDs.txt","r");

	sprintf(sysCommand,"mkdir %s_Results/",argv[1]);
	system(sysCommand);

	sprintf(sysCommand,"mkdir %s_Results/data/",argv[1]);
        system(sysCommand);

	while(!feof(Template_List))
	{
		Line[0]='\0'; Line[1]='\0';
		template[0]='\0'; template[1]='\0';		

		fgets(Line,20,Template_List);
		sscanf(Line,"%s",template);
		
		fgets(RMSD_line,20,Chosen_RMSDs);
		sscanf(RMSD_line,"%lf",&RMSD);

		if(strlen(template) > 1)
		{
			sprintf(filename,"align_%s_%s.py",argv[1],template);

			Align_File = fopen(filename,"w");

			fprintf(Align_File,"from modeller import *\n\nenv = environ()\naln = alignment(env)\n");
			fprintf(Align_File,"mdl = model(env, file='All_Holo_pdb/%c%c%c%c.pdb', ",template[0],template[1],template[2],template[3]);
			fprintf(Align_File," model_segment=('FIRST:%c','LAST:%c'))\n",template[4],template[4]);
			fprintf(Align_File,"aln.append_model(mdl, align_codes='%s', atom_files='All_Holo_pdb/%c%c%c%c.pdb')\n",template,template[0],template[1],template[2],template[3]);
			fprintf(Align_File,"aln.append(file='%s.ali', align_codes='%c%c%c%c%c')\n",argv[1],argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4]);
			fprintf(Align_File,"aln.align2d()\n");
			fprintf(Align_File,"aln.write(file='%s-%s.ali', alignment_format='PIR')\n",argv[1],template);
			fprintf(Align_File,"aln.write(file='%s-%s.pap', alignment_format='PAP')",argv[1],template);

			fclose(Align_File);

			sprintf(sysCommand,"modpy.sh python align_%s_%s.py",argv[1],template);
			system(sysCommand);

			sprintf(filename,"model_%s_%s.py",argv[1],template);
			
			Model_File = fopen(filename,"w");

			fprintf(Model_File,"from modeller import *\nfrom modeller.automodel import *\n#from modeller import soap_protein_od\n\nenv = environ()\n");
			fprintf(Model_File,"a = automodel(env, alnfile='%s-%s.ali',",argv[1],template);
			fprintf(Model_File,"knowns='%s', sequence='%c%c%c%c%c',",template,argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4]);
			fprintf(Model_File,"assess_methods=(assess.DOPE,\n\t#soap_protein_od.Scorer(),\n\tassess.GA341))\n");
			fprintf(Model_File,"a.starting_model = 1\na.ending_model = 10\na.make()");

			fclose(Model_File);

			sprintf(sysCommand,"modpy.sh python model_%s_%s.py",argv[1],template);
                        system(sysCommand);
			
			int R;
			for(R=1;R<=10;R++)
			{

				if(R < 10)	
					sprintf(sysCommand,"/users/afarrel/procheck/procheck.scr %c%c%c%c%c.B9999000%d.pdb %lf",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],R,RMSD);

				if(R >= 10)
                                        sprintf(sysCommand,"/users/afarrel/procheck/procheck.scr %c%c%c%c%c.B999900%d.pdb %lf",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],R,RMSD);

                                system(sysCommand);

				//sprintf(sysCommand,"mv %c%c%c%c%c.B9999000%d.pdb %s_Results/%s_%s_%d.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],R,argv[1],argv[1],template,R);	
				//system(sysCommand);
			}	

			sprintf(sysCommand,"./Get_5Best_Models.o %c%c%c%c%c",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4]);
			system(sysCommand);


			sprintf(sysCommand,"mv %c%c%c%c%c_Best1.pdb %s_Results/%s_%s1.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1],argv[1],template);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c_Best2.pdb %s_Results/%s_%s2.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1],argv[1],template);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c_Best3.pdb %s_Results/%s_%s3.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1],argv[1],template);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c_Best4.pdb %s_Results/%s_%s4.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1],argv[1],template);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c_Best5.pdb %s_Results/%s_%s5.pdb",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1],argv[1],template);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c.B999900* %s_Results/data/",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1]);
			system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c.D00000* %s_Results/data/",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1]);
                        system(sysCommand);

			sprintf(sysCommand,"mv %c%c%c%c%c.V99990* %s_Results/data/",argv[1][0],argv[1][1],argv[1][2],argv[1][3],argv[1][4],argv[1]);
                        system(sysCommand);

			Template_List_DNA  = fopen("Chosen_Templates.txt","r");
			
			while(!feof(Template_List_DNA))	
			{

				Line_DNA[0]='\0'; Line_DNA[1]='\0';
		                template_DNA[0]='\0'; template_DNA[1]='\0';
	
        		        fgets(Line_DNA,20,Template_List_DNA);
        		        sscanf(Line_DNA,"%s",template_DNA);
				
								

				sprintf(sysCommand,"./Create_Single_chain_DNA_complex.o All_Holo_pdb/%c%c%c%c.pdb %c",template_DNA[0],template_DNA[1],template_DNA[2],template_DNA[3],template_DNA[4]);
				system(sysCommand);
				

				sprintf(sysCommand,"TMAlign/TMalign %s_Results/%s_%s1.pdb temp_Chain.pdb -o TM.sup",argv[1],argv[1],template);
                                system(sysCommand);

                                sprintf(sysCommand,"./PDB_complete_holo_model.o %s_%s1_D%s",argv[1],template,template_DNA);
                                system(sysCommand);

                                sprintf(sysCommand,"TMAlign/TMalign %s_Results/%s_%s2.pdb temp_Chain.pdb -o TM.sup",argv[1],argv[1],template);
                                system(sysCommand);

                                sprintf(sysCommand,"./PDB_complete_holo_model.o %s_%s2_D%s",argv[1],template,template_DNA);
                                system(sysCommand);

                                sprintf(sysCommand,"TMAlign/TMalign %s_Results/%s_%s3.pdb temp_Chain.pdb -o TM.sup",argv[1],argv[1],template);
                                system(sysCommand);

                                sprintf(sysCommand,"./PDB_complete_holo_model.o %s_%s3_D%s",argv[1],template,template_DNA);
                                system(sysCommand);

                                sprintf(sysCommand,"TMAlign/TMalign %s_Results/%s_%s4.pdb temp_Chain.pdb -o TM.sup",argv[1],argv[1],template);
                                system(sysCommand);

                                sprintf(sysCommand,"./PDB_complete_holo_model.o %s_%s4_D%s",argv[1],template,template_DNA);
                                system(sysCommand);

                                sprintf(sysCommand,"TMAlign/TMalign %s_Results/%s_%s5.pdb temp_Chain.pdb -o TM.sup",argv[1],argv[1],template);
                                system(sysCommand);

                                sprintf(sysCommand,"./PDB_complete_holo_model.o %s_%s5_D%s",argv[1],template,template_DNA);
                                system(sysCommand);
                        	

	                        //sprintf(sysCommand,"/users/afarrel/procheck/procheck.scr %s_%s_D%s.holo.pdb %lf",argv[1],template,template_DNA,RMSD);
	                        //system(sysCommand);

				//sprintf(sysCommand,"mv %s_%s_D%s.holo.sum %s_Results/",argv[1],template,template_DNA,argv[1]);
        	                //system(sysCommand);

				sprintf(sysCommand,"mv %s_%s1_D%s.holo.pdb %s_Results/",argv[1],template,template_DNA,argv[1]);
				system(sysCommand);

				sprintf(sysCommand,"mv %s_%s2_D%s.holo.pdb %s_Results/",argv[1],template,template_DNA,argv[1]);
                                system(sysCommand);

				sprintf(sysCommand,"mv %s_%s3_D%s.holo.pdb %s_Results/",argv[1],template,template_DNA,argv[1]);
                                system(sysCommand);

				sprintf(sysCommand,"mv %s_%s4_D%s.holo.pdb %s_Results/",argv[1],template,template_DNA,argv[1]);
                                system(sysCommand);

				sprintf(sysCommand,"mv %s_%s5_D%s.holo.pdb %s_Results/",argv[1],template,template_DNA,argv[1]);
                                system(sysCommand);
			}
			fclose(Template_List_DNA);

		}
		
	} 

	fclose(Template_List);
	fclose(Chosen_RMSDs);	

        sprintf(sysCommand,"mv *%s_* %s_Results/data/",argv[1],argv[1]);
        system(sysCommand);

	sprintf(sysCommand,"mv *%s.* %s_Results/data/",argv[1],argv[1]);
        system(sysCommand);	

	sprintf(sysCommand,"mv *%s-* %s_Results/data/",argv[1],argv[1]);
        system(sysCommand);

	sprintf(sysCommand,"rm TM.sup* *.ini *.rsr *.sch");
	system(sysCommand);

	return 0;
}
