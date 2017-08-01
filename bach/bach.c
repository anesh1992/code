/***********************************************************************
 *----------------------------------------------------------------------
 *				Program to log Data in file
 -----------------------------------------------------------------------
 *@ Author 		: Anesh Kumar
 *@ Email		: anesh_kumar@yahoo.com
 *@ Version		:
 *@ Date		: May 13, 2017
 *@ Brief		: Creating a solution to manage a log file
 						1. Adding records to a log file (the log file has a predefined format)
 						2. Printing the contents of a log file
*/
/* ________________________________________________________________________
   |
*/  




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

/*
* #pragma pack instructs the compiler to pack structure members with particular alignment.
* will insert padding between members 
* to ensure that they are aligned to appropriate addresses in memory
*/
#pragma pack(1)

void setupLog (char *fileName);
// ----: create file and add initial setup data
uint16_t getNumberOfRecords (char *fileName);
// ----: Read the 1st byte of log and return number of records
uint8_t getAuxAndDst (int auxFlag);
// ----: Find flag value by checking auxiliary and DST flag
uint32_t epochTimeStamp ();
// ----: Return current timestamp(after 2000)
uint8_t calculateRecordChecksum(FILE *ptrToFile);
// -----: Calculate the checksum from the current record just entered
uint8_t calculateLogChecksum(FILE *ptrToFile);
// -----: Calculate the checksum of all data inside log file
void addRecordToLog(uint8_t updateAuxDst, char *message, char *fileName);
// ----: Function add record (sequence number, text message, checksum etc.) to log file
void dumpLog (char *fileName);
// -----: Function display the logfile data to terminal/command line
void help ();
// -----: User when do mistakes or do not enter correct format from command line, it throws suggestions
void wrongArgument ();
// -----: Tells user about wrong argument

typedef struct logStruct{
    uint16_t recordTotal;
    uint16_t logSpareByte;
	int8_t logChecksum;
} logStruct;
//data for log file

typedef struct recordStruct {
        uint16_t sequenceNumber;
        uint8_t flag;
        uint32_t timeStamp;
        uint16_t spareByte;
        //text message should not be more than 30 character, later i have taken care of this
        char textMessage[30];
        int8_t checksum;
} recordStruct;
// data for record


int main(int argc, char *argv[])
{
    char *fileName;
    char *inputCharacter;
    uint8_t setAuxiliaryFlag = 0;

	if (argc == 1){
		//Case when user doesn't enter any argument at all, i.e, ./abcd.out
		printf ("No Argument Entered.\n"
			"\tPlease enter argument.\n");
		help();
	}
	else if (argc == 7 || argc == 6){
		//case: (if number of argument 7 or 6), it could be for appending
		if ((strncmp(argv[1], "appendlog", 9) == 0) && (strncmp(argv[2], "[-a]", 4) == 0) && (strncmp(argv[3], "-f", 2) == 0) && (strncmp(argv[5], "-t", 2) == 0)){
			// case: matching specified format for appending including [-f].
			printf("main(): PROGRAM IN APPEND MODE\nfilename: %s\nText to append: %s\n", argv[4], argv[6]);
			if (strlen(argv[6]) > 30){
				printf("characters to append is too long, consider only 30 characters\n");
				exit(0);
			}
            fileName = argv [4];
            inputCharacter = argv[6];
    		setAuxiliaryFlag = getAuxAndDst(1);
    		setupLog(fileName);
    		//seting up log file
    		addRecordToLog(setAuxiliaryFlag, inputCharacter ,fileName);
    		//adding record to log
		}
		else if ((strncmp(argv[1], "appendlog", 9) == 0) && (strncmp(argv[2],"-f", 2) == 0) && (strncmp(argv[4], "-t", 2) == 0)){
			//case: matching specified format for appending excluding [-f]
			if (strlen(argv[5]) > 30){
				printf("characters to append is too long, consider only 30 characters\n");
				exit(0);
			}
			printf ("mai(): PROGRAM IN APPEND MODE\nfileName: %s\ntext to append: %s\n", argv[3], argv[5]);
            fileName = argv [3];
            inputCharacter = argv[5];
    		setAuxiliaryFlag = getAuxAndDst(0);
    		setupLog(fileName);
    		//seting up log file
    		addRecordToLog(setAuxiliaryFlag, inputCharacter ,fileName);
    		//adding record to log
		}
		else {
			wrongArgument();
			//Throwing suggestions to user
		}
	}
	else if (argc == 4){
		//case: if (number of argument = 4), it could be for dumplog
		if ((strncmp(argv[1], "dumplog", 7) == 0) && (strncmp(argv[2], "-f", 2) == 0)){
			// case: matching specified format for dumplog
			printf("main(): PROGRAM IN LOG DUMP MODE\nfilename: %s\n", argv[3]);
			fileName = argv[3];			
			dumpLog (fileName);
		}
		else {
			wrongArgument();
		}
	}
	else {
		wrongArgument();
	}


	return 0;
}


