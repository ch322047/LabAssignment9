#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{//do chaining with hashing, therefore linked lists
	struct RecordType *RTypePtr;
	struct HashType *next;
	int size;
//???
};

// Compute the hash function
int hash(int x)
{
x = (x % 23);// ??

return x;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block // might already be done for us
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz ; ++i ){// if index is occupied with any records, print all
		if(pHashArray[i].RTypePtr != NULL && pHashArray[i].RTypePtr->id != -1){//??
			printf("index %d ", i);

		//Since we are going to be printing a linked list, we should iterate thorugh
		//the linked list and print each thing until we are at the end

		struct HashType *Temp = &pHashArray[i];

		while((Temp != NULL) && (Temp->RTypePtr != NULL) && (Temp->RTypePtr->id != -1)){
			printf("-> %d, %c, %d", Temp->RTypePtr->id, Temp->RTypePtr->name, Temp->RTypePtr->order);
			Temp = Temp->next;
		}
	}
	printf("\n");
}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	//declare and initialize hashtable array
	struct HashType *pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * 50);

	for(int i = 0; i < 50; i++){
		pHashArray[i].RTypePtr = (struct RecordType*)malloc(sizeof(struct RecordType));

		pHashArray[i].RTypePtr->id = -1;
		pHashArray[i].RTypePtr->name = '\0';
		pHashArray[i].RTypePtr->order = -1;

		pHashArray[i].size = 0;
		pHashArray[i].next = NULL;
	}

	//iterate through hashtable

	for(int j = 0; j < recordSz; j++){
		int index = hash(pRecords[j].id);
		

		//Since we will iterate thorugh it as chains/linked list
		//we should have a hashtype pointer to iterate through it
		//to connect the list together
		struct HashType *CurrPtr = &pHashArray[index];

		if(CurrPtr->RTypePtr->id == -1){//if this specifc index is empty
			CurrPtr->RTypePtr = &pRecords[j];
		}

		else{//if that specific index is NOT empty
		//traverse until the end of the list and append the new thing to it
			while(CurrPtr->next != NULL){
				CurrPtr = CurrPtr->next;
			}

				CurrPtr->next = (struct HashType*)malloc(sizeof(struct HashType));
				CurrPtr->next->RTypePtr = &pRecords[j];
				CurrPtr->next->size = 1;
				CurrPtr->next->next = NULL;
			
		}
	}

	displayRecordsInHash(pHashArray, recordSz);
}