#include <stdlib.h>
#include <float.h>

#include "main.h"
#include "vty.h"
#include "hardwareConfig.h"
#include "configuration.h"
#include "softwareConfig.h"
#include "hardware.h"
#include "sim900.h"

#include "cmdline.h"

#include "hc12.h"

#include "../../drv/include/twi.h"

#include "adxl345.h"
#include "bmp085.h"
#include "mhc5883l.h"
#include "rc_ibus.h"


#if LANG_EN
#include "vty_en.h"
#endif

#if LANG_PL
#include "vty_pl.h"
#endif

#ifndef LANG_VTY
#error "Vty Language not defined"
#endif

static CliExRes_t helpFunction           (CliState_t *state);
static CliExRes_t historyFunction        (CliState_t *state);
static CliExRes_t statusFunction         (CliState_t *state);
static CliExRes_t enableFunction         (CliState_t *state);
static CliExRes_t disableFunction        (CliState_t *state);
static CliExRes_t configureModeFunction  (CliState_t *state);
static CliExRes_t saveConfigFunction     (CliState_t *state);

static CliExRes_t forwardFunction        (CliState_t *state);
static CliExRes_t backwordFunction       (CliState_t *state);
static CliExRes_t rotateLeftFunction     (CliState_t *state);
static CliExRes_t rotateRightFunction    (CliState_t *state);
static CliExRes_t stopFunction           (CliState_t *state);

static CliExRes_t pwrFunction            (CliState_t *state);

static CliExRes_t hc12modeFunction       (CliState_t *state);
static CliExRes_t hc12channelFunction    (CliState_t *state);
static CliExRes_t hc12baudrateFunction   (CliState_t *state);
static CliExRes_t hc12powerFunction      (CliState_t *state);
static CliExRes_t hc12statusFunction     (CliState_t *state);

static CliExRes_t pwmSetFreq             (CliState_t *state);

static CliExRes_t hc12sendForwardFunction    (CliState_t *state);
static CliExRes_t hc12sendBackwordFunction   (CliState_t *state);
static CliExRes_t hc12sendRotateLeftFunction (CliState_t *state);
static CliExRes_t hc12sendRotateRightFunction(CliState_t *state);
static CliExRes_t hc12sendStopFunction       (CliState_t *state);

#if USE_SIM900
static CliExRes_t sim900OnFunction           (CliState_t *state);
static CliExRes_t sim900OffFunction          (CliState_t *state);
static CliExRes_t sim900atMode               (CliState_t *state);
#endif
static CliExRes_t twiWtiteAndRead            (CliState_t *state);
static CliExRes_t adxlTest                   (CliState_t *state);
static CliExRes_t bmpTest                    (CliState_t *state);
static CliExRes_t mhcTest                    (CliState_t *state);
static CliExRes_t l3gTest                    (CliState_t *state);
static CliExRes_t calibrate                  (CliState_t *state);
static CliExRes_t rcTest                     (CliState_t *state);

static CliExRes_t sendHC12(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);
static CliExRes_t sendHC12AtCmd(CliState_t *state, const char cmd[]);

static CliExRes_t sendHC12loopback(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[]);




const char okStr[] PROGMEM = "OK\r\n";
const char nlStr[] PROGMEM = "\r\n";

const char* const errorStrings[] PROGMEM = {
  errorOK,
  errorNoFile,
  errorxModemFrameStartTimeout,
  errorxModemByteSendTimeout,
  errorxModemWrongFrameNo,
  errorxModemFrameFrameNoCorrectionNotMatch,
  errorxModemFrameCrc,
  errorxModemRemoteSideCan,
  errorxModemUnknownResponse,
  errorNoRemoteDevice,
  errorBootloaderNotResponding,
  errorOpenFile
};

