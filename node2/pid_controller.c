
#include "sam.h"
#include "pid_controller.h"
#include "motor_driver.h"

volatile int16_t integral = 0;

void update_integral(uint16_t pos, uint16_t current_deviation){
    integral += pos - current_deviation; 
}

uint16_t get_integral(){
    return integral;
}

int16_t PI(PID_t *pid, uint16_t referance){

    // Get deviation
    uint16_t pos = read_encoder();
    uint16_t e = pos - referance;

    // update integral
    update_integral(pos, e);
    
    uint16_t K_P_prod = pid->K_P * e;
    uint16_t I_prod = pid->T * pid->K_I * integral;

    return K_P_prod + I_prod;
}
