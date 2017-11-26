# Homology_Modeling_Project
Generate Homology Models of Transcription Factor- DNA structures using a Transcription factor FASTA sequence

This project takes a FASTA file as input. 
A pairwise sequence alignment is performed between the FASTA file and every Transcription factor(TF)-DNA complex PDB file in the database
The structures with the 5 highest percent Identities are used as a template for homology modeling using MODELLER.
5 Models are built for each template. Each TF Model is paired with each template's DNA structure (5 DNA structures).
This results in 125 TF-DNA complex PDBs

PDB_Master_program_125_5best.c:
Main Pipeline. Runs custom scripts.

PDB_convert_fasta.c:
Converts FASTA file to file format that is usable by MODELLER

PDB_build_profile.c:
Generates python scripts that customize the Homology Modeling process for the specific TF sequence.

PDB_get_template.c:
Uses Modeller (by running python scripts) to check the database for possible templates and selecting the 5 with the highest percent identity

PDB_make_model_125_5best.c:
Generates Models from templates using python scripts/MODELLER. 
Performs structural alignments between models and templates to add DNA structures to generate homology models using TMAlign.
Perform Structure quality checks. 
Chooses top models based on Structure/Ramachandran plot quality scores from TMAlign's qualtiy check.




