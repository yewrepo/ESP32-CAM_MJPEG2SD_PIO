// Global MJPEG2SD declarations
//
// s60sc 2021, 2022

#pragma once
#include "globals.h"

#if defined(CONFIG_IDF_TARGET_ESP32)
// default pin configuration for ESP32 cam boards
#define CAMERA_MODEL_AI_THINKER // Has PSRAM  
#elif defined(CONFIG_IDF_TARGET_ESP32S3)
// default pin configuration below for Freenove ESP32S3 cam boards
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
#endif

/**************************************************************************
 Uncomment one only of the camera models below if not using a default above
 and comment out above define block
 Selecting wrong model may crash your device due to pin conflict
***************************************************************************/

//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_TTGO_T_CAMERA_PLUS

/**************************************************************************/

#define USE_DS18B20 false  // if true, requires additional libraries: OneWire and DallasTemperature

#define ALLOW_SPACES false // set true to allow whitespace in configs.txt key values

// web server ports
#define WEB_PORT 80 // app control
#define OTA_PORT (WEB_PORT + 1) // OTA update
#define STREAM_PORT (WEB_PORT + 2) // camera images

/*********************** Fixed defines leave as is ***********************/ 
/** Do not change anything below here unless you know what you are doing **/

//#define DEV_ONLY // leave commented out
#ifdef DEV_ONLY 
//#define SIDE_ALARM // uncomment if used for side alarm
#endif 
#define STATIC_IP_OCTAL "133" // dev only
#define CHECK_MEM false // leave as false
#define FLUSH_DELAY 200 // for debugging crashes
 
#define APP_NAME "ESP-CAM_MJPEG" // max 15 chars
#define APP_VER "8.8"

#define MAX_CLIENTS 2 // allowing too many concurrent web clients can cause errors
#define INDEX_PAGE_PATH DATA_DIR "/MJPEG2SD" HTML_EXT
#define FILE_NAME_LEN 64
#define JSON_BUFF_LEN (32 * 1024) // set big enough to hold all file names in a folder
#define MAX_CONFIGS 130 // > number of entries in configs.txt

#ifdef SIDE_ALARM
#define STORAGE LittleFS
#define GITHUB_URL ""
#else
#define STORAGE SD_MMC
#define GITHUB_URL "https://raw.githubusercontent.com/s60sc/ESP32-CAM_MJPEG2SD/master"
#endif
#define RAMSIZE (1024 * 8) // set this to multiple of SD card sector size (512 or 1024 bytes)
#define CHUNKSIZE (1024 * 4)
#define RAM_LOG_LEN 5000 // size of ram stored system message log in bytes
#define INCLUDE_FTP 
#define INCLUDE_SMTP
#define INCLUDE_MQTT
#define ISCAM // cam specific code in generics
// set true for emailing external ip changes
#define IP_EMAIL false

#define IS_IO_EXTENDER false // must be false except for IO_Extender
#define EXTPIN 100

// to determine if newer data files need to be loaded
#define CFG_VER 2
#define HTM_VER 4
#define JS_VER 1

#define AVI_EXT "avi"
#define CSV_EXT "csv"
#define AVI_HEADER_LEN 310 // AVI header length
#define CHUNK_HDR 8 // bytes per jpeg hdr in AVI 
#define WAVTEMP "/current.wav"
#define AVITEMP "/current.avi"
#define TLTEMP "/current.tl"
#define TELETEMP "/current.csv"

// non default pins configured for SD card on given camera board
#if defined(CAMERA_MODEL_ESP32S3_EYE)
#define SD_MMC_CLK 39 
#define SD_MMC_CMD 38
#define SD_MMC_D0 40
#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
#define SD_MMC_CLK 7 
#define SD_MMC_CMD 9
#define SD_MMC_D0 8
#elif defined(CAMERA_MODEL_TTGO_T_CAMERA_PLUS)
#define SD_MMC_CLK 21 // SCLK
#define SD_MMC_CMD 19 // MOSI
#define SD_MMC_D0 22  // MISO
#endif


/******************** Libraries *******************/

