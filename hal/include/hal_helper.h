#ifndef _HAL_HELPER_H_
#define _HAL_HELPER_H_

// BeagleBone hardware level
// This module provides helper functions for read/write into GPIO files
// - Support Write operation to GPIO files
// - Support Read operation to GPIO files

void writeToFile(const char* fileToWriteGpio, const char* content);
void readFromFile(const char* fileToReadGpio, char* buff, unsigned int maxLength);
// Partially provided by Brian Fraser, reads a value from a file and returns it
// Edited from Daven's assignment 1
int valueReader(char *fileName);
// Provided by Brian Fraser (Runs a linux command)
void runCommand(char* command);

// Provided by Brian Fraser
void write_int_to_file(const char* filename, int value);
void fileWriter(char* filePath, char* data);
// Provided by Brian Fraser
void setIntValueInFile(const char* filename, int value);

#endif