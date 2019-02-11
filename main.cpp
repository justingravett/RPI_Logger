#include <iostream>
#include "rs232.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <chrono>

using namespace std;



#define COMPORT			24 		// this is '/dev/ttyUSB0' (for arduino) or choose wherever you added '/dev/ttyAMA0' (for raspberry pi UART pins) to the list
#define BAUDRATE		230400	// or whatever baudrate you want
#define RECEIVE_CHARS	1		// or whatever size of buffer you want to receive

unsigned char incomingData[RECEIVE_CHARS];


void ReceiveData(ofstream& filename)
{
    int readResult = RS232_PollComport(COMPORT, incomingData, RECEIVE_CHARS);  
    if (readResult >= 1){  
        filename.write((const char*)incomingData,sizeof(incomingData));
    }
}

int main()
{    
    
   /* //Read in the port name from file: 
    std::ifstream conFile ("rpi_serial.conf");
    char inString[100]; 
    conFile >> inString; 
    conFile.close();
    char* portName = inString; 
*/ 

    //sequencing file numbers: 
    std::ifstream seqFile ("sequence.dat", std::ifstream::in); 
    int fileNum; 
    seqFile >> fileNum; 
    seqFile.close(); 
    fileNum++; 
    std::ofstream seqFile2 ("sequence.dat"); 
    seqFile2 << fileNum; 
    seqFile2.close(); 
    
    //set up file for data logging, write as binary to be fast:
    char fileHold[20]; 
    sprintf(fileHold,"FlightLog_%i.dat",fileNum);           
    char* fileName = fileHold; 
    std::ofstream logfile (fileName,std::ofstream::binary);
     
    //Open up the serial connection: 
    RS232_OpenComport(COMPORT, BAUDRATE,"8N1");

    /*//Checking if Arduino is connected or not:
    if (arduino->isConnected()){
        std::cout << "Connection established at port " << portName << endl;
    }
    */
    
    //Log the ADC data:
    while(1){   
        ReceiveData(logfile);
    }
    
    //find a clean way to exit the code:
    std::cout << "Arduino disconnected"; 
    logfile.close();
}
