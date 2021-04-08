/**
 * @file datalogger.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief Implementation of datalogger functions.
 * @version 0.1
 * @date 2021-04-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>

#include "include/datalogger.h"
#include "include/datalogger_extern.h"

/* datalogger_directory
 * - log
 * - - eps
 * - - - settings.cfg
 * - - - 0.dat
 * - - - 1.dat
 * - - - 2.dat
 * - - - 3.dat
 * - - - 4.dat
 * - - - 5.dat
 * - - acs
 * - - - settings.cfg
 * - - - 0.dat
 * - - - 1.dat
 */

/*
.cfg file format
BEGIN
1. log index
2. max file size (KB)
3. max dir size (KB)
4. do overwrite

*/

// data: .data files
// settings: Datalogger's personal variable repository.
// directory: c-string, likely the name of the calling module
// Note: Directory will probably be accessed some other way eventually.
int logData(int size, int* dataIn, char* directory){
    // Constructs the settings.cfg directory into settingsFile.
    char settingsFile[20] = "/log/";
    strcat(settingsFile, directory); // "/log/eps"
    strcat(settingsFile, "/settings.cfg"); // "/log/eps/settings/cfg"

    // Index will get the data file number, can only accept up to 999.
    char sIndex[10]; // sIndex = c string index
    char sIndexP1[10]; // sIndex.toInt() + 1
    char sMaxFileSize[10];
    char sMaxDirSize[10];
    char sDoOverwrite[1];

    // Open the settings file and update numbers.
    FILE *settings = NULL;
    settings = fopen(settingsFile, "r+");

    if (settings = NULL){
        return SAVE_OPEN;
    }

    // Gets the index, maxSizes, and doOverwrite..
    if (fgets(sIndex, 10, (FILE*)settings) == NULL
    || fgets(sMaxFileSize, 10, (FILE*)settings) == NULL
    || fgets(sMaxDirSize, 10, (FILE*)settings) == NULL
    || fgets(sDoOverwrite, 1, (FILE*)settings == NULL)){
        return SAVE_ACCESS;
    }

    // Converts the retrieved string to an int.
    // In the case of index, it modulos it, and then
    // makes it a c-string again.
    int maxFileSize = atoi(sMaxFileSize);
    int maxDirSize = atoi(sMaxDirSize);
    int doOverwrite = atoi(sDoOverwrite);
    int index = atoi(sIndex) % (maxDirSize/maxFileSize); // 64 * 16KB == 1MB
    itoa(index, sIndex);
    itoa(index+1, sIndexP1); // Makes a c-string form of index+1

    // Constructs the n.dat directory.
    char dataFile[20] = "/log/";
    strcat(dataFile, directory); // "/log/eps"
    strcat(dataFile, "/"); // "/log/eps/"
    strcat(dataFile, sIndex); // "/log/eps/42"
    strcat(dataFile, ".dat"); // "/log/eps/42.dat"

    // Constructs the n+1.dat directory.
    char dataFileOld[20] = "/log/";
    strcat(dataFileOld, directory); // "/log/eps"
    strcat(dataFileOld, "/"); // "/log/eps/"
    strcat(dataFileOld, sIndexP1); // "/log/eps/43"
    strcat(dataFileOld, ".dat"); // "/log/eps/43.dat"

    // ! This is no longer needed because we overwrite 
    // Remove the old file, if it exists.
    // remove(dataFileOld);

    FILE *data = NULL;
    data = fopen(dataFile, "a");

    if (data = NULL){
        return DATA_OPEN;
    }

    long int fileSize = ftell(dataFile);

    // This file has reached its maximum size.
    // Make a new one and iterate the index.
    if (fileSize >= maxFileSize){ // 16KB
        fclose(settings);
        settings = fopen(settingsFile, "w");

        if (settings = NULL){
            return SAVE_OPEN;
        }

        // Rewrite the entire file.
        fprintf(settings, "%d\n", index+1);
        fprintf(settings, "%d\n", maxFileSize);
        fprintf(settings, "%d\n", maxDirSize);
        fprintf(settings, "%d\n", doOverwrite);

        fclose(data);
        data = fopen(dataFileOld, "w"); // This will overwrite the old file.
    }

    // TODO: Here is where we will write the passed data to the now opened .data file.
    // size_t fread(void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
    // size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);

    // fwrite(dataIn, elementSize, sizeof(data), data);

    fclose(settings);
    fclose(data);
}

// TODO: WIP
int* retrieveData(){

}

// TODO: set max etc can probably be condensed into a edit settings function... ?

