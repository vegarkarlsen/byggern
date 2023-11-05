#include "IR_driver.h"
#include "timer.h"
#include "component/component_adc.h"
#include "component/component_pio.h"
#include "instance/instance_adc.h"
#include "pio/pio_sam3x8e.h"
#include "sam3x8e.h"
#include <stdint.h>

uint8_t detected_goal = 0;
// ADC configuration, page 1317
// using CH0: This is extrafunction of PA2 (see table 9-2 page 40) PA2 in arduino is AD7 on shild.
void ir_init(){
    // Enable pmc clock for ADC 
    PMC->PMC_PCER1 |= PMC_PCER1_PID37;
    // Disable write protect
    ADC->ADC_WPMR &= ~ADC_WPMR_WPEN;
    // Mode register:
    // Dont wait for any triggers
    ADC->ADC_MR |= ADC_MR_FREERUN_ON; 
    // ADC_clock = MCK/((PRESCAL +1 ) *2)
    // Dont know what to set, randonly chooseing 16
    ADC->ADC_MR |= ADC_MR_PRESCAL(16);
    // Enable channel 0
    ADC->ADC_CHER |= ADC_CHER_CH0; 
}

// see page 1351 and page 1322
uint16_t read_ir_raw(){
    // TODO: mabye wait for conversion?
    return ADC->ADC_CDR[0]; 
}

void detect_goal(uint8_t *goals){
    uint16_t raw_signal = read_ir_raw();
    if (raw_signal<LOWER_THRESHOLD){
        detected_goal = 1;
        return;
        //return goals;
    }
    else if (raw_signal>UPPER_THRESHOLD && detected_goal){
        detected_goal = 0;
        *goals = *goals + 1;
        //return new_goals;
        return;
    }
    else {
        //return goals;
        return;
    }
}



