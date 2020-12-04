#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>

#define M 997    // Closest Prime number to 1000
#define MM 996   // M-1
#define R_HORNER 31
#define MAX 30   // It sets maximum length of some character arrays

struct node{    // My hashTable be formed by these nodes that keeps
	struct documents * head;	// Docs linked list head and
	char dictionary_word[MAX];  // words itself.
};
struct documents{ // To keep every doc contains the word, I used Linked list
	char document_name[MAX];
	struct documents * next;
};
typedef struct node node;
typedef struct documents docs;

double get_key(char *, int);				// The functions are described below
int hash(double, int, int);
void fillHash(node *, int, FILE *, int *);
void initializeHash(node *, int);
bool inList(docs *, char *);
docs * findTheDocs(node *, int, char *, int);
void printDocs(docs *);
int insertInto(node * ,int , char * , double, char *, int *);
void hashToFile(FILE *,node *, int, int *);

int main(){
	char filename[MAX];			// ; The file we keep hashtable in it
	char document[MAX];			// ; Given document file
	char words[MAX];			// ; To keep each word
	char readFileChar='x';		// ; I read files char by char
	char loadFactor[8];			// ; At the top of my txt I track Load Factor
	char ch;					// ; A character temp
	int i=0;					// ; Iterator
	int words_length;			// ; To keep each words length
	int process = 1;			// ; It terminates an infinity while
	int hashCon = 0;			// ; It tracks hash Size
	int whereTo;				// ; Shows where the program inserted
	FILE * fp;					// ; FILE pointer to open hash File
	FILE * fd;					// ; FILE pointer to open document File
	fp = fopen("data.txt","r");
	if(fp == NULL){
		perror("Unable to open file!");
		exit(1);
	}
	while(readFileChar != ':' && readFileChar != EOF){		// My txt File has ' LOAD FACTOR:0.000 ' at the top
		readFileChar = fgetc(fp);		
	}
	if(readFileChar != EOF){
		while(readFileChar != '\n' && readFileChar != EOF){	// I just want to read Load Factor
			readFileChar = fgetc(fp);
			if(readFileChar != EOF){
				loadFactor[i] = readFileChar;
				i++;
			}
		}
		loadFactor[i] = '\0';
		printf("LOAD FACTOR: %s\n",loadFactor);
		readFileChar = 'x';
	}else{
		printf("LOAD FACTOR: 0.000\n");
	}
	
	node * hashTable = (node *)malloc(M*sizeof(node));
	initializeHash(hashTable,M);
	if(readFileChar != EOF)
		if(strcmp(loadFactor,"0.000\n")!=0)				// To check if it needs a filling
			fillHash(hashTable,M,fp,&hashCon);
	
	fclose(fp);							// Reading process has Done
	
	
	printf("Enter the document name which you are going to hash(Ex. : document1.txt)");
	printf("\nIf you want to search words then enter 'WORD'\n");
	printf("If you want to exit enter -1\n");
	scanf("%s",document);
	while(strcmp(document,"-1") != 0){
		if(strcmp(document,"WORD")==0){
			//	Take words as input then search in hash and 
			//	display where the word exists
			printf("Enter the word you are searching for :\n");
			scanf("%s",words);
			words_length = strlen(words);
			for(i=0;i<words_length;++i)
				words[i] = tolower(words[i]);		// Making the program case insensitive
			docs * temp = findTheDocs(hashTable,M,words,words_length);	// It returns the head of wanted docs
			printDocs(temp);	// Prints the docs
		}else{
			i=0;
			fd = fopen(document,"r");
			int document_size = strlen(document);
			readFileChar = 'x';
			if(fd == NULL){
				perror("Unable to open document!");
			}else{
				while(readFileChar != EOF){
					i=0;
					if(readFileChar == ' ' || readFileChar == '\n')
						readFileChar = 'x';
					while((readFileChar != ' ' && readFileChar != '\n') && readFileChar != EOF){
						readFileChar = fgetc(fd);
						if((readFileChar != EOF && readFileChar != '\n') && readFileChar != ' '){
							words[i] = tolower(readFileChar);
							++i;
						}
					}
					
					words[i]='\0';	// Seperating new line, white space and ending with EOF
									// we got our word
					
					if(words[0] != '\0'){
						words_length = strlen(words);
						double word_key= get_key(words,words_length);	// to get double equivalent I use get_key
					
						
						whereTo = insertInto(hashTable,M,document,word_key,words,&hashCon); // Inserting process
						
						if(whereTo == -1){
							// Hash is full.
							printf("THE WORD : %s BELONGS THE DOC : %s COULD NOT BE ADDED BECAUSE YOUR HASH IS FULL !\n",words,document);
						}else if(whereTo == M+1){
							// Doc and word already together
						}					
					}
					
				}
			}
				
			fp = fopen("data.txt","w");
			hashToFile(fp,hashTable,M,&hashCon);	// I print hash table in to file
			fclose(fp);
			fclose(fd);
		}
		printf("\nEnter the document name which you are going to hash(Ex. : document1.txt)");
		printf("\nIf you want to search words then enter 'WORD'\n");
		printf("If you want to exit enter -1\n");
		scanf("%s",document);
	}

	free(hashTable);	// Free the allocated memory
	return 0;
}
double get_key(char * the_word,int words_length){	// To get a double equivalent of a string usşng HORNERS METHOD
	int i = words_length-1;
	double key = 0;
	while(i>-1){
		key += (pow(R_HORNER,i)) * (the_word[words_length-i-1] - 'a' +1);	// Usual horners method
		--i;
	}
	return key;		// It returns the double which represents the word
}
int hash(double key, int i,int hashSize){	// To find index where we are going to insert this function
											// returns key to index with an ' i ' variable
	/**[h1(key) + i*h2(key)] mod M**/
	int h1 = fmod(key,hashSize);
	int h2 = 1+fmod(key,hashSize-1);
	return (h1+(i*h2))%hashSize;	
}
int insertInto(node * hashTable,int tableSize , char * doc_name, double key, char * the_word, int * hashCon){ // Main function about hashing
	int i=0;
	int index = hash(key,i,tableSize);				// First getting the index value then we'll check if it is avaible
	
	while((i<tableSize && strcmp(hashTable[index].dictionary_word,the_word) != 0) 
			&& hashTable[index].head != NULL){
		++i;
		index = hash(key,i,tableSize);				// If it is not avaible, we are getting new index by incrementing ' i ' DOUBLE HASHING
		
	}
	if(hashTable[index].head == NULL){				// Why our loop is terminated, is it because an empty index founded
		docs * headDoc = (docs *)malloc(sizeof(docs));
		strcpy(headDoc->document_name,doc_name);
		headDoc->next = NULL;
		hashTable[index].head = headDoc;
		strcpy(hashTable[index].dictionary_word,the_word);
		*hashCon += 1;								// Then increment hashSize and insert it
		float loadFac = (float) *hashCon / tableSize;
		if(loadFac != 0.0 && loadFac < 0.001)
			printf("ADDED WORD : %-15s ,LOAD FACTOR:0.001\n",the_word);
		else
			printf("ADDED WORD : %-15s ,LOAD FACTOR:%.3f\n",the_word,loadFac);
		
		if(*hashCon >= (tableSize*0.8))
			printf("WARNING !! Your Hashs Load Factor is ABOVE 0.8, Load Factor:%.3f\n",
													((float)*hashCon / (float)tableSize));
	}else if(i<tableSize){							// Is it beacuse the same word has already inserted
		bool flag = inList(hashTable[index].head,doc_name);
		if(!flag){									// Then if the doc not exists with the word, attach it
			docs * newDoc = (docs *)malloc(sizeof(docs));
			strcpy(newDoc->document_name,doc_name);
			newDoc->next = hashTable[index].head;
			hashTable[index].head = newDoc;
		}else
			return tableSize+1;						// Return M+1 if the word and document already together , just incase
	}else
		return -1;									// Return -1 if there is no space in hash Table
	return index;									// Return the index otherwise
}
void fillHash(node * hashTable, int size, FILE * fp, int * hashCon){ // The function, fills the hash at the beginnig of our program, if the file
																	 // has already words in it
	int i;				// An Iterator
	char ch='x';		// Temporary character holder
	char holder[MAX];	// Temporary word holder
	char holder_doc[MAX]; // Temporary doc name holder
	fseek(fp,0,SEEK_SET);	// To start at the beginnig of our file
	while(ch!=EOF && ch!='\n')
		ch = fgetc(fp);
	while(ch!=EOF){
		i=0;
		ch = 'x';
		while((ch != EOF && ch != '\n') && ch != ','){
			ch = fgetc(fp);
			if((ch != EOF && ch != '\n') && ch != ','){
				holder[i] = ch;
			}
			i++;
		}
		holder[i-1]='\0';		// Seperate word from the docs by ','
		
		while(ch != EOF && ch != '\n'){
			i=0;
			ch = 'x';
			while((ch != EOF && ch != '\n') && ch != ','){
				ch = fgetc(fp);
				if((ch != EOF && ch != '\n') && ch != ','){
					holder_doc[i] = ch;
					}
				i++;
			}
			holder_doc[i-1] = '\0'; // Seperate each doc name by ','
			if(holder_doc[0] != '\0'){
				if(ch == ',' || ch == '\n'){
					int holder_length = strlen(holder);
					double key = get_key(holder,holder_length);
					int whereTo = insertInto(hashTable,size,holder_doc,key,holder,hashCon);	// Main inserting function
				}
			}
		}
		
	}
}
void initializeHash(node * hashTable, int size){	// To check NULL values we got to initialize Hash at the first
	int i;
	for(i=0;i<size;i++){
		hashTable[i].head = NULL;	
	}
}
bool inList(docs * head, char * doc_name){		// It checks if the word and document has already together in Hash
	docs * iter = head;
	while(iter != NULL && strcmp(iter -> document_name,doc_name) != 0){
		iter = iter -> next;
	}
	if(iter == NULL)							// It did not find
		return false;
	return true;								// Word and doc together
}
docs * findTheDocs(node * hashTable, int tableSize, char * word, int word_length){	// This functions basically hash Search
	
	int key = get_key(word,word_length);
	int i=0;
	int index = hash(key,i,tableSize);
	int step=1;
	
	while(i<tableSize && strcmp(hashTable[index].dictionary_word,word)!=0){
		i++;
		index = hash(key,i,tableSize);						// Trying to find the word
		step++;
	}
	printf("In %d STEP\n",step);
	if(i<tableSize){
		return hashTable[index].head;
	}
	
	return NULL;										// Function did not find the word
}
void printDocs(docs * head){				// Prints the list of documents which have the given word in it
	int flag = 0;
	docs * iter = head;
	printf("DOCUMENTS : ");
	while(iter != NULL){
		printf("%s ",iter->document_name);
		iter = iter->next;
		flag = 1;
	}
	if(flag == 0)
		printf("NOT FOUNDED !");
}
void hashToFile(FILE * fp, node * hashTable, int hashSize, int * loadFactor){	// To print hash to txt File
	int i;
	float loadFac = (float) *loadFactor / (float)hashSize;
	if(loadFac != 0.0 && loadFac < 0.01){
		fprintf(fp,"LOAD FACTOR:0.001\n");
	}else{
		fprintf(fp,"LOAD FACTOR:%.3f\n",loadFac);
	}
	for(i=0;i<hashSize;i++){
		docs * iter = hashTable[i].head;
		if(iter != NULL){
			fprintf(fp,hashTable[i].dictionary_word);
			fputc(',',fp);
			while(iter != NULL){
				fprintf(fp,iter->document_name);
				fputc(',',fp);					// Seperating with ','
				iter = iter -> next;
			}
			fputc('\n',fp);
		}		
	}	
}