const Command_t cmdListNormal[] PROGMEM =
{
  {cmd_help,            cmd_help_help,            helpFunction,                0},
  {cmd_history,         cmd_help_history,         historyFunction,             0},
  {cmd_status,          cmd_help_status,          statusFunction,              0},
  {cmd_enable,          cmd_help_enable,          enableFunction,              0},
  {cmd_forward,         cmd_help_forward,         forwardFunction,             0},
  {cmd_backward,        cmd_help_backward,        backwordFunction,            0},
  {cmd_rotateLeft,      cmd_help_rotateLeft,      rotateLeftFunction,          0},
  {cmd_rotateRight,     cmd_help_rotateRight,     rotateRightFunction,         0},
  {cmd_stop,            cmd_help_stop,            stopFunction,                0},
  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction,     0},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction,    0},
  {cmd_hc12rotateLeft , cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction,  0},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction, 0},
  {cmd_hc12stop       , cmd_help_hc12stop,        hc12sendStopFunction,        0},
  {cmd_twiWriteAndRead, cmd_help_twiWriteAndStop, twiWtiteAndRead,             0},
  {cmd_adxlTest       , cmd_help_adxlTest,        adxlTest,                    0},
  {cmd_bmpTest        , cmd_help_bmpTest,         bmpTest,                     0},
  {cmd_mhcTest        , cmd_help_mhcTest,         mhcTest,                     0},
  {cmd_l3gTest        , cmd_help_l3gTest,         l3gTest,                     0},
  {cmd_calibrate      , cmd_help_calibrate,       calibrate,                   0},
  {cmd_rc             , cmd_help_rc,              rcTest,                      0},
  {cmd_pwr,             cmd_help_pwr,             pwrFunction,                 0},
  {NULL               , NULL,                     NULL,                        0}
};

const Command_t cmdListEnable[] PROGMEM =
{
  {cmd_help,            cmd_help_help,            helpFunction,               0},
  {cmd_history,         cmd_help_history,         historyFunction,            0},
  {cmd_status,          cmd_help_status,          statusFunction,             0},
  {cmd_disable,         cmd_help_disable,         disableFunction,            0},
  {cmd_configure,       cmd_help_configure,       configureModeFunction,      0},
  {cmd_forward,         cmd_help_forward,         forwardFunction,            0},
  {cmd_backward,        cmd_help_backward,        backwordFunction,           0},
  {cmd_rotateLeft,      cmd_help_rotateLeft,      rotateLeftFunction,         0},
  {cmd_rotateRight,     cmd_help_rotateRight,     rotateRightFunction,        0},
  {cmd_stop,            cmd_help_stop,            stopFunction,               0},
  {cmd_pwr,             cmd_help_pwr,             pwrFunction,                0},
  {cmd_HC12status,      cmd_help_HC12status,      hc12statusFunction,         0},
  {cmd_hc12forward,     cmd_help_hc12forward,     hc12sendForwardFunction,    0},
  {cmd_hc12backward,    cmd_help_hc12backward,    hc12sendBackwordFunction,   0},
  {cmd_hc12rotateLeft,  cmd_help_hc12rotateLeft,  hc12sendRotateLeftFunction, 0},
  {cmd_hc12rotateRight, cmd_help_hc12rotateRight, hc12sendRotateRightFunction,0},
  {cmd_hc12stop,        cmd_help_hc12stop,        hc12sendStopFunction,       0},
#if USE_SIM900
  {cmd_sim900on,        cmd_help_sim900on,        sim900OnFunction,           0},
  {cmd_sim900off,       cmd_help_sim900off,       sim900OffFunction,          0},
  {cmd_sim900at,        cmd_help_sim900at,        sim900atMode,               0},
#endif
  {NULL, NULL, NULL, 0}
};