#include "esp_camera.h"
#include "camera_pins.h"

/******************** Function declarations *******************/

struct mjpegStruct {
  size_t buffLen;
  size_t buffOffset;
  size_t jpegSize;
};

struct fnameStruct {
  uint8_t recFPS;
  uint32_t recDuration;
  uint16_t frameCnt;
};


// global app specific functions

void buildAviHdr(uint8_t FPS, uint8_t frameType, uint16_t frameCnt, bool isTL = false);
void buildAviIdx(size_t dataSize, bool isVid = true, bool isTL = false);
bool checkMotion(camera_fb_t* fb, bool motionStatus);
bool checkSDFiles();
void doIOExtPing();
esp_err_t extractQueryKey(httpd_req_t *req, char* variable);
bool fetchMoveMap(uint8_t **out, size_t *out_len);
void finalizeAviIndex(uint16_t frameCnt, bool isTL = false);
void finishAudio(bool isValid);
mjpegStruct getNextFrame(bool firstCall = false);
bool getPIRval();
bool haveWavFile(bool isTL = false);
bool isNight(uint8_t nightSwitch);
void openSDfile(const char* streamFile);
void prepAviIndex(bool isTL = false);
bool prepRecording();
void prepTelemetry();
void prepMic();
void setCamPan(int panVal);
void setCamTilt(int tiltVal);
uint8_t setFPS(uint8_t val);
uint8_t setFPSlookup(uint8_t val);
void setLamp(uint8_t lampVal);
void startAudio();
void startStreamServer();
void startTelemetry();
void stopPlaying();
void stopTelemetry(const char* fileName);
size_t writeAviIndex(byte* clientBuf, size_t buffSize, bool isTL = false);
size_t writeWavFile(byte* clientBuf, size_t buffSize);


/******************** Global app declarations *******************/

// motion detection parameters
extern int moveStartChecks; // checks per second for start motion
extern int moveStopSecs; // secs between each check for stop, also determines post motion time
extern int maxFrames; // maximum number of frames in video before auto close 

// motion recording parameters
extern int detectMotionFrames; // min sequence of changed frames to confirm motion 
extern int detectNightFrames; // frames of sequential darkness to avoid spurious day / night switching
extern int detectNumBands;
extern int detectStartBand;
extern int detectEndBand; // inclusive
extern int detectChangeThreshold; // min difference in pixel comparison to indicate a change

// record timelapse avi independently of motion capture, file name has same format as avi except ends with T
extern int tlSecsBetweenFrames; // too short interval will interfere with other activities
extern int tlDurationMins; // a new file starts when previous ends
extern int tlPlaybackFPS;  // rate to playback the timelapse, min 1 

// status & control fields 
extern bool autoUpload;
extern bool dbgMotion;
extern bool doPlayback;
extern bool doRecording; // whether to capture to SD or not
extern bool forceRecord; // Recording enabled by rec button
extern bool forcePlayback; // playback enabled by user
extern uint8_t FPS;
extern uint8_t fsizePtr; // index to frameData[] for record
extern bool isCapturing;
extern uint8_t lightLevel;  
extern uint8_t lampLevel;  
extern int micGain;
extern uint8_t minSeconds; // default min video length (includes moveStopSecs time)
extern float motionVal;  // motion sensitivity setting - min percentage of changed pixels that constitute a movement
extern uint8_t nightSwitch; // initial white level % for night/day switching
extern bool nightTime; 
extern bool stopPlayback;
extern bool isStreaming;
extern bool useMotion; // whether to use camera for motion detection (with motionDetect.cpp)  
extern bool timeLapseOn; // enable time lapse recording
extern int maxFrames;
extern char inFileName[];
extern uint8_t xclkMhz;
extern char camModel[];

// buffers
extern uint8_t iSDbuffer[];
extern uint8_t aviHeader[];
extern const uint8_t dcBuf[]; // 00dc
extern const uint8_t wbBuf[]; // 01wb
extern byte* uartData;

// peripherals

