
#include "sam.h"
#include "pid_controller.h"
#include "motor_driver.h"
#include <stdint.h>
#include <stdio.h>
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"

int16_t integral = 0;
int16_t previus_integral = 0;

void update_integral(int16_t current_deviation){
    printf("integral: %d\n\r", integral);
    integral += current_deviation; 
}

int16_t get_integral(){
    return integral;
}

int16_t P(PID_t pid, uint16_t referance, uint16_t pos){
    int16_t e = (int16_t)((pos - referance));

    // printf("deviation: %d\n\r", e);

    float u = (float)pid.K_P * e;
    return (int16_t)u;
}

int16_t PI(PID_t pid, uint16_t referance, uint16_t pos){

    // Get deviation
    int16_t e = (pos - referance);
    // printf("e: %d\n\r", e);
    if (e < 5 && e > -5){
        e = 0;
    }

    // update integral
    update_integral(e);
    
    float K_P_prod = pid.K_P * (float)e;
    float K_I_prod = pid.T * pid.K_I * (float)integral;
    
    // printf("const part: %d\n\r", (int)K_P_prod);
    // printf("I part : %d\n\r", (int)K_I_prod);

    int16_t u = (K_P_prod + K_I_prod);

    // Antiwindup
    if (u > 100){
    u = 100;
        integral = previus_integral;
    }
    else if (u < -100) {
        u = -100;
        integral = previus_integral;
    }
    
    else {
        previus_integral = integral;
    }

    return u;
}