const Command_t cmdListConfigure[] PROGMEM =
{
  {cmd_help,            cmd_help_help,         helpFunction,         0},
  {cmd_history,         cmd_help_history,      historyFunction,      0},
  {cmd_status,          cmd_help_status,       statusFunction,       0},
  {cmd_HC12mode,        cmd_help_HC12mode,     hc12modeFunction,     0},
  {cmd_HC12channel,     cmd_help_HC12channel,  hc12channelFunction,  0},
  {cmd_HC12baudrate,    cmd_help_HC12baudrate, hc12baudrateFunction, 0},
  {cmd_HC12power,       cmd_help_HC12power,    hc12powerFunction,    0},
  {cmd_HC12status,      cmd_help_HC12status,   hc12statusFunction,   0},
  {cmd_conf_save,       cmd_help_conf_save,    saveConfigFunction,   0},
  {cmd_enable,          cmd_help_enable,       enableFunction,       0},
  {cmd_disable,         cmd_help_disable,      disableFunction,      0},
  {cmd_pwm_freq,        cmd_help_pwm_freq,     pwmSetFreq,           0},
  {NULL,                NULL,                  NULL,                 0}
};

void VtyInit(CliState_t* state, FILE *stream)
{
  cmdStateConfigure(state, stream, &cmdListNormal[0], NR_NORMAL);
}


void printErrorInfo(CliState_t *state)
{
  if (state->error.errno != 0)
  {
    fprintf(state->myStdInOut, (const char *)(errorStrings + state->error.errno), state->error.err1, state->error.err2);
  }
  state->error.errno = 0;
  state->error.err1 = 0;
  state->error.err2 = 0;
}

static CliExRes_t enableFunction(CliState_t *state)
{
    CliExRes_t result = ERROR_OPERATION_NOT_ALLOWED;
    if (state->internalData.cliMode != RESTRICTED_NORMAL)
    {
        state->internalData.cmdList = cmdListEnable;
        state->internalData.cliMode = NR_ENABLE;
        result = OK_SILENT;
    }
    return result;
}

static CliExRes_t disableFunction(CliState_t *state)
{
    state->internalData.cmdList = cmdListNormal;
    if (state->internalData.cliMode != RESTRICTED_NORMAL)
    {
        state->internalData.cliMode = NR_NORMAL;
    }
    return OK_SILENT;
}

