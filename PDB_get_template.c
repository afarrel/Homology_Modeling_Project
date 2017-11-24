#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double conver_E_to_double(char[]);

int main(int argc, char *argv[])
{

	char Line[300];
	FILE *build_profile;

	char Templates[10][20];
	double E_values[10][4];
	char template[20];
	char temp[10];
	char temp_EV[20];
	double max_value=0;
	double pct_ID;
	double coverage;
	char sysCommand[100];


	double number;
	int x,y;

	for(x=0;x<10;x++) 
	{
		E_values[x][0] = 1000;
		E_values[x][1] = 0;
		E_values[x][2] = 0;
	}

        build_profile = fopen("build_profile.prf","r");
	
	for(x=0;x<7;x++) fgets(Line,300,build_profile);

	while(!feof(build_profile))
	{

		fgets(Line,300,build_profile);
		
		sscanf(Line,"%s %s %s %s %s %s %s %s %s %lf %lf %s",temp,template,temp,temp,temp,temp,temp,temp,temp,&coverage,&pct_ID,temp_EV);
		
		number =conver_E_to_double(temp_EV);
		max_value = -1;
		for(y=0;y<10;y++)
		{
			if(max_value < E_values[y][0]) max_value = E_values[y][0];
		}



		if(number <= max_value)
		for(y=0;y<10;y++)
		{
			if(max_value == E_values[y][0])
			{
				if(number < E_values[y][0])
				{
					E_values[y][0] = number;
					E_values[y][1] = pct_ID;
					E_values[y][2] = coverage;
					sprintf(Templates[y],"%s",template);
					break;
				}
				else
				{	
					if(number == E_values[y][0])
					{
						if(pct_ID > E_values[y][1])
						{
							E_values[y][0] = number;
                                			E_values[y][1] = pct_ID;
                                			E_values[y][2] = coverage;
							sprintf(Templates[y],"%s",template);
							break;
						}	
						else
						{				
							if((pct_ID==E_values[y][1]) && (coverage>E_values[y][2]))
							{
								E_values[y][0] = number;
                                			        E_values[y][1] = pct_ID;
                                       			 	E_values[y][2] = coverage;
								sprintf(Templates[y],"%s",template);
								break;
							}
						}
					}
				}

			}
		}
	

	}

	for(x=0;x<10;x++) printf("%s\t%lf\n",Templates[x],E_values[x][0]);



	
	FILE *new_Compare_file;
	char filename[100];
	sprintf(filename,"Compare_%s.py",argv[1]);

	new_Compare_file = fopen(filename,"w");

	fprintf(new_Compare_file,"from modeller import *\n\nenv = environ()\naln = alignment(env)\n");

	fprintf(new_Compare_file,"for (pdb, chain) in (");
	
	for(x=0;x<10;x++)
	{
		if(strlen(Templates[x])>=5)
		fprintf(new_Compare_file,"('%c%c%c%c','%c')",Templates[x][0],Templates[x][1],Templates[x][2],Templates[x][3],Templates[x][4]);
		else
		fprintf(new_Compare_file,"('%c%c%c%c','')",Templates[x][0],Templates[x][1],Templates[x][2],Templates[x][3]);		

		if(x<9) fprintf(new_Compare_file,",");
	}
	fprintf(new_Compare_file,"):\n");
	fprintf(new_Compare_file,"\tm = model(env, file='All_Holo_pdb/'+pdb+'.pdb', model_segment=('FIRST:'+chain, 'LAST:'+chain))\n");
	fprintf(new_Compare_file,"\taln.append_model(m, atom_files=pdb, align_codes=pdb+chain);\n");
	fprintf(new_Compare_file,"aln.malign()\naln.malign3d()\naln.compare_structures()\n");
	fprintf(new_Compare_file,"aln.id_table(matrix_file='family.mat')\nenv.dendrogram(matrix_file='family.mat', cluster_cut=-1.0)");
	
	fclose(new_Compare_file);

	
	sprintf(sysCommand,"modpy.sh python Compare_%s.py > Compare_%s.log",argv[1],argv[1]);
	system(sysCommand);


	FILE *Compare_log;
	FILE *Chosen_Templates;
	FILE *Chosen_RMSDs;

	char Compare_Lines[200];
	char Comp_dashes[100];
	char Comp_pdbID[10];
	char Comp_rmsd[10]; 
	char fixed_rmsd[10];

	double top_RMSDs[5];
	double rmsd;

	sprintf(filename,"Compare_%s.log",argv[1]);
	Compare_log = fopen(filename,"r");
	
	while(!feof(Compare_log))
	{
		fgets(Compare_Lines,200,Compare_log);
		
		if((Compare_Lines[0]=='W')&&(Compare_Lines[1]=='e')&&(Compare_Lines[2]=='i')&&(Compare_Lines[3]=='g'))
		{
			while(!feof(Compare_log))
			{
				sprintf(Compare_Lines,"blank");
				fgets(Compare_Lines,200,Compare_log);

				if(strstr(Compare_Lines,"@")!=NULL)
				{
					printf("yes");

					if(strstr(Compare_Lines,"|")==NULL)
					sscanf(Compare_Lines," %s %s %s",Comp_dashes,Comp_pdbID,Comp_rmsd);
					if(strstr(Compare_Lines,"|")!=NULL)
                                        sscanf(Compare_Lines,"%s %s %s %s",temp,Comp_dashes,Comp_pdbID,Comp_rmsd);


					printf("\n %s %s %s\n",Comp_dashes,Comp_pdbID,Comp_rmsd);
					for(x=0;x<10;x++)
					{
						if(strcmp(Comp_pdbID,Templates[x])==0)
						{
							sscanf(Comp_rmsd,"@%s",fixed_rmsd);
							sscanf(fixed_rmsd,"%lf",&rmsd);
							E_values[x][3]=rmsd;
							printf("%s\n",fixed_rmsd);
							break;
						}	
					}
				}

			}
		}
	}
	fclose(Compare_log);


for(x=0;x<10;x++) printf("%s\t%lf\t%lf\t%lf\n",Templates[x],E_values[x][1],E_values[x][3],(E_values[x][1]/E_values[x][3]));

	double min_score=1000;
	double top_scores[5];
	char top_templates[5][10];
	double scores[10];

	for(x=0;x<10;x++) scores[x] = E_values[x][1]/E_values[x][3];
	for(x=0;x<5;x++) top_scores[x] = 0;


	for(x=0;x<10;x++)
	{
		min_score = 1000;
		for(y=0;y<5;y++)
		{
			if(top_scores[y] < min_score) min_score = top_scores[y];			
		}

		if(scores[x]>min_score)
		{
			for(y=0;y<5;y++)
			{
				if(E_values[x][3]!=1)
				if(top_scores[y]==min_score)
				{
					top_scores[y] = scores[x];
					sprintf(top_templates[y],"%s",Templates[x]);
					top_RMSDs[y] = E_values[x][3];
					break;
				}
			}
		}
	}

	Chosen_Templates = fopen("Chosen_Templates.txt","w");
	Chosen_RMSDs = fopen("Chosen_RMSDs.txt","w");
	for(x=0;x<5;x++)
	{ 
		fprintf(Chosen_Templates,"%s",top_templates[x]);
		fprintf(Chosen_RMSDs,"%lf",top_RMSDs[x]);
		if(x<4) fprintf(Chosen_Templates,"\n");
		if(x<4) fprintf(Chosen_RMSDs,"\n");
	}

	fclose(Chosen_Templates);
 return 0;
}


double conver_E_to_double(char number[])
{

	char char_base[10];

	double base;	
	char sign;
	
	char char_exp[10];
	int exp=0;
	double value;

	int i=0;	
	int j=0;
	int k;

	while((number[i]!='e')&&(number[i]!='E')&&(i<strlen(number)))
	{
		char_base[j]=number[i];
		i++;
		j++;
	}
	char_base[j] = '\0';
	
	i++;
	if(number[i]=='-')
	{
		sign = '-';
		i++;
	}
	else
		sign = '+';

	j=0;
	for (k=i;k<strlen(number);k++)
	{
		char_exp[j]=number[k];
		j++;
	}
	char_exp[j] = '\0';

	sscanf(char_base,"%lf",&base);
	sscanf(char_exp,"%d",&exp);


	if(sign=='-') value = base/pow(10,exp);
	if(sign=='+') value = base*pow(10,exp);

	return value;
}