void help (){
	printf ("-------------------------------Help--------------------------\n"
					"\tFollow the below format\n"
					"\t\tExample:./bach.out  appendlog [-a] -f LOGFILE -t TEXT_MESSAGE\n"
					"\tDESCRIPTION:\n"
					"\t\tThe appendlog utility appends the text message specified by the –t option to the log file"
					"specified by the –f option. If the specified file does not exist, it is created.If the optional"
					"–a parameter is set, the auxiliary flag will be set for the log record.\n"
					"---------------------------------OR---------------------------\n"
					"\t\tExample: ./bach.out dumplog -f LOGFILE\n"
					"\tDESCRIPTION:\n"
					"\t\tThe dumplog utility prints each record in the log file specified by the –f option to standard"
					"output. Printed output is decoded in a user-friendly way, eliminating the need to refer to the file"
					"format specification.\n\n\n");
}

void wrongArgument (){
	printf ("You have entered wrong argument\n");
	help ();
}

uint32_t epochTimeStamp (){
	time_t sec;
    sec = time (NULL);
    //Number of second between midnight January 1, 1970 to midnight January 1, 2000 is 946684800.
    //https://www.epochconverter.com/

    return (sec - 946684800);
}

void setupLog (char *fileName){
    logStruct log;
    log.recordTotal = 0;
    log.logSpareByte = 0;
    log.logChecksum = 0;
    FILE *ptrToFile;
	ptrToFile = fopen(fileName,"rb");
	//Open file in read mode
	if(ptrToFile != NULL) {
		//if exist no need to run setup, just exit
		fclose(ptrToFile);
	} else {
		ptrToFile = fopen(fileName,"wb");
		//Open file in write mode and add setup data 
		fwrite(&log, sizeof(log), 1, ptrToFile);
		fclose(ptrToFile);
	}
}




void addRecordToLog(uint8_t updateAuxDst, char *message, char *fileName){
    //Below few lines collect data for record and make necessary arrangement too.
    logStruct log;
    recordStruct record;
	log.recordTotal = getNumberOfRecords(fileName) + (uint16_t)1;
	record.sequenceNumber = log.recordTotal;
	record.flag = updateAuxDst;
	record.timeStamp = epochTimeStamp ();
	record.spareByte = 0;
	//Initially Padding space(ASCII 32) in all 30 bytes location
	memset(record.textMessage, ' ',sizeof(record.textMessage));
	//adding message to those location, empty locations will contain space(ASCII 32)
    strncpy(record.textMessage, message, (strlen(message)) );

    //Write data to log file
    FILE *ptrToFile;
	ptrToFile = fopen(fileName,"r+b");
	//Important: File must open in "r+" mode, "a" mode won't allow reading previous log data, won't be able to move fseek()
	if(ptrToFile != '\0') {
        //First update number of records in first memory address of log file.
        fwrite(&log.recordTotal, sizeof(log.recordTotal), 1, ptrToFile);
        //((Go to end of the line) - 1) memory location
        fseek(ptrToFile, -1, SEEK_END);
        // Write record data to file except checksum
        fwrite(&record, (sizeof(record) - 1), 1, ptrToFile);
        //read the current record from file and calculate checksum
        record.checksum = calculateRecordChecksum(ptrToFile);
        //write checksum for current record
        fwrite(&record.checksum, sizeof(record.checksum), 1, ptrToFile);
        //read the entire file and calculate checksum
        log.logChecksum = calculateLogChecksum(ptrToFile);
        //write checksum for entire log
        fwrite(&log.logChecksum, sizeof(log.logChecksum), 1, ptrToFile);
        fclose(ptrToFile);
	} else {
		printf("ERROR WRITING\n");
	}
}

uint8_t getAuxAndDst (int auxFlag){    
	//Auxilliary flag we get from command line by verifying [-a] tag, below lines find DST flag
	time_t rawtime;
   	struct tm *info;
   	// struct tm (it's not typedef, don't forget) is a structure defind in time.h contain all date and timing information in easy usable format
   	time ( &rawtime );
 	//Stealing time and in rawtime variable 
   	info = localtime( &rawtime );   

   	//Following test cases are just to test auxilliary and info->tm_isdt and return correct flag value
    if (auxFlag == 1 && info->tm_isdst == 1){
        return (uint8_t) 192; // 1100 0000
    }
    else if (auxFlag == 0 && info->tm_isdst == 1){
        return (uint8_t) 64;    //0100 0000
    }
    else if (auxFlag == 1 && info->tm_isdst == 0){
        return (uint8_t) 128;   //1000 0000
    } else {
        return (uint8_t) 0;     //0000 0000
    }
}

uint16_t getNumberOfRecords (char *fileName){
    uint16_t numberOfRecords;
    FILE *ptrToFile;
	ptrToFile = fopen(fileName,"rb");
	fseek(ptrToFile, 0, SEEK_SET);
	fread(&numberOfRecords, 2, 1, ptrToFile );
	// Read first 2 byte of of log file, that contains number of records previously added
	fclose(ptrToFile);
	return numberOfRecords;
}