// ************************** VTY API ***************************************************************************************
void printStatus(FILE *stream)
{
  uint16_t tmp;
#ifdef USE_XC8
  fprintf(stream, SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n");
  fprintf(stream, "PWR status: 4v3 %s, RPI 3v3 %s, RPI 4v3 %s\r\n", isPwr4v3() ? "On": "Off", isPwr3v3rpi() ? "On": "Off", isPwr4v3rpi() ? "On": "Off");
  
  fprintf(stream, "Hc12 config:\r\n");
  fprintf(stream, "\tmode    %d\r\n", confHC12mode);
  fprintf(stream, "\tbaud    %d\r\n", confHC12baud);
  fprintf(stream, "\tchannel %d\r\n", confHC12channel);
  fprintf(stream, "\tpower   %d\r\n", confHC12power);    
#else
  fprintf_P(stream, PSTR(SYSTEM_NAME" ver "S_VERSION" build: "__DATE__", "__TIME__"\r\n"));
  fprintf_P(stream, PSTR("PWR status: 4v3 %s, RPI 3v3 %s, RPI 4v3 %s\r\n"), isPwr4v3() ? "On": "Off", isPwr3v3rpi() ? "On": "Off", isPwr4v3rpi() ? "On": "Off");

  fprintf_P(stream, PSTR("Hc12 config:\r\n"));
  fprintf_P(stream, PSTR("\tmode    %d\r\n"), confHC12mode);
  fprintf_P(stream, PSTR("\tbaud    %d\r\n"), confHC12baud);
  fprintf_P(stream, PSTR("\tchannel %d\r\n"), confHC12channel);
  fprintf_P(stream, PSTR("\tpower   %d\r\n"), confHC12power);
#endif

//  uint16_t res = ADCA.CH0RES;
  ADCA.CH0.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN8_gc;                  //PB0

  ADCA.CH1.CTRL    = ADC_CH_INPUTMODE_SINGLEENDED_gc;        //Pojedyncze wejście
  ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN9_gc;                  //PB0


  ADCA.CTRLA     = ADC_ENABLE_bm | ADC_CH0START_bm | ADC_CH1START_bm;          //Włączenie przetwornika AC oraz uruchomienie pomiaru na kanale 0

  while (ADCA.CH0.INTFLAGS==0);
  uint16_t res = ADCA.CH0RES;
  ADCA.CH0.INTFLAGS=1;

  while (ADCA.CH1.INTFLAGS==0);
  uint16_t res2 = ADCA.CH1RES;
  ADCA.CH1.INTFLAGS=1;


  ADCA.CTRLA = ADC_ENABLE_bm;

#ifdef USE_XC8
  fprintf(stream, "Pwr: %d + %d/128 V\r\n", res>>7, res&0x7F);
  fprintf(stream, "     %d + %d/32 A\r\n",  res2>>5, res2&0x1F);
#else
  fprintf_P(stream, PSTR("Pwr: %d + %d/128 V\r\n"), res>>7, res&0x7F);
  fprintf_P(stream, PSTR("     %d + %d/32 A\r\n"),  res2>>5, res2&0x1F);  
#endif
  //Print system state
  
  
#ifdef USE_XC8
  fprintf(stream, "Tasks:\r\n");
#else
  fprintf_P(stream, PSTR("Tasks:\r\n"));
#endif
  
  tmp = xPortGetFreeHeapSize();
  tmp-=2;
  char *buffer = pvPortMalloc(1024);
  if (buffer != NULL)
  {
    vTaskList(tmp, buffer);
    fprintf(stream, buffer);
    vPortFree(buffer);  
  }
  
#ifdef USE_XC8
  fprintf(stream, "Heap free space: %d\r\n", xPortGetFreeHeapSize());
#else
  fprintf_P(stream, PSTR("Heap free space: %d\r\n"), xPortGetFreeHeapSize());
#endif
  
}


// ************************** CLI Functions *********************************************************************************

static CliExRes_t statusFunction(CliState_t *state)
{
    printStatus(state->myStdInOut);
    return OK_SILENT;
}

static CliExRes_t historyFunction(CliState_t *state)
{
    cmdPrintHistory(state);
    return OK_SILENT;
}

static CliExRes_t helpFunction(CliState_t *state)
{
    cmdPrintHelp(state);
    return OK_SILENT;
}

static CliExRes_t saveConfigFunction(CliState_t *state)
{
    (void) state;
    saveConfiguration();
    return OK_SILENT;
}

static CliExRes_t configureModeFunction(CliState_t *state)
{
    CliExRes_t result = ERROR_OPERATION_NOT_ALLOWED;
    if (state->internalData.cliMode == NR_ENABLE)
    {
        state->internalData.cmdList = cmdListConfigure;
        state->internalData.cliMode = NR_CONFIGURE;
        result = OK_SILENT;
    }
    return result;
}


static CliExRes_t forwardFunction        (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);

    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, FORWARD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t backwordFunction       (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, BACKWORD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}
static CliExRes_t rotateLeftFunction     (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, ROTATE_LEFT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t rotateRightFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12loopback(state, 0, ROTATE_RIGHT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t stopFunction           (CliState_t *state)
{
    uint8_t dta[1];
    dta[0] = 0;
    sendHC12loopback(state, 0, STOP, 1, dta);

    return OK_SILENT;
}


static CliExRes_t pwrFunction           (CliState_t *state)
{
    if (state->argc <2)
        return ERROR_INFORM;

    uint8_t devNo = atoi(state->argv[1]);
    uint8_t devState = atoi(state->argv[2]);

    if (devState == 0)
    {
        switch (devNo)
        {
            case 1: pwrOff4v3();     break;
            case 2: pwrOff3v3rpi();  break;
            case 3: pwrOff4v3rpi();  break;
            default:                 break;
        }
    }
    else
    {
        switch (devNo)
        {
            case 1: pwrOn4v3();      break;
            case 2: pwrOn3v3rpi();   break;
            case 3: pwrOn4v3rpi();   break;
            default:                 break;
        }
    }
    return OK_SILENT;
}

static CliExRes_t hc12sendForwardFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
          dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, FORWARD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);
    return OK_SILENT;
}

static CliExRes_t hc12sendBackwordFunction   (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, BACKWORD, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);
    return OK_SILENT;
}


