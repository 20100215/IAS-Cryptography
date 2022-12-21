/**
 *  CRYPTOGRAPHY SIMULATION: [4] Polyalphabetic Cipher
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
char* buffer2;
int length;				//character length of file
int newlength;
int mode; 				//1 = encrypt, 2 = decrypt
char key[100];			//encryption/decryption key 
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
        printf("\n----------- CRYPTOGRAPHY SIMULATION (Transposition Cipher) ----------");
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
	int x, y, ndx, keylen, base=0;	
	ndx=0;
	keylen=strlen(key);
	//storing positions when sorting the letters of the key
	char cols[keylen], temp;
	for(x=0;x<keylen;x++){
		cols[x]=x;
	}
	//determine new buffer length and where to plaace '\0'
	if(length % keylen != 0){
		newlength = length + (keylen - length % keylen);
	}else{
		newlength = length;
	}
	//allocate space for buffer2 base on new length
	buffer2 = (char*)calloc(newlength,sizeof(char));
	buffer = (char*)realloc(buffer,(newlength+1)*sizeof(char));	
	//fill gaps with spaces
	for(x=length;x<newlength;x++){
		buffer[x] = ' '; //this can be changeable to null character
	}
	//sorting
	for (x = 0; x < keylen - 1; x++) {
    	for (y = 0; y < keylen - x - 1; y++) {
			if (key[y] > key[y + 1]) {
				//swap letters
        		temp = key[y];
        		key[y] = key[y + 1];
        		key[y + 1] = temp;
				//swap positions
        		temp = cols[y];
        		cols[y] = cols[y + 1];
        		cols[y + 1] = temp;
      		}
    	}
	}
	//store to temp buffer, jumbled columns
	//process n letters at a time (n=keylength)
	for(x=0;x<newlength;x++){
		buffer2[x] = buffer[base+cols[ndx]];
		ndx++;
		if(ndx==keylen){
			ndx=0;
			base+=keylen;
		}
	}
	//store letters by accessing col1 -> row0-n, col2, ..., coln
	ndx=0;
	for(x=0;x<keylen;x++){
		for(y=x;y<newlength;y+=keylen){ 
			buffer[ndx]=buffer2[y];
			ndx++;
		}
	}
	//add terminating null character
	buffer[newlength]='\0';
}

void decrypt(){
	int x, y, ndx, base=0, keylen, collength, min;	
	ndx=0;
	keylen=strlen(key);
	//storing positions when sorting the letters of the key
	char cols[keylen+1], temp;
	for(x=0;x<keylen;x++){
		cols[x]=-1;
	}
	key[keylen+1]=127;
	collength = length / keylen;
	//allocate space for buffer2 base on new length
	buffer2 = (char*)calloc(length,sizeof(char));
	newlength = length;
	//sorting
	for (x = 0; x < keylen; x++) {
		min = keylen+1;
    	for (y = 0; y < keylen; y++) {
			if (key[y] < key[min] && cols[y]==-1) {
        		min = y;
      		}
    	}
    	cols[min] = x;
	}
	//store letters by accessing col1 -> row0-n, col2, ..., coln
	ndx=0;
	for(x=0;x<collength;x++){
		for(y=x;y<newlength;y+=collength){ 
			buffer2[ndx]=buffer[y];
			ndx++;
		}
	}
	//store to temp buffer, jumbled columns
	//process n letters at a time (n=keylength)
	ndx=0;
	for(x=0;x<newlength;x++){
		buffer[x] = buffer2[base+cols[ndx]];
		ndx++;
		if(ndx==keylen){
			ndx=0;
			base+=keylen;
		}
	}
	
	//add terminating null character
	buffer[newlength]='\0';
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
	int x;
	if(mode==1){
		printf("\nEnter an encryption key (Input only alphabetic characters, no spaces/numbers).");
	}else{
		printf("\nEnter the key for decryption (Input only alphabetic characters, no spaces/numbers).");
	}
	printf("\n\nINPUT KEY: ");
	do{
		//obtain key
		scanf("%s",key);
		//check if key input is correct - alphabet only
		for(x=0;key[x]!='\0'&& isalpha(key[x])!=0; x++){}
		if(key[x]!='\0' || x==0){
			printf("\nIncorrect input. Please try again.");
			printf("\nINPUT KEY: ");
		}
	}while(key[x]!='\0' || x==0);
	
	//convert to lowercase
	for(x=0;key[x]!='\0'; x++){
		if(isupper(key[x])) key[x]-=32;
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
  	buffer = (char*)calloc((length+1),sizeof(char));
  	if(buffer != NULL){
  		//transfer contents
    	fread(buffer, 1, length, fileIn);
	}
	//buffer[length] = '\0';
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
		fwrite(buffer,1,newlength,fileOut);
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
	free(buffer2);
	length = 0;
	mode = 0;
	strcpy(key,"");
}