int dlgr_editSettings(int value, int setting, char* directory){
    // TODO: Overwrite value.
    // Open settings.cfg
    // Retrieve data
    // Close settings.cfg
    // Open settings.cfg "w" (deleting the content)
    // Write modified data
    // Close settings.cfg
    // Most of this is already done in logData() (can be reused).
    
    // Constructs the settings.cfg directory into settingsFile.
    char settingsFile[20] = "/log/";
    strcat(settingsFile, directory); // "/log/eps"
    strcat(settingsFile, "/settings.cfg"); // "/log/eps/settings/cfg"

    char sIndex[10]; // sIndex = c string index
    char sIndexP1[10]; // sIndex.toInt() + 1
    char sMaxFileSize[10];
    char sMaxDirSize[10];
    char sDoOverwrite[1];

    // Open the settings file and update numbers.
    FILE *settings = NULL;
    settings = fopen(settingsFile, "r+");

    if (settings = NULL){
        return SAVE_OPEN;
    }

    // Gets the index, maxSizes, and doOverwrite..
    if (fgets(sIndex, 10, (FILE*)settings) == NULL
    || fgets(sMaxFileSize, 10, (FILE*)settings) == NULL
    || fgets(sMaxDirSize, 10, (FILE*)settings) == NULL
    || fgets(sDoOverwrite, 1, (FILE*)settings == NULL)){
        return SAVE_ACCESS;
    }

    // Converts the retrieved string to an int.
    int maxFileSize = atoi(sMaxFileSize);
    int maxDirSize = atoi(sMaxDirSize);
    int doOverwrite = atoi(sDoOverwrite);
    int index = atoi(sIndex);

    fclose(settings);

    // Adjust values as desired.
    //maxFileSize = size_B;
    switch(setting){
        case MAX_FILE_SIZE:
            if (value > SIZE_FILE_HARDLIMIT || value < 1){
                return SET_SETTING;
            }

            maxFileSize = value;

            break;
        case MAX_DIR_SIZE:
            // Memory amount out-of-bounds.
            if (value > SIZE_DIR_HARDLIMIT || value < 1){
                return SET_SETTING;
            }

            maxDirSize = value;

            break;
        case DO_OVERWRITE:
            // Psuedo-boolean value not binary.
            if (value != 0 && value != 1){
                return SET_SETTING;
            }

            doOverwrite = value;

            break;
    }

    // Reopen the file in w-mode, clearing the file contents.
    settings = fopen(settingsFile, "w");

    // Write everything back into the file.
    fprintf(settings, "%d\n", index);
    fprintf(settings, "%d\n", maxFileSize);
    fprintf(settings, "%d\n", maxDirSize);
    fprintf(settings, "%d\n", doOverwrite);

    fclose(settings);
}

/*
// Sets the maximum log size, in bytes. will create a new log file when this is hit. 
int dlgr_setMaxFileSize(int size_B, char* directory){
    // TODO: Overwrite value.
    // Open settings.cfg
    // Retrieve data
    // Close settings.cfg
    // Open settings.cfg "w" (deleting the content)
    // Write modified data
    // Close settings.cfg
    // Most of this is already done in logData() (can be reused).
    
    // Constructs the settings.cfg directory into settingsFile.
    char settingsFile[20] = "/log/";
    strcat(settingsFile, directory); // "/log/eps"
    strcat(settingsFile, "/settings.cfg"); // "/log/eps/settings/cfg"

    char sIndex[10]; // sIndex = c string index
    char sIndexP1[10]; // sIndex.toInt() + 1
    char sMaxFileSize[10];
    char sMaxDirSize[10];
    char sDoOverwrite[1];

    // Open the settings file and update numbers.
    FILE *settings = NULL;
    settings = fopen(settingsFile, "r+");

    if (settings = NULL){
        return SAVE_OPEN;
    }

    // Gets the index, maxSizes, and doOverwrite..
    if (fgets(sIndex, 10, (FILE*)settings) == NULL
    || fgets(sMaxFileSize, 10, (FILE*)settings) == NULL
    || fgets(sMaxDirSize, 10, (FILE*)settings) == NULL
    || fgets(sDoOverwrite, 1, (FILE*)settings == NULL)){
        return SAVE_ACCESS;
    }

    // Converts the retrieved string to an int.
    int maxFileSize = atoi(sMaxFileSize);
    int maxDirSize = atoi(sMaxDirSize);
    int doOverwrite = atoi(sDoOverwrite);
    int index = atoi(sIndex);

    fclose(settings);

    // Adjust values as desired.
    maxFileSize = size_B;

    // Reopen the file in w-mode, clearing the file contents.
    settings = fopen(settingsFile, "w");

    fclose(settings);
}

// Sets the maximum log directory size, will overwrite logs at dirSize/logSize logs. 
int dlgr_setMaxDirSize(int size_B, char* directory){
    // Overwrite the value of line 3.
    FILE* settings = NULL;
    settings = fopen("/sav/values.sav", "r+");
    if (settings = NULL){
        return SAVE_OPEN;
    }

    // TODO: Overwrite value.

    fclose(settings);
}

// 0 to ignore max directory size and never overwrite, 1 to behave
int dlgr_doOverwrite(int overwrite, char* directory){
    // Overwrite the value of line 4.
    FILE* settings = NULL;
    settings = fopen("/sav/values.sav", "r+");
    if (settings = NULL){
        return SAVE_OPEN;
    }

    // TODO: Overwrite value.

    fclose(settings);
}
*/