static CliExRes_t hc12sendRotateLeftFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
      dta.pwmLeft = atoi(state->argv[1]);
      dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, ROTATE_LEFT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t hc12sendRotateRightFunction    (CliState_t *state)
{
    TlvMsgMoveDta_t dta;
    dta.duration = 0;
    dta.pwmLeft  = 50;
    dta.pwmRight = 50;

    if (state->argc == 1)
        dta.pwmLeft = dta.pwmRight = atoi(state->argv[1]);
    if (state->argc >=2)
    {
        dta.pwmLeft = atoi(state->argv[1]);
        dta.pwmRight = atoi(state->argv[2]);
    }
    sendHC12(state, 0, ROTATE_RIGHT, sizeof(TlvMsgMoveDta_t), (uint8_t *) &dta);

    return OK_SILENT;
}

static CliExRes_t hc12sendStopFunction       (CliState_t *state)
{
  sendHC12(state, 0, STOP, 0, NULL);
  return OK_SILENT;
}

static CliExRes_t sendHC12AtCmd(CliState_t *state, const char cmd[])
{
    if (xSemaphoreTake(Hc12semaphore, 10) == pdTRUE)
    {
        vTaskDelay(2);
        HC12setAtMode();
        vTaskDelay(25);
        fprintf(state->myStdInOut, cmd, state->argv[1]);
        fprintf(&hc12Stream,       cmd, state->argv[1]);

        uint8_t val;
        while (xQueueReceive(xHC12Rec, &val, 100) == pdTRUE)
        {
            fputc(val, state->myStdInOut);
        }

        HC12setTransparentMode();
        xSemaphoreGive(Hc12semaphore );
        return OK_SILENT;
    }
    else
    {
        return ERROR_INFORM;
    }
}

static CliExRes_t sendHC12(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  TlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12Stream);

  return OK_INFORM;
}

static CliExRes_t sendHC12loopback(CliState_t *state, uint8_t addr, uint8_t type, uint8_t len, const uint8_t const cmdDta[])
{
  (void) state;
  TlvMsg_t msg;
  msg.sync = TLV_SYNC;
  msg.address = addr;
  msg.type = type;
  msg.dtaLen = len;

  tlvCalculateCrcSepDta(&msg, cmdDta);

  sendTlvMsgDta(&msg, cmdDta, &hc12FakeStream);

  return OK_INFORM;
}


static CliExRes_t bmpTest(CliState_t *state)
{
    float temperature = bmp085_readTemperature(&hardwarePAL.bmp);
    uint16_t tmp = bmp085_readRawTemperature(&hardwarePAL.bmp);
    CMD_printf("Temperature: %f (%u) \r\n", temperature, tmp);

    uint32_t preassureRaw = bmp085_readRawPressure(&hardwarePAL.bmp);
    int32_t preassure = bmp085_readPressure(&hardwarePAL.bmp);
    CMD_printf("Preassure: %ld (%lu)\r\n", preassure, preassureRaw);

    return OK_SILENT;    
}

static CliExRes_t mhcTest(CliState_t *state)
{
    mhc5883l_readRaw(&hardwarePAL.mhc);
    CMD_printf("Raw data: %u %u %u\r\n", hardwarePAL.mhc.v.XAxis, hardwarePAL.mhc.v.YAxis, hardwarePAL.mhc.v.ZAxis);

    
    return OK_SILENT;    
}

