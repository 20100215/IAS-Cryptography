/**
 *  CRYPTOGRAPHY SIMULATION: [6] AES/RSA Cryptography
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
#include<math.h>

/********** A1. GLOBAL VARIABLES *******/
char fileName[100];
char outputName[100];
FILE* fileIn; 			//input file  (to obtain ciplertext/plaintext)
FILE* fileOut;  		//output file (to store ciphertext/plaintext)
char* buffer;			//area for character manipulation
int length;				//character length of file
int mode; 				//1 = encrypt, 2 = decrypt
long long int p1, p2, N, tot, e, d;
char ch;				

/********** A2. FUNCTION PROTOTYPES *******/
void encrypt();
void decrypt();

void getFileName();
void getMode();
void getKey();

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
        printf("\n---------------------------------------------------------------------");
        printf("\n--------- CRYPTOGRAPHY SIMULATION (AES - RSA Cryptography) ----------");
        printf("\n-------------------- DEVELOPED BY: WAYNE DAYATA  --------------------");
        printf("\n---------------------------- 11/13/2022 -----------------------------");
        printf("\n");

		//INSTRUCTIONS
        printf("\nInstructions for use:");
        printf("\n- 1. Enter the file name");
        printf("\n- 2. Select the mode (encrypt/decrypt)");
        printf("\n- 3. Input your desired key (for encryption) or the key used (for decryption)");
        printf("\n- 4. Check the output file generated.");
        printf("\n");

        //USER INPUTS
        getFileName();
        getMode();
    	getKey();

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
	long long int x, m, c;

	p1=2;
	p2=13;
	//product of primes
	N = p1 * p2;
	//totient
	tot = (p1-1)&(p2-1);
	//encryption key
	e = 5;
	d = 5;
	printf("\nDecryption key (save this for decryption) = (%d %d)\n\n",d,N);

	printf("Original: ");
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			m = tolower(buffer[x]);
			m = m - 'a' + 1;
			printf("%3d", m);
		}
	}
	printf("\nCipher:   ");

	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			m = tolower(buffer[x]);
			m = m - 'a' + 1;
			//encryption = c = m^e (mod N)
			c = (int)(pow(m,e)) % N;
			c = (c>=26) ? c%26 : c;
			printf("%3d", c);
			buffer[x] = (c!=0) ? c + 'a' - 1 : 'z';
		}
	}
}

void decrypt(){
	long long int x, m, c;

	printf("Cipher:   ");
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			c = tolower(buffer[x]);
			c = c - 'a' + 1;
			printf("%3d", c);
		}
	}
	printf("\nOriginal: ");

	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			c = tolower(buffer[x]);
			c = c - 'a' + 1;
			//decryption = m = c^d (mod N)
			m = (int)(pow(c,d)) % N;
			m = (m>=26) ? m%26 : m;
			printf("%3d", m);
			buffer[x] = (m!=0) ? m + 'a' - 1 : 'z';
		}
	}
}


/********** D. UTILITY FUNCTIONS *******/
void getFileName(){
	printf("\n=== STEP 1: ===");
	printf("\nEnter file name (including the extension).");
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
	printf("\nFile obtained. Now enter the mode of operation.");
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

void getKey(){
	printf("\n=== STEP 3: ===");
	if(mode==1){
		printf("\nEnter two prime numbers p1 p2: (E.g.: 2 13)");
		printf("\n\nINPUT KEYS: ");
		scanf("%d %d", &p1, &p2);
	}else{
		printf("\nEnter the decryption key set d N: (E.g. 5 26)");
		printf("\n\nINPUT KEYS: ");
		scanf("%d %d", &d, &N);
	}
}


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
	
	printf("\n=== STEP 4: ===");
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
}
