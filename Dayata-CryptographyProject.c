/**
 *  CRYPTOGRAPHY PROJECT: 7 LEVEL CIPHER
 *  CS 3106 (Information Assurance and Security)
 *  SUBMITTED BY: Wayne Dayata
 *  SUBMITTED TO: Mr. Godwin Monserate
 *  12/05/2022
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
char* buffer2;
int length;				//character length of file
int newlength;
int mode; 				//1 = encrypt, 2 = decrypt
char key[100];			//encryption/decryption key 
char key2[100];         //another key generated based on the given key
long long int p1, p2, N, tot, e, d;	//RSA
char ch;				
int start, increment, order[7]; //order of encryption

/********** A2. FUNCTION PROTOTYPES *******/

//main modes
void getOrder();
void encrypt();
void decrypt();

//encrypt functions
void encrypt1(); //RSA
void encrypt2(); //atbash
void encrypt3(); //shifting of (strlen) keys
void encrypt4(); //vigenere (original key)
void encrypt5(); //vigenere (reversed key x2 value)
void encrypt6(); //transposition (original key)
void encrypt7(); //transposition (reversed key)

//decrypt functions
void decrypt1(); //RSA
void decrypt2(); //atbash
void decrypt3(); //shifting of (strlen) keys
void decrypt4(); //vigenere (original key)
void decrypt5(); //vigenere (reversed key x2 value)
void decrypt6(); //transposition (original key)
void decrypt7(); //transposition (reversed key)

void getFileName();
void getMode();
void getKey();
void getKey2();
void getRSA();

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
        printf("\n--------------- CRYPTOGRAPHY PROJECT - 7 LEVEL CIPHER ---------------");
        printf("\n-------------------- DEVELOPED BY: WAYNE DAYATA  --------------------");
        printf("\n---------------------------- 12/05/2022 -----------------------------");
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
		getKey2();
		getRSA();

        // PROCESS AND RESULT
        readFromInputFile();
        switch(mode){
        	case 1: 
				encrypt();
				break;
        	case 2: 
				decrypt(); 
				break;
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

void getOrder(){
	//determines the order of execution of the 7 levels of encryption or decryption
	//for the starting number - (sum of ASCII values in the string) mod 7
	//for the intervals - (digit sum of the sum of the ASCII values in the string) mod 6 + 1
	int x, sum;
	start = increment = sum = 0;
	for(x=0;key[x]!='\0';x++){
		start += key[x];
	}
	sum = start;
	start = start % 7;
	while(sum>0){
		increment += sum % 10;
		sum /= 10;
	}
	increment = increment % 6 + 1;

	//assigning the order
	printf("\nOrder: ");
	for(x=0;x<7;x++){
		order[x] = start;
		start = (start + increment) % 7;
		printf("%d - ",order[x]);
	}



}

void encrypt(){
	int x;
	//determine new buffer length and where to plaace '\0' - for transposition
	int length = strlen(buffer);
	int keylen = strlen(key);
	if(length % keylen != 0){
		newlength = length + (keylen - length % keylen);
	}else{
		newlength = length;
	}
	buffer = (char*)realloc(buffer,(newlength+1)*sizeof(char));	
	//fill gaps with spaces
	for(x=length;x<newlength;x++){
		buffer[x] = ' '; //this can be changeable to null character
	}
	buffer[newlength]='\0';
	length=newlength;

	getOrder();
	for(x=0;x<7;x++){ //iterate through the order array
		switch(x){
			case 0: encrypt1(); break;
			case 1: encrypt2(); break;
			case 2: encrypt3(); break;
			case 3: encrypt4(); break;
			case 4: encrypt5(); break;
			case 5: encrypt6(); break;
			case 6: encrypt7(); break;
		}
	}
	
}

void decrypt(){
	int x;

	getOrder();
	for(x=6;x>=0;x--){ //iterate through the order array in reverse
		switch(x){
			case 0: decrypt1(); break;
			case 1: decrypt2(); break;
			case 2: decrypt3(); break;
			case 3: decrypt4(); break;
			case 4: decrypt5(); break;
			case 5: decrypt6(); break;
			case 6: decrypt7(); break;
		}
	}
}

/********** C. FUNCTIONS FOR ENCRYPTION AND DECRYPTION *******/

void encrypt1(){ //RSA
	long long int x, m, c;

	p1=2;
	p2=13;
	//product of primes
	N = p1 * p2;
	//totient
	tot = (p1-1)*(p2-1);
	//encryption key
	e = 5;
	d = 5;
	printf("\nDecryption key (save this for decryption) = (%d %d)\n\n",d,N);
	int iscaps;

	for(x=0;x<newlength;x++){
		if(isalpha(buffer[x])){		
			iscaps = (isupper(buffer[x])) ? 1 : 0;
			m = buffer[x];
			m = (iscaps==1) ? (m - 'A' + 1) : (m - 'a' + 1);
			//encryption = c = m^e (mod N)
			c = (int)(pow(m,e)) % N;
			c = (c>=26) ? c%26 : c;
			if(iscaps==1)
				buffer[x] = (c!=0) ?  c + 'A' - 1 : 'Z';
			else 
				buffer[x] = (c!=0) ?  c + 'a' - 1 : 'z';
		}
	}
} 
void encrypt2(){ //atbash + string reverse
	int x, oldch, newch;	
	for(x=0;x<newlength;x++){
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
void encrypt3(){ //shifting of (strlen) keys
	int x, oldch, newch;	
	int offset = strlen(key);
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+offset%26)%26;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = (oldch+offset%26)%26;
				buffer[x] = newch + 'a';
			}	
		}else if (isdigit(buffer[x])){
			oldch = buffer[x] - '0';
			newch = (oldch+offset%10)%10;
			buffer[x] = newch + '0';
		}
	}
} 
void encrypt4(){ //vigenere (original key)
	int x, ndx, keylen, offset, oldch, newch;	
	ndx=0;
	keylen=strlen(key);
	for(x=0;x<length;x++){
		offset = key[ndx] - 'a';
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+offset)%26;
				buffer[x] = newch + 'A';			
			}else{
				oldch = buffer[x] - 'a';
				newch = (oldch+offset)%26;
				buffer[x] = newch + 'a';
			}	
			ndx = (ndx+1)%keylen;
		}
	}
} 
void encrypt5(){ //vigenere (reversed key x2 value)
	int x, ndx, keylen, offset, oldch, newch;	
	ndx=0;
	keylen=strlen(key2);
	for(x=0;x<length;x++){
		offset = key2[ndx] - 'a';
		offset *= 2;
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+offset)%26;
				buffer[x] = newch + 'A';			
			}else{
				oldch = buffer[x] - 'a';
				newch = (oldch+offset)%26;
				buffer[x] = newch + 'a';
			}	
			ndx = (ndx+1)%keylen;
		}
	}
} 