static CliExRes_t l3gTest(CliState_t *state)
{
    uint8_t i;
    //l3g_calibrate(&HardwarePAL.l3g, 20);
    CMD_msg("Gyro test\r\n");
    for (i=0; i<20; i++)
    {
        vTaskDelay(25);
        l3g_readNormalize(&hardwarePAL.l3g);    
        CMD_printf("%2d/20: %12f (%6d) %12f ", i, hardwarePAL.l3g.n.XAxis, hardwarePAL.l3g.r.XAxis, hardwarePAL.l3g.n.YAxis);
        CMD_printf("(%6d) %10f (%6d)\r\n", hardwarePAL.l3g.r.YAxis, hardwarePAL.l3g.n.ZAxis, hardwarePAL.l3g.r.ZAxis);
    }
    
    return OK_SILENT; 
}

static CliExRes_t rcTest(CliState_t *state)
{
    uint8_t i;
    uint8_t j;
    uint16_t tmp1 = rc_getNoOfNewFrames();
    uint16_t tmp2 = rc_getNoOfProcFrames();
    uint16_t tmp3 = rc_getNoOfCrcErrors();
    CMD_printf("Rc demo: no of new Frames: %u, ", tmp1);
    CMD_printf("(%u + ", tmp2);
    CMD_printf("%u)\r\n", tmp3);

    pwrOn4v3rpi();
    
    //return OK_INFORM; 
        
        
    for (i=0; i< 20; i++)
    {
        for (j=0; j< 4; j++)
        {
            tmp1 = rc_getChan(j);
            CMD_printf("%u ", tmp1);
            //vTaskDelay(2);
        }
        CMD_msg("\r\n");
        vTaskDelay(50);
    }
    return OK_INFORM; 
}

static CliExRes_t calibrate(CliState_t *state)
{
    CMD_msg("Calibrating GYRO\r\n");
    l3g_calibrate(&hardwarePAL.l3g, 20);
    return OK_INFORM; 
}


static CliExRes_t adxlTest(CliState_t *state)
{

    
//  adxl345_s
    
    
    struct VectorUint16 tmpVect;
    adxl345_readRaw(&hardwarePAL.adxl, &tmpVect);
    CMD_printf("RAW result: %d %d %d\r\n", tmpVect.XAxis, tmpVect.YAxis, tmpVect.ZAxis);

    struct VectorFloat tmpVect2;
    adxl345_readNormalize(&hardwarePAL.adxl, &tmpVect2, ADXL345_GRAVITY_EARTH);

    return OK_SILENT;
}

