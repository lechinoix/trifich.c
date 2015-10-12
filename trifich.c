#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* inputPath = "./file.txt";
char* outputPath = "./sortedFile.txt";
char* outputData;
char* fileContents;
FILE* inputFile;
FILE* outputFile;
int showInputFile = 0;
int showOutputFile = 0;
int fileOverride = 0;


int nGetArg(int argc, char *argv[])
{
	 //Mode Interactif
     if(argc==1)
	 {
		 return nGetInteractiveArgs(argc,argv);
	 }
	 //Mode non Interactif
	 else
	 {
		 int inputFileIndex =0;
		 int outputFileIndex =0;
		 int i;
		 for(i=0;i<argc;i++)
		 {
			if(argv[i][0]=='-')
			{
				if(argv[i][1]=='i')
				{
					showInputFile=1;	
				}				
				else if(argv[i][1]=='e')
				{
					fileOverride=1;
				}
				else if(argv[i][1]=='o')
				{
					showOutputFile=1;
				}				
				else
				{
					printf("Wrong arguments ! \n");
					return 1;
				}
			}				
			else
			{
				if(inputFileIndex != 0)
				{
					outputFileIndex=i;
				}
				else
				{
					inputFileIndex=i;
				}				
			}
		 }
		 
		 
		 if(inputFileIndex == 0)
		 {
            return nGetInteractiveArgs(argc,argv);       
         }  
         
         
         
              
		 if(nFicExist(argv[inputFileIndex])==0)
		 {
			 inputPath=argv[inputFileIndex];
			  if((nFicExist(argv[outputFileIndex])==1))
			  {
				  outputPath = argv[outputFileIndex];
				  return 0;
			  }
			  else
			  {
				  if(fileOverride==0)
				  {
				 	 printf("Output file already exists override option not selected\n");
				 	 return 1;
				  }
				  else
				  {
				 	 outputPath = argv[outputFileIndex];
				 	 return 0;
				  }
			  }
		 }
		 else
		 {
             
			 printf("No input File at given path\n");
			 return 1;
		 }
	 }
}

int nGetInteractiveArgs(int argc, char *argv[])
{
    char* path ;
    char* path2;
    int bool = 0;
    do {
       path = malloc(sizeof(char)*63);
       bool=0;
       printf("Enter input file path : \n");
       
       gets(path);
       printf("You entered %s",path);
       if(path[0] == '\0')
       {
           return 1;
       }
       else
       {
           
           if(nFicExist(path)!=0)
           {
               bool = 1;
           }
       }
       
   
       } 
   while (bool);
   inputPath = path;
   int bool2 = 0;
   do {
       path2 = malloc(sizeof(char)*63);
       bool2 = 0;
       printf("Enter output file path : \n");
       gets(path2);
       printf("You entered %s",path2);
       if(path2[0] == '\0')
       {
           return 1;
       }
       else
       {
           if(nFicExist(path2)==0)
           {
               char answer[12] ={'\0'};
               printf("File already exists, Override ? (y/n) \n");
               gets(answer);
               if(answer[0]!='y')
               {
                  bool2 =  1;         
               }
           }
       }
       } 
    while (bool2);
    outputPath = path2;
       
}

int nGetFic(void){

	inputFile = fopen(inputPath, "rw");
	return(0);

}

int nFicExist(const char *fileName)
{
	if( access( fileName, F_OK ) == 0 ) {
    	return 0;
	} else {
    	return 1;
	}
}

int nTriFic (){
	
	char strLine[256];
	int flag = 0;
	int line_size;

	struct Line
	{
		char * text;
		struct Line * nextLine;
	};

	struct Line * currentLine;
	struct Line * first;
	struct Line * testedLine;
	struct Line * previousLine;

	while(fgets(strLine, sizeof(strLine), inputFile))
	{
		if(showInputFile)
		{
			if(flag == 0)
			{
				printf("\nInput file :\n\n");
			}
			printf("%s", strLine);
		}
		currentLine = malloc(sizeof(struct Line*));
		line_size = sizeof(strLine) + 100;
		currentLine->text = malloc(line_size*sizeof(char));
		strcpy(currentLine->text, strLine);

		if(flag == 0)
		{
			currentLine->nextLine = NULL;
			first = currentLine;
		}
		else
		{
			testedLine = first;
			int end = 0;

			while( !end )
			{

				int i = 0;
				int bool = 1;
				
				while(bool)
				{
					bool = 0;
					char testedChar = testedLine->text[i];			
					char currentChar = currentLine->text[i];

					if(testedChar < 97){
						testedChar += 32;
					}
					if(currentChar < 97){
						currentChar += 32;
					}

					if(testedChar==currentChar){
						bool = 1;
						i++;
					}
				}

				char testedChar = testedLine->text[i];			
				char currentChar = currentLine->text[i];

				if(testedChar < 97){
					testedChar += 32;
				}
				if(currentChar < 97){
					currentChar += 32;
				}
				if( testedChar < currentChar )
				{
					if(testedLine->nextLine == NULL)
					{
						currentLine->nextLine = NULL;
						testedLine->nextLine = currentLine;
						end = 1;
					}
					previousLine = testedLine;
					testedLine = testedLine->nextLine;

				}else
				{

					currentLine->nextLine = testedLine;
					if(testedLine != first)
					{
						previousLine->nextLine = currentLine;
					}else
					{
						first = currentLine;
					}
					end = 1;
				}
			}
		}
		flag = 1;
	}

	int inputFile_size = ftell(inputFile);
	outputData = malloc(inputFile_size*sizeof(char));

	currentLine = first;

	while(currentLine != NULL)
	{
		strcat(outputData, currentLine->text);
		previousLine = currentLine;
		currentLine = currentLine -> nextLine;
		free(previousLine->text);
		free(previousLine);
	}

	return 0;

}

int main(int argc, char *argv[] ){

	int test = nGetArg(argc, argv);
	if(test == 1){
		printf("Function terminated\n");
		return;
	}
	nGetFic();
	nTriFic();
	fclose(inputFile);

	if(showOutputFile)
	{
		printf("\nOutput file :\n\n%s", outputData);
	}
	if(fileOverride)
	{
		outputFile = fopen(inputPath, "w");
	}
	else
	{
		outputFile = fopen(outputPath, "w");
	}

	fputs(outputData, outputFile);
	fclose(outputFile);
	return 0;

}