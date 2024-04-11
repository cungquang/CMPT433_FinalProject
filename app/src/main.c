#include <stdio.h>
#include <stdlib.h>
#include "../include/app_tcp_udp.h"
#include "camera.h"
#include "app_helper.h"
#include "buzzer.h"
#include "drive.h"

void camera_operation(void)
{
    Tcp_init();
    //char *imagePath = captureImage();
    char *imagePath="./image_10_23-12-39.jpg";
    int result_fromAI = Tcp_sendImage(imagePath);

    if (result_fromAI >= 99999)
    {
        printf("No human detected within the image!\n");
    } else {
        printf("Result from AI: %d pixel\n", result_fromAI);
    }
    Tcp_cleanUp();
}

int main_operation(void)
{
    Pwm_init();
    drive_init();    
    
    camera_operation();

    for(int i =0;i<10;i++){
        turn_left(90);
        turn_right(90);
        drive_set_both_wheels(true);
        sleepForMs(250);
        drive_set_both_wheels(false);
        sleepForMs(250);
    }
    
    camera_operation();

    drive_cleanup();
    return 0;

}

void tcp_testing(void)
{
    Tcp_init();
    char *imagePath = "./image_10_23-17-21.jpg";
    int result_fromAI = Tcp_sendImage(imagePath);
    Tcp_cleanUp();

    if (result_fromAI >= 99999)
    {
        printf("No human detected within the image!\n");
    } else {
        printf("Result from AI: %d pixel\n", result_fromAI);
    }
}

int main() 
{
    for(int i = 0; i < 30; i++)
    {
        tcp_testing();
    }
    return 0;
}