static CliExRes_t twiWtiteAndRead(CliState_t *state)
{
    uint8_t result = 1;
    uint8_t tmpDta[8];
    
    uint8_t address = atoi(state->argv[1]);
    uint8_t rdDtaLen = atoi(state->argv[2]);
    uint8_t wrDtaLen = hexStrToDataN(tmpDta, state->argv[3], 8);



    fprintf(state->myStdInOut, "Sending %d bytes\r\n", wrDtaLen);
    result = TwiMaster_ReadAndWrite(&hardwarePAL.twiSensors, address, wrDtaLen, tmpDta, rdDtaLen, NULL);    
    
    fprintf(state->myStdInOut, "Result: 0x%02x", result);
        
    if (result & TWI_REZ_ARBITRATION_LOST)        
        fprintf(state->myStdInOut, " ARBITRATION_LOST");
        
    if (result & TWI_REZ_BUS_ERROR)        
        fprintf(state->myStdInOut, " BUS_ERROR");
        
    if (result & TWI_REX_NACK_RECEIVED)        
        fprintf(state->myStdInOut, " NACK_RECEIVED");
        
    if (result & TWI_REZ_FAIL)        
        fprintf(state->myStdInOut, " FAIL");

    if (result & TWI_REZ_MUTEX_TIMEOUT_STAGE1)        
        fprintf(state->myStdInOut, " MUTEX_TIMEOUT_STAGE1");

    if (result & TWI_REZ_MUTEX_TIMEOUT_STAGE2)        
        fprintf(state->myStdInOut, " MUTEX_TIMEOUT_STAGE2");        
        
    fprintf(state->myStdInOut, "\r\n");
    fprintf(state->myStdInOut, "Received: %d / %d bytes\r\n", hardwarePAL.twiSensors.bytesRead, hardwarePAL.twiSensors.bytesToRead);
    fprintf(state->myStdInOut, "Transmitted: %d / %d bytes\r\n", hardwarePAL.twiSensors.bytesWritten, hardwarePAL.twiSensors.bytesToWrite);
    
    if (result == TWI_REZ_OK)
    {
        vTaskDelay(100);
        for(result = 0; result < rdDtaLen; result++)
        {
            fprintf(state->myStdInOut, "0x%02x ", hardwarePAL.twiSensors.readData[result]);
        }
        fprintf(state->myStdInOut, "\n");
      
        return OK_INFORM;    
    }
    return ERROR_INFORM;
}


static CliExRes_t pwmSetFreq (CliState_t *state)
{
    uint8_t preskaler = atoi(state->argv[1]);
    if (preskaler > 7 || preskaler < 4)
        return ERROR_INFORM;

    TCC0.CTRLA &= 0xF0;
    TCC0.CTRLA |= preskaler;

    return OK_INFORM;
}

static CliExRes_t hc12modeFunction       (CliState_t *state)
{
    confHC12mode = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+FU%s\r\n"));
}

static CliExRes_t hc12channelFunction    (CliState_t *state)
{
    confHC12channel = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+C%s\r\n"));
}

static CliExRes_t hc12baudrateFunction   (CliState_t *state)
{
    confHC12baud = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+B%s\r\n"));
}

static CliExRes_t hc12powerFunction      (CliState_t *state)
{
    confHC12power = atoi(state->argv[1]);
    return sendHC12AtCmd(state, PSTR("AT+P%s\r\n"));
}

static CliExRes_t hc12statusFunction     (CliState_t *state)
{
    return sendHC12AtCmd(state, PSTR("AT+RX\r\n"));
}

#if USE_SIM900
static CliExRes_t sim900OnFunction           (CliState_t *state)
{
    (void) state;
    sim900pwrOn();
    return OK_INFORM;
}

static CliExRes_t sim900OffFunction          (CliState_t *state)
{
    (void) state;
    sim900pwrOffHw();
    return OK_INFORM;
}

static CliExRes_t sim900atMode               (CliState_t *state)
{
    uint8_t znak;
#ifdef USE_XC8
    fprintf(state->myStdInOut, "Press ^z to exit at mode\r\n");
#else
    fprintf_P(state->myStdInOut, PSTR("Press ^z to exit at mode\r\n"));
#endif
    for ( ; ;)
    {
        if (xQueueReceive(xVtyRec, &znak, 0) == pdTRUE)
        {
            if (znak == 0x1A)
                break;
            xQueueSend(xSIM900Tx, &znak, 0);
        }
        if (xQueueReceive(xSIM900Rec, &znak, 0) == pdTRUE)
            fputc(znak, state->myStdInOut);
    }
    return OK_INFORM;
}
#endif

/*
static void printTable(FILE *stream)
{
  fprintf_P(stream, PSTR("\r\nAddress Table:\r\n\r\n"));  //Print system state

  uint16_t i;
  for(i=0; i<256; i++)
  {
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\t"), i, translateTable[i]);
    i++ ;
    fprintf_P(stream, PSTR("%3d -> %3d\r\n"), i, translateTable[i]);
  }
  fprintf_P(stream, PSTR("\r\n"));
}
*/
