

#include "adc.h"
#include "sam.h"
#include <errno.h>
#include <stdio.h>


int adc_init(void) {
    // Enable peripheral clock for ADC (ID = 37)
    PMC->PMC_PCER1 |= (1 << (ID_ADC - 32));

    // Disable ADC write protection
    ADC->ADC_WPMR = ADC_WPMR_WPKEY(0x414443); // "ADC" in ascii and a 0 bit to disable all protection

    // Configure ADC mode register
    ADC->ADC_MR = ADC_MR_FREERUN_OFF | ADC_MR_PRESCAL(10) | ADC_MR_STARTUP_SUT64 | ADC_MR_TRACKTIM(3);
    // Til dum og dummere: -------------------------------------------------------------------------------------------------------
    // her kan vi potensielt sette over til FREERUN_ON for at ADCen skal lese av kontinuerlig, så slipper vi å starte en avlesning
    // og så vente på at den skal bli ferdig hver gang vi skal lese av

    // Enable the specified channel
    ADC->ADC_CHER = (1 << 0);

    // Deactivate PIO on PA2
    PIOA->PIO_PDR |= PIO_PDR_P2;

    // Give peripheral control over PA2 to ADC
    PIOA->PIO_ABSR |= PIO_PA2X1_AD0; // Peripheral A for PA2

    return 0;
}

int adc_read(uint16_t* result) {

    // Start conversion
    ADC->ADC_CR = ADC_CR_START;

    // Wait for conversion to complete
    while (!(ADC->ADC_ISR & (1 << 0)));

    // Read result
    *result = ADC->ADC_CDR[0];

    return 0;
}