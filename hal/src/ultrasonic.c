#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "hal_helper.h"
#include "app_helper.h"
#include "ultrasonic.h"
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include "joystick.h"

#define ECHO_PATH_DIRECTION       "/sys/class/gpio/gpio66/direction" // 
#define ECHO_PATH_VALUE       "/sys/class/gpio/gpio66/value" //
#define TRIGGER_PATH_DIRECTION    "/sys/class/gpio/gpio67/direction" //
#define TRIGGER_PATH_VALUE        "/sys/class/gpio/gpio67/value" 

#define SPEED_OF_SOUND_CM_PER_SEC 34300 // Speed of sound in cm/s
#define SPEED_OF_SOUND_CM_PER_MS 34.3 // Speed of sound in cm/ms
#define SPEED_OF_SOUND_CM_PER_US 0.0343 // Speed of sound in cm/us
#define SPEED_OF_SOUND_CM_PER_NS  0.0000343 // Speed of sound in cm/ns

//static pthread_t ultrasonicThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//bool endProgram = false;
intmax_t  distance[3];
int count = 0;

//static bool endProgram = false;
void* ultrasonicLoop();

void initializeUltrasonic() {
    runCommand("config-pin p8.07 gpio");
    runCommand("config-pin p8.08 gpio");
    writeToFile(ECHO_PATH_DIRECTION, "in");
    writeToFile(TRIGGER_PATH_DIRECTION, "out");
    //pthread_create(&ultrasonicThread, NULL, ultrasonicLoop, NULL);
}

// void* ultrasonicLoop() {
//     while (!endProgram) {
//         // Measure the length of the pulse on the echo pin
//         intmax_t startTime = 0;
//         intmax_t initialTime = 0;
//         intmax_t stopTime = 0;
//         intmax_t elapsed_time_ns = 0;
//         intmax_t timeout_ns= 1000000000; // 1 second
//         // Send a 10us pulse to trigger pin
//         pthread_mutex_lock(&mutex);
//         writeToFile(TRIGGER_PATH_VALUE, "1");
//         sleepForMs(0.001);  // Wait 0.001ms
//         writeToFile(TRIGGER_PATH_VALUE, "0");
//         //printf("did the trigger\n");
//         initialTime = getCurrentTimeNanoseconds();
//         while (valueReader(ECHO_PATH_VALUE) == 0 && elapsed_time_ns < timeout_ns) {  // Wait for echo to go high
//             startTime = getCurrentTimeNanoseconds();
//             elapsed_time_ns = startTime - initialTime;
//         }
//         elapsed_time_ns = 0;
//         while (valueReader(ECHO_PATH_VALUE) == 1 && elapsed_time_ns < timeout_ns) {
//             //printf("Waiting for echo to return\n");
//             stopTime = getCurrentTimeNanoseconds();
//             elapsed_time_ns = stopTime - startTime;
//         }
//         if (elapsed_time_ns >= timeout_ns) {
//             //printf("Timeout! Object is too far.\n");
//             distance[count] = 650;
//         } else {
//             intmax_t timeElapsed = stopTime - startTime;
//             //double timeElapsedInSec = (double)timeElapsed/1000;
//             // Calculate distance in centimeters (assumes speed of sound is 343m/s)
//             distance[count] = timeElapsed * SPEED_OF_SOUND_CM_PER_NS / 2.0;  // in cm
//             //printf("Time elapsed is: %lld ms\n", timeElapsed);
//             if (distance[count] < 0)
//             {
//                 distance[count] = 400;
//             }
//         }
//         pthread_mutex_unlock(&mutex);
//         count++;
//         if (count > 4)
//         {
//             count = 0;
//         }
//         sleepForMs(100);  // Wait before next measurement
//     }
//     return 0;
// }

intmax_t getDistance(){
        // Measure the length of the pulse on the echo pin
        int count = 0;
        while (count < 3)
        {
            intmax_t startTime = 0;
            intmax_t initialTime = 0;
            intmax_t stopTime = 0;
            intmax_t elapsed_time_ns = 0;
            intmax_t timeout_ns= 1000000000; // 1 second
            // Send a 10us pulse to trigger pin
            pthread_mutex_lock(&mutex);
            writeToFile(TRIGGER_PATH_VALUE, "1");
            sleepForMs(0.001);  // Wait 0.001ms
            writeToFile(TRIGGER_PATH_VALUE, "0");
            //printf("did the trigger\n");
            initialTime = getCurrentTimeNanoseconds();
            while (valueReader(ECHO_PATH_VALUE) == 0 && elapsed_time_ns < timeout_ns) {  // Wait for echo to go high
                startTime = getCurrentTimeNanoseconds();
                elapsed_time_ns = startTime - initialTime;
            }
            elapsed_time_ns = 0;
            while (valueReader(ECHO_PATH_VALUE) == 1 && elapsed_time_ns < timeout_ns) {
                //printf("Waiting for echo to return\n");
                stopTime = getCurrentTimeNanoseconds();
                elapsed_time_ns = stopTime - startTime;
            }
            pthread_mutex_unlock(&mutex);
            if (elapsed_time_ns >= timeout_ns) {
                //printf("Timeout! Object is too far.\n");
                distance[count] = 650;
            } else {
                intmax_t timeElapsed = stopTime - startTime;
                //double timeElapsedInSec = (double)timeElapsed/1000;
                // Calculate distance in centimeters (assumes speed of sound is 343m/s)
                distance[count] = timeElapsed * SPEED_OF_SOUND_CM_PER_NS / 2.0;  // in cm
                if (distance[count] < 0)
                {
                    distance[count] = 650;
                }
                
                //printf("Time elapsed is: %lld ms\n", timeElapsed);
            }
            return 2000;
            count++;
            sleepForMs(100);  // Wait before next measurement
        }
        intmax_t totalDistance = distance[0] + distance[1] + distance[2];
        return totalDistance/count;
}

// intmax_t getDistance(){
//     sleepForMs(3000);
//     pthread_mutex_lock(&mutex);
//     intmax_t totalDistance = distance[0] + distance[1] + distance[2]+ distance[3] + distance[4];
//     pthread_mutex_unlock(&mutex);
//     return totalDistance/5;
// }

void ultrasonicShutdown() {
    //endProgram = true;
    //pthread_join(ultrasonicThread, NULL);
}

