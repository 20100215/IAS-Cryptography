/**
 *  CRYPTOGRAPHY SIMULATION: [1] Abash/Atbash
 *  CS 3106 (Information Assurance and Security)
 *  SUBMITTED BY: Wayne Dayata
 *  SUBMITTED TO: Mr. Godwin Monserate
 *  11/13/2022
 * 
 *******************************************************************************************
 *  NOTE TO USER: RUN THE .EXE FILE DIRECTLY AND DO NOT CHANGE THE CONETNTS OF THIS FILE.  *
 *******************************************************************************************
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/********** A1. GLOBAL VARIABLES *******/
char fileName[100];
char outputName[100];
FILE* fileIn; 			//input file  (to obtain ciplertext/plaintext)
FILE* fileOut;  		//output file (to store ciphertext/plaintext)
char* buffer;			//area for character manipulation
int length;				//character length of file
int mode; 				//1 = encrypt, 2 = decrypt
//int key;				//encryption/decryption key (not applicable)
char ch;				

/********** A2. FUNCTION PROTOTYPES *******/
void encrypt();
void decrypt();

void getFileName();
void getMode();
//void getKey();

void openFile();
void readFromInputFile();

void getOutputFileName();
void writeToOutputFile();

void displayResultMessage();

void resetValues();

/********** B. MAIN FUNCTION *******/
int main(){
do{
		//INITIALIZATION
        system("cls"); // clear screen
        resetValues();

        //HEADER
        printf("\n------------------------------------------------------------");
        printf("\n-------------- CRYPTOGRAPHY SIMULATION (Abash) -------------");
        printf("\n---------------- DEVELOPED BY: WAYNE DAYATA  ---------------");
        printf("\n------------------------- 11/13/2022 -----------------------");
        printf("\n");

		//INSTRUCTIONS
        printf("\nInstructions for use:");
        printf("\n- 1. Enter the file name");
        printf("\n- 2. Select the mode (encrypt/decrypt)");
        //printf("\n  3. Input your desired key (for encryption) or the key used (for decryption)");
        printf("\n- 3. Check the output file generated.");
        printf("\n");

        //USER INPUTS
        getFileName();
        getMode();
        //getKey();

        // PROCESS AND RESULT
        readFromInputFile();
        switch(mode){
        	case 1: encrypt(); break;
        	case 2: decrypt(); break;
		}
		getOutputFileName();
		writeToOutputFile();
		displayResultMessage();

        // ASK FOR MAIN MENU OPTION
        printf("\nPress any key to process another file");
        printf("\nPress the 0 key to exit program");
        ch = getch();

    } while (ch != '0');
    free(buffer);
}

/********** C. FUNCTIONS FOR ENCRYPTION AND DECRYPTION *******/
void encrypt(){
	int x, oldch, newch;	
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = 25 - oldch;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = 25 - oldch;
				buffer[x] = newch + 'a';
			}	
		}else if (isdigit(buffer[x])){
			oldch = buffer[x] - '0';
			newch = 9 - oldch;
			buffer[x] = newch + '0';
		}
	}
}

void decrypt(){
	int x, oldch, newch;	
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = 25 - oldch;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = 25 - oldch;
				buffer[x] = newch + 'a';
			}	
		}else if (isdigit(buffer[x])){
			oldch = buffer[x] - '0';
			newch = 9 - oldch;
			buffer[x] = newch + '0';
		}
	}	
}


/********** D. UTILITY FUNCTIONS *******/
void getFileName(){
	printf("\n=== STEP 1: ===");
	printf("\nEnter file name (including the extension):");
	printf("\nExample: file1.txt");
	printf("\n\nFILE NAME: ");
	do{
		//obtain file name
		scanf("%s",fileName);
		//check if file exists
		openFile();
		if(fileIn == NULL){
			printf("\nSpecified file does not exist. Please input file name again.");
			printf("\nFILE NAME: ");
		}
	}while(fileIn == NULL);
}

void getMode(){
	printf("\n=== STEP 2: ===");
	printf("\nFile obtained. Now enter the mode of operation:");
	printf("\n[ 1 ] - Encrypt file");
	printf("\n[ 2 ] - Decrypt file");
	printf("\n\nMODE: ");
	do{
		//obtain mode
		scanf("%d",&mode);
		//check if mode input is correct
		if(!(mode == 1 || mode == 2)){
			printf("\nIncorrect input. Please try again.");
			printf("\nMODE: ");
		}
	}while(!(mode == 1 || mode == 2));
}

//void getKey(){
//	printf("\n=== STEP 3: ===");
//	if(mode==1){
//		printf("\nEnter an encryption key (Input a positive number only).");
//	}else{
//		printf("\nEnter the key for decryption (Input a positive number only).");
//	}
//  printf("\n\nINPUT KEY: ");
//	do{
//		//obtain key
//		scanf("%d",&key);
//		//check if key input is correct
//		if(key<=0){
//			printf("\nIncorrect input. Please try again.");
//			printf("\nINPUT KEY: ");
//		}
//	}while(key<=0);
//}


void openFile(){
	//open the file
	fileIn = fopen(fileName,"rb");
}


void readFromInputFile(){
	//get file length
	fseek(fileIn, 0, SEEK_END);
  	length = ftell(fileIn);
  	//set file pointer to beginning of file
  	fseek(fileIn, 0, SEEK_SET);
  	buffer = (char*)malloc((length+1)*sizeof(char));
  	if(buffer != NULL){
  		//transfer contents
    	fread(buffer, 1, length, fileIn);
	}
	buffer[length] = '\0';
  	fclose(fileIn);	
}

void getOutputFileName(){
	
	strcpy(outputName,fileName);
	char *dot = strrchr(outputName, '.');
    char fileExt[10];
    strcpy(fileExt,dot);
	if(mode==1){
		strcpy(dot,"_encrypted");
	}else{
		strcpy(dot,"_decrypted");
	}
	strcat(outputName,fileExt);
}

void writeToOutputFile(){
	//create new file
	fileOut = fopen(outputName,"wb");
	//write contents to file
	length = strlen(buffer);
	if(fileOut != NULL){
		//transfer contents
		fwrite(buffer,1,length,fileOut);
	}
	fclose(fileOut);
}

void displayResultMessage(){
	char modeStr[10];
	if(mode==1){
		strcpy(modeStr,"encrypted");
	}else{
		strcpy(modeStr,"decrypted");
	}
	
	printf("\n=== STEP 3: ===");
	printf("\nYour file has been %s.",modeStr);
	printf("\nPlease check the output file named \"%s\"\n",outputName);
	
}

void resetValues(){
	strcpy(fileName,"");
	strcpy(outputName,"");
	fileIn = NULL;
	fileOut = NULL;
	free(buffer);
	length = 0;
	mode = 0;
	//key = 0;
}
