
#include "sam.h"
#include "pid_controller.h"
#include "motor_driver.h"
#include <stdint.h>
#include <stdio.h>

volatile int16_t integral = 0;

void update_integral(int16_t current_deviation){
    integral += current_deviation; 
}

uint16_t get_integral(){
    printf("hei how i am update_integral\n\r");
    return integral;
}

int16_t P(PID_t pid, uint16_t referance, uint16_t pos){
    int16_t e = (int16_t)(pos - referance);
    // printf("deviation: %d\n\r", e);

    int16_t u = (int16_t)pid.K_P * e;
    return u;
}

int16_t PI(PID_t pid, uint16_t referance, uint16_t pos){

    // Get deviation
    int16_t e = pos - referance;
    printf("e: %d\n\r",e);

    // update integral
    update_integral(e);
    
    int16_t K_P_prod = pid.K_P * e;
    int16_t I_prod = pid.T * pid.K_I * integral;
    printf("const part: %d\n\r", K_P_prod);
    printf("I part : %d\n\r", I_prod);

    return K_P_prod + I_prod;
}
