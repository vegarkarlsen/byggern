
#include "pwm_lib.h"
#include "sam.h"
#include "component/component_pmc.h"
#include "component/component_pwm.h"
#include "sam3x8e.h"
#include <stdint.h>
#include <stdio.h>
#include "uart_and_printf/printf-stdarg.h"
#include "uart_and_printf/uart.h"

void pwm_init(){

    // enable PWM in PMC?
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    // disable PIOC18
    PIOC->PIO_PDR |= PIO_PDR_P18;
    /* // choose peripheral B */ 
    PIOC->PIO_ABSR |= PIO_ABSR_P18;

    // Dissable write protect controll register
    PWM->PWM_WPCR = PWM_WPCR_WPCMD(0);
    // clock generator: use PREA/PREB clock divider, set PREA = MCP/128
    PWM->PWM_CLK = PWM_CLK_DIVA(1) | PWM_CLK_PREA(0x07);

    // enable PWM on Channel 6, and use PREA clock 
    PWM->PWM_CH_NUM[6].PWM_CMR |= PWM_CMR_CPRE_CLKA; 
    // Configure waveform alignment : set CALG => center aligned, CALG = 0 => left aligned
    /* PWM->PWM_CH_NUM[6].PWM_CMR = <CALG> */
    // Configure waveform polarity channel 6: set = output waveform start high level
    PWM->PWM_CH_NUM[6].PWM_CMR |= PWM_CMR_CPOL;
    
    // disable channel 6
    PWM->PWM_DIS = (1 << 6);
    // Configure period for channel 6, CPDR = mck/x * T
    // mck = 84Mhz, x = 128, T = 20ms
    // => CPDR = 13125 = 0x3345
    PWM->PWM_CH_NUM[6].PWM_CPRD = PWM_CPRD_CPRD(0x3345);

    // set duty cyle on channel 6 CDTT = DC[ms]/T * CPRD = 1.5ms/20ms * 13125 = 984
    PWM->PWM_CH_NUM[6].PWM_CDTY = PWM_CDTY_CDTY(984);

    // enable channel 6
    PWM->PWM_ENA = (1 << 6);

    // Syncnous channel = not (defalut)
    
    // Configure manual sync and manual update
    PWM->PWM_SCM = PWM_SCM_UPDM_MODE0; 

    // wait for PWM channel enable
    while ((PWM->PWM_SR & (1 << 6)) == 0){

    }

}

void set_pwn_duty_cycle(uint8_t prosentage){
    if (prosentage > 100){
        prosentage = 0;
    }

    uint16_t cdty_max = 1378;
    uint16_t cdty_min = 591;
    uint16_t cdty = prosentage * (cdty_max - cdty_min) / (100) + cdty_min;

    // update duty cyle
    PWM->PWM_CH_NUM[6].PWM_CDTYUPD = PWM_CDTY_CDTY(cdty); 
    // trigger duty cyle update on nex periode
    PWM->PWM_SCUC = PWM_SCUC_UPDULOCK;
}