// IO Extender use
extern bool useIOextender; // true to use IO Extender, otherwise false
extern bool useUART0;
extern int uartTxdPin;
extern int uartRxdPin;
// peripherals used
extern bool pirUse; // true to use PIR for motion detection
extern bool lampUse; // true to use lamp
extern bool lampAuto; // if true in conjunction with usePir & useLamp, switch on lamp when PIR activated
extern bool lampNight;
extern int lampType;
extern bool servoUse; // true to use pan / tilt servo control
extern bool voltUse; // true to report on ADC pin eg for for battery
// microphone cannot be used on IO Extender
extern bool micUse; // true to use external I2S microphone 
extern bool wakeUse;

// sensors 
extern int pirPin; // if usePir is true
extern bool pirVal;
extern int lampPin; // if useLamp is true
extern int wakePin; // if wakeUse is true
extern bool teleUse;
extern int teleInterval;

// Pan / Tilt Servos 
extern int servoPanPin; // if useServos is true
extern int servoTiltPin;
// ambient / module temperature reading 
extern int ds18b20Pin; // if USE_DS18B20 true
// batt monitoring 
extern int voltPin; 

// microphone recording
extern int micSckPin; // I2S SCK 
extern int micSWsPin;  // I2S WS / PDM CLK
extern int micSdPin;  // I2S SD / PDM DAT

// configure for specific servo model, eg for SG90
extern int servoDelay;
extern int servoMinAngle; // degrees
extern int servoMaxAngle;
extern int servoMinPulseWidth; // usecs
extern int servoMaxPulseWidth;

// battery monitor
extern int voltDivider;
extern float voltLow;
extern int voltInterval;

// audio
extern const uint32_t SAMPLE_RATE; // audio sample rate
extern const uint32_t WAV_HEADER_LEN;

// task handling
extern TaskHandle_t playbackHandle;
extern TaskHandle_t DS18B20handle;
extern TaskHandle_t telemetryHandle;
extern TaskHandle_t servoHandle;
extern TaskHandle_t uartClientHandle;
extern TaskHandle_t emailHandle;
extern TaskHandle_t ftpHandle;
extern SemaphoreHandle_t frameMutex;
extern SemaphoreHandle_t motionMutex;


/************************** structures ********************************/

struct frameStruct {
  const char* frameSizeStr;
  const uint16_t frameWidth;
  const uint16_t frameHeight;
  const uint16_t defaultFPS;
  const uint8_t scaleFactor; // (0..4)
  const uint8_t sampleRate; // (1..N)
};

// indexed by frame size - needs to be consistent with sensor.h framesize_t enum
const frameStruct frameData[] = {
  {"96X96", 96, 96, 30, 1, 1},   // 2MP sensors
  {"QQVGA", 160, 120, 30, 1, 1},
  {"QCIF", 176, 144, 30, 1, 1}, 
  {"HQVGA", 240, 176, 30, 2, 1}, 
  {"240X240", 240, 240, 30, 2, 1}, 
  {"QVGA", 320, 240, 30, 2, 1}, 
  {"CIF", 400, 296, 30, 2, 1},  
  {"HVGA", 480, 320, 30, 2, 1}, 
  {"VGA", 640, 480, 20, 3, 1}, 
  {"SVGA", 800, 600, 20, 3, 1}, 
  {"XGA", 1024, 768, 5, 3, 1},   
  {"HD", 1280, 720, 5, 3, 1}, 
  {"SXGA", 1280, 1024, 5, 3, 1}, 
  {"UXGA", 1600, 1200, 5, 3, 1},  
  {"FHD", 920, 1080, 5, 3, 1},    // 3MP Sensors
  {"P_HD", 720, 1280, 5, 3, 1},
  {"P_3MP", 864, 1536, 5, 3, 1},
  {"QXGA", 2048, 1536, 5, 4, 1},
  {"QHD", 2560, 1440, 5, 4, 1},   // 5MP Sensors
  {"WQXGA", 2560, 1600, 5, 4, 1},
  {"P_FHD", 1080, 1920, 5, 3, 1},
  {"QSXGA", 2560, 1920, 4, 4, 1}
};
