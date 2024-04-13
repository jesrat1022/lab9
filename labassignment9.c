//Esrat Jahan, 04-12-24, lab assignment 9, cop3502 sec 0005, lab sec 0038

#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// Fill out this structure
struct HashType
{
    struct RecordType* records;
    struct HashType* next;
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
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
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        struct HashType* ptr = &pHashArray[i];

        if (ptr->records != NULL)
        {
            printf("Index %d -> ", i);
            while (ptr != NULL)
            {
                printf("%d, %c, %d -> ", ptr->records->id, ptr->records->name, ptr->records->order);
                ptr = ptr->next;
            }
            printf("\n");
        }
    }
}


// Initialize the hash table
struct HashType* initHashTable(int hashSz)
{
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    for (int i = 0; i < hashSz; ++i)
    {
        hashTable[i].records = NULL;
        hashTable[i].next = NULL;
    }
    return hashTable;
}

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record, int hashSz)
{
    int index = hash(record.id, hashSz);
    struct HashType* ptr = &hashTable[index];

    // Find the last node in the chain
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    // Create a new node for the record
    struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
    newNode->records = (struct RecordType*)malloc(sizeof(struct RecordType));
    *newNode->records = record;
    newNode->next = NULL;

    // Insert the new node at the end of the chain
    ptr->next = newNode;
}


int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    int hashSz = 31; // Example size
    struct HashType* hashTable = initHashTable(hashSz);

    // Insert records into hash table
    for (int i = 0; i < recordSz; i++)
    {
        insertRecord(hashTable, pRecords[i], hashSz);
    }

    // Display records in hash table
    displayRecordsInHash(hashTable, hashSz);

    // Free memory
    for (int i = 0; i < hashSz; i++)
    {
        struct HashType* ptr = &hashTable[i];
        while (ptr != NULL)
        {
            struct HashType* temp = ptr;
            ptr = ptr->next;
            free(temp->records);
            free(temp);
        }
    }
    free(pRecords);
    free(hashTable);

    return 0;
}