uint8_t calculateRecordChecksum(FILE *ptrToFile){
    uint8_t readlog[40];
    uint8_t sum = 0;
    uint8_t checksum = 0;
    fseek(ptrToFile, -39 , SEEK_END);
    //Go to end of the log and offset(means come back 39 byte towards top of file) -39 byte 
    fread(readlog, 1, 39, ptrToFile );
    // From that location read for 39 bytes
    for (int i =0; i < 39 ; i++)
    	//iterate to array and add them
        sum = sum + readlog[i];
    checksum = 255 - sum;
    //above statement is same as taking complement (not 1's complement)
    return checksum;
}

uint8_t calculateLogChecksum(FILE *ptrToFile){
    size_t maxBufferSize = 45;
    uint8_t *readlog;
    uint8_t *reAllocatedMem;
    uint8_t sum = 0;
    uint8_t checksum = 0;
    fseek(ptrToFile, 0, SEEK_END);
    //Go to bottom most line of log file
    long fileSize = ftell(ptrToFile);
    //ftell() return current position in terms of number of data(not memory location)

    //Below few lines that include malloc and realloc wasn't necessary,
    // but many embedded processors have very limited stacks, so decided to use heap
    readlog = malloc(maxBufferSize);
    if (fileSize > 45){
        maxBufferSize = fileSize;
        reAllocatedMem = realloc(readlog, maxBufferSize);
        if (reAllocatedMem){
            readlog = reAllocatedMem;
        } else {
            printf ("Memory not allocated\n");
        }
    }
    rewind(ptrToFile);
    //again go to top most byte of log file
    fread(readlog, 1, (int)fileSize, ptrToFile );
    //read all log file 
    for (int i =0; i < (int)fileSize ; i++)
        sum = sum + readlog[i];
    checksum = 255 - sum;
    free(readlog);
    return checksum;
}

void dumpLog (char *fileName){
    logStruct log;
    FILE *ptrToFile;
    ptrToFile = fopen(fileName,"rb");
    //First read log setup data (means first 4 bytes) 
    fread(&log.recordTotal, sizeof(log.recordTotal),1, ptrToFile );
    printf("\t_________________________________________________________________________________________\n");
    printf("\t|Data\t\t\t\t\tLogged Data\t\t\t\tSize \n");
    printf("\t|___________________________________|___________________________________|________________|\n");
    printf("\t|Total Record\t\t\t\t%d\t\t\t\t\t%d Byte\n", log.recordTotal, (int)sizeof(log.recordTotal));
    printf("\t|___________________________________|___________________________________|________________|\n");
    fread(&log.logSpareByte, sizeof(log.logSpareByte),1, ptrToFile );
    printf("\t|Spare Byte (2 Byte)\t\t\t%d\t\t\t\t\t%d Byte\n", log.logSpareByte, (int)sizeof(log.logSpareByte));
    printf("\t|___________________________________|___________________________________|________________|\n");
    recordStruct record[log.recordTotal];
    fseek(ptrToFile, 4, SEEK_SET);
    //From top go to 5th byte and start reading record file till last byte -1
    fread(record, sizeof(recordStruct), log.recordTotal, ptrToFile);
    for (int i = 0; i< log.recordTotal; i++){
        printf("\t|\tSequence Number\t\t\t%d\t\t\t\t\t%d Byte\n", record[i].sequenceNumber, (int)sizeof(record[i].sequenceNumber));
        printf("\t|___________________________________|___________________________________|________________|\n");
        printf("\t|\tFlags\t\t\t\t%d\t\t\t\t\t%d Byte\n", record[i].flag, (int)sizeof(record[i].flag));
        printf("\t|___________________________________|___________________________________|________________|\n");
        printf("\t|\tTimestamp\t\t\t%d\t\t\t\t%d Byte\n", record[i].timeStamp, (int)sizeof(record[i].timeStamp));
        printf("\t|___________________________________|___________________________________|________________|\n");
        printf("\t|\tSpare Byte (2 byte)\t\t%d\t\t\t\t\t%d Byte\n", record[i].spareByte, (int)sizeof(record[i].spareByte));
        printf("\t|___________________________________|___________________________________|________________|\n");
        printf("\t|\tText Message\t\t\t");
        for (int j = 0; j < 30; j++)
            printf("%c", record[i].textMessage[j]);
        printf("\t\t%d Byte", (int)sizeof(record[i].textMessage));
        printf("\n\t|___________________________________|___________________________________|________________|\n");
        printf("\t|\tChecksum\t\t\t%d\t\t\t\t\t%d Byte\n", record[i].checksum, (int)sizeof(record[i].checksum));
        printf("\t|___________________________________|___________________________________|________________|\n");
    }
    //Finally read last byte that is checksum of log file
    fread(&log, sizeof(log.logChecksum),1, ptrToFile );
    printf("\t|Checksum\t\t\t\t%d\t\t\t\t\t%d Byte\n", log.logChecksum, (int)sizeof(log.logChecksum));
    printf("\t|___________________________________|___________________________________|________________|\n");
}
