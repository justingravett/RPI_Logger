#include <iostream>
#include "SerialPort.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_DATA_LENGTH 1

char incomingData[MAX_DATA_LENGTH];

//Arduino SerialPort object
SerialPort *arduino;

void ReceiveData(ofstream& filename)
{
    int readResult = arduino->readSerialPort(incomingData, MAX_DATA_LENGTH);     
    if (readResult >= 1){
        filename.write(incomingData,sizeof(incomingData));
    }
}

int main()
{    
    
    //Read in the port name from file: 
    std::ifstream conFile ("rpi_serial.conf");
    char inString[100]; 
    conFile >> inString; 
    conFile.close();
    char* portName = inString; 

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
    arduino = new SerialPort(portName);

    //Checking if Arduino is connected or not:
    if (arduino->isConnected()){
        std::cout << "Connection established at port " << portName << endl;
    }
    
    //Log the ADC data:
    while(arduino->isConnected()){   
        ReceiveData(logfile);
    }
    
    //find a clean way to exit the code:
    std::cout << "Arduino disconnected"; 
    logfile.close();
}
