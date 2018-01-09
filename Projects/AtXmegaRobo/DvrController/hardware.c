#include "hardware.h"
#include <avr/pgmspace.h>
#include <stddef.h>

//xQueueHandle      xSpiRx;             /// Kolejka z odebranymi bajtami z SPI. Blokuje transmisję do czasu zakończenia wysyłania poprzedniego bajtu
//xQueueHandle      xSpiRxEnc;

uint8_t ReadCalibrationByte( uint8_t index )
{
    uint8_t result;

    /* Load the NVM Command register to read the calibration row. */
    NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
    result = pgm_read_byte(index);

    /* Clean up NVM Command register. */
    NVM_CMD = NVM_CMD_NO_OPERATION_gc;

    return( result );
}

void hardwareInit(void)
{
    /** PORT A
    0 - ACS711 - current sensor
    1 - Voltage divider - power supply voltage sensor
    2 - Out 7
    3 - Out 8
    4 - Out 5
    5 - Out 6
    6 - Out 9
    7 - Out 10          **/
    PORTA.DIR=0xFC;
    PORTA.OUT=0x00;

    /** PORT B
    0 - Out 11
    1 - Out 12
    2 - In 1
    3 - In 2           **/
    PORTB.DIR=0x03;
    PORTB.OUT=0x00;

    /** PORT C
    0 - I2C
    1 - I2C
    2 - UART VTY RxD
    3 - UART VTY TxD
    4 - In 3
    5 - In 4
    6 - UART radio RxD
    7 - UART radio TxD   **/
    PORTC.REMAP=((1<<PORT_TC0A_bp)|(1<<PORT_TC0B_bp)) ;
    PORTC.DIR=0x88;
    PORTC.OUT=0x00;

    /** PORT D
    0 - Radio set/reset
    1 - Sim900 reset
    2 - Sim900 RxD
    3 - Sim900 TxD
    4 - Sim900 PWR key
    5 - Rs485 En
    6 - Rs485 Rx
    7 - Rs485 Tx         **/
    PORTD.DIR=0xBB;
    PORTD.OUT=0x00;

    /** PORT E
    0 Out 3
    1 Out 4
    2 RPI RxD
    3 RPI TxD          **/
    PORTE.DIR=0x0B;
    PORTE.OUT=0x03;

    /** PORT R
    0 Out 1
    1 Out 2            **/
    PORTR.DIR=0x03;
    PORTR.OUT=0x00;


    /// A/C init
    //ADCA.CTRLA     = ADC_ENABLE_bm | ADC_CH0START_bm;        //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0
    //ADCA.CTRLB     = 0x80;//ADC_LOWIMP_bm;                            //Sprawdzam, czy poprawi się jakość pomiaru
    ADCA.REFCTRL   = ADC_BANDGAP_bm | ADC_TEMPREF_bm | ADC_REFSEL_INTVCC_gc;          //BANDGAP enable, TempRef enable, Vref = VCC/1.6 V
    ADCA.EVCTRL    = 0;
    ADCA.PRESCALER = ADC_PRESCALER_DIV256_gc;                  //prescaler 256, f=125kHz
    ADCA.INTFLAGS  = 0x0F;

    ADCA.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0) );
    ADCA.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1) );
}

inline void setOut1   (void) { PORTR.OUTSET=0x01; } //R0
inline void clearOut1 (void) { PORTR.OUTCLR=0x01; }
inline void setOut2   (void) { PORTR.OUTSET=0x02; } //R1
inline void clearOut2 (void) { PORTR.OUTCLR=0x02; }

inline void setOut3   (void) { PORTE.OUTSET=0x01; } //E0
inline void clearOut3 (void) { PORTE.OUTCLR=0x01; }
inline void setOut4   (void) { PORTE.OUTSET=0x02; } //E1
inline void clearOut4 (void) { PORTE.OUTCLR=0x02; }

inline void setOut5   (void) { PORTA.OUTSET=0x10; } //A4
inline void clearOut5 (void) { PORTA.OUTCLR=0x10; }
inline void setOut6   (void) { PORTA.OUTSET=0x20; } //A5
inline void clearOut6 (void) { PORTA.OUTCLR=0x20; }

inline void setOut7   (void) { PORTA.OUTSET=0x04; } //A2
inline void clearOut7 (void) { PORTA.OUTCLR=0x04; }
inline void setOut8   (void) { PORTA.OUTSET=0x08; } //A3
inline void clearOut8 (void) { PORTA.OUTCLR=0x08; }

inline void setOut9   (void) { PORTA.OUTSET=0x40; } //A6
inline void clearOut9 (void) { PORTA.OUTCLR=0x40; }
inline void setOut10  (void) { PORTA.OUTSET=0x80; } //A7
inline void clearOut10(void) { PORTA.OUTCLR=0x80; }

inline void setOut11  (void) { PORTB.OUTSET=0x01; } //B0
inline void clearOut11(void) { PORTB.OUTCLR=0x01; }
inline void setOut12  (void) { PORTB.OUTSET=0x02; } //B1
inline void clearOut12(void) { PORTB.OUTCLR=0x02; }

void setOut  (uint8_t no)
{
    switch (no)
    {
    case 1:
        setOut1();
        break;
    case 2:
        setOut2();
        break;
    case 3:
        setOut3();
        break;
    case 4:
        setOut4();
        break;
    case 5:
        setOut5();
        break;
    case 6:
        setOut6();
        break;
    case 7:
        setOut7();
        break;
    case 8:
        setOut8();
        break;
    case 9:
        setOut9();
        break;
    case 10:
        setOut10();
        break;
    case 11:
        setOut11();
        break;
    case 12:
        setOut12();
        break;
    }
}

void clearOut(uint8_t no)
{
    switch (no)
    {
    case 1:
        clearOut1();
        break;
    case 2:
        clearOut2();
        break;
    case 3:
        clearOut3();
        break;
    case 4:
        clearOut4();
        break;
    case 5:
        clearOut5();
        break;
    case 6:
        clearOut6();
        break;
    case 7:
        clearOut7();
        break;
    case 8:
        clearOut8();
        break;
    case 9:
        clearOut9();
        break;
    case 10:
        clearOut10();
        break;
    case 11:
        clearOut11();
        break;
    case 12:
        clearOut12();
        break;
    }
}

void HC12setAtMode()
{
  PORTD.OUTCLR = PIN0_bm;
}

void HC12setTransparentMode()
{
  PORTD.OUTSET = PIN0_bm;
}