void encrypt6(){ //transposition (original key)
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
	length = newlength;
	free(buffer2);
}

void encrypt7(){ //transposition (reversed key)
int x, y, ndx, keylen, base=0;	
	ndx=0;
	keylen=strlen(key2);
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
			if (key2[y] > key2[y + 1]) {
				//swap letters
        		temp = key2[y];
        		key2[y] = key2[y + 1];
        		key2[y + 1] = temp;
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
	length = newlength;
	free(buffer2);
}


void decrypt1(){ //RSA
	long long int x, m, c;
	int iscaps;

	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			iscaps = (isupper(buffer[x])) ? 1 : 0;
			c = buffer[x];
			c = (iscaps==1) ? c - 'A' + 1 : c - 'a' + 1;
			//decryption = m = c^d (mod N)
			m = (int)(pow(c,d)) % N;
			m = (m>=26) ? m%26 : m;
			if(iscaps==1)
				buffer[x] = (m!=0) ?  m + 'A' - 1 : 'Z';
			else 
				buffer[x] = (m!=0) ?  m + 'a' - 1 : 'z';
		}
	}
} 
void decrypt2(){ //atbash + string reverse
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
void decrypt3(){ //shifting of (strlen) keys
	int x, oldch, newch;
	int offset = strlen(key);	
	for(x=0;x<length;x++){
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+26-offset)%26;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = (oldch+26-offset)%26;
				buffer[x] = newch + 'a';
			}	
		}else if (isdigit(buffer[x])){
			oldch = buffer[x] - '0';
			newch = (oldch+10-offset%10)%10;
			buffer[x] = newch + '0';
		}
	}	
} 
void decrypt4(){ //vigenere (original key)
	int x, ndx, keylen, offset, oldch, newch;	
	ndx=0;
	keylen=strlen(key);	
	for(x=0;x<length;x++){
		offset = key[ndx] - 'a';
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+26-offset)%26;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = (oldch+26-offset)%26;
				buffer[x] = newch + 'a';
			}
			ndx = (ndx+1)%keylen;
		}
	}	
} 
void decrypt5(){ //vigenere (reversed key)
	int x, ndx, keylen, offset, oldch, newch;	
	ndx=0;
	keylen=strlen(key2);	
	for(x=0;x<length;x++){
		offset = key2[ndx] - 'a';
		offset *= 2;
		if(isalpha(buffer[x])){			
			if(isupper(buffer[x])){
				oldch = buffer[x] - 'A';
				newch = (oldch+52-offset)%26;
				buffer[x] = newch + 'A';			
			}else if (islower(buffer[x])){
				oldch = buffer[x] - 'a';
				newch = (oldch+52-offset)%26;
				buffer[x] = newch + 'a';
			}
			ndx = (ndx+1)%keylen;
		}
	}	
} 
void decrypt6(){ //transposition (original key)
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
void decrypt7(){ //transposition (reversed key)
	int x, y, ndx, base=0, keylen, collength, min;	
	ndx=0;
	keylen=strlen(key2);
	//storing positions when sorting the letters of the key
	char cols[keylen+1], temp;
	for(x=0;x<keylen;x++){
		cols[x]=-1;
	}
	key2[keylen+1]=127;
	collength = length / keylen;
	//allocate space for buffer2 base on new length
	buffer2 = (char*)calloc(length,sizeof(char));
	newlength = length;
	//sorting
	for (x = 0; x < keylen; x++) {
		min = keylen+1;
    	for (y = 0; y < keylen; y++) {
			if (key2[y] < key2[min] && cols[y]==-1) {
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
	printf("\n=== STEP 3A: ===");
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

void getKey2(){
	strcpy(key2,key);
	strrev(key2);
}

void getRSA(){
	printf("\n=== STEP 3B: ===");
	if(mode==1){
		printf("\nEnter two prime numbers - p1 p2: (E.g.: 2 13)");
		printf("\n\nINPUT KEYS: ");
		scanf("%d %d", &p1, &p2);
	}else{
		printf("\nEnter the decryption key set - d N: (E.g. 5 26)");
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

	if(mode==2){ //remove extra spaces from transposition if decrypt
		int x;
		for(x=length-1;x>=0 && buffer[x]==' ';x--){}
		length = x + 1;
	}

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
	//free(buffer2);
	length = 0;
	newlength = 0;
	mode = 0;
	strcpy(key,"");
}
