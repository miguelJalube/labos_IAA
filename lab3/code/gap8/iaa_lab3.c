#include "pmsis.h"
#include "cpx.h"
#include "wifi.h"
#include "bsp/bsp.h"
#include "bsp/camera/himax.h"
#include "bsp/buffer.h"

#define IMG_ORIENTATION 0x0101
#define CAM_WIDTH 324
#define CAM_HEIGHT 244
#define CROP_WIDTH 200
#define CROP_HEIGHT 200
#define BUFFER_SIZE 40
#define N_CHARS 1020
#define CAPTURE_DONE_BIT (1 << 0)

static EventGroupHandle_t evGroup;

void sendToSTM32(void);
void rx_wifi_task(void *parameters);
void send_image_via_wifi(unsigned char *image, uint16_t width, uint16_t height);
int open_pi_camera_himax(struct pi_device *device);
static void capture_done_cb(void *arg);
void camera_task(void *parameters);

static int wifiClientConnected = 0;
static struct pi_device camera;
unsigned char *imgBuff;
static pi_buffer_t buffer;
static SemaphoreHandle_t capture_sem = NULL;

static CPXPacket_t rxp;
static int wifiConnected = 0;

typedef struct
{
  uint8_t magic;
  uint16_t width;
  uint16_t height;
  uint8_t depth;
  uint8_t type;
  uint32_t size;
} __attribute__((packed)) img_header_t;

typedef enum
{
  RAW_ENCODING = 0,
  JPEG_ENCODING = 1
} __attribute__((packed)) StreamerMode_t;

pi_buffer_t header;
uint32_t headerSize;
pi_buffer_t footer;
uint32_t footerSize;
pi_buffer_t jpeg_data;
uint32_t jpegSize;

static StreamerMode_t streamerMode = JPEG_ENCODING;

static CPXPacket_t txp;

void createImageHeaderPacket(CPXPacket_t * packet, uint32_t imgSize, StreamerMode_t imgType) {
  img_header_t *imgHeader = (img_header_t *) packet->data;
  imgHeader->magic = 0xBC;
  imgHeader->width = CAM_WIDTH;
  imgHeader->height = CAM_HEIGHT;
  imgHeader->depth = 1;
  imgHeader->type = imgType;
  imgHeader->size = imgSize;
  packet->dataLength = sizeof(img_header_t);
}

void start(void) {

    cpxInit();
    cpxEnableFunction(CPX_F_WIFI_CTRL);
    
    BaseType_t xTask;

    vTaskDelay(2000);
    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3]\n\n *** IAA LAB03 ! ***\n\n");
    

    /* Create wifi listener task */
    xTask = xTaskCreate(rx_wifi_task, "rx_wifi_task", configMINIMAL_STACK_SIZE * 2,
                      NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Open and init camera buffer */
    if (open_pi_camera_himax(&camera)) {
        cpxPrintToConsole(LOG_TO_CRTP, "Failed to open camera\n");
        return;
    }

    pi_buffer_init(&buffer, PI_BUFFER_TYPE_L2, imgBuff);
    pi_buffer_set_format(&buffer, CAM_WIDTH, CAM_HEIGHT, 1, PI_BUFFER_FORMAT_GRAY);
    
    imgBuff = (unsigned char *)pmsis_l2_malloc(CAM_WIDTH*CAM_HEIGHT); 

    uint32_t freq = pi_freq_get(PI_FREQ_DOMAIN_FC);
    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Frequence = %d [Hz]\n", freq);
    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Size of packet = %d [B]\n", sizeof(CPXPacket_t));

    /* Create Camera task */
    xTask = xTaskCreate(camera_task, "camera_task", configMINIMAL_STACK_SIZE * 4,
                      NULL, tskIDLE_PRIORITY + 1, NULL);
    
    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] camera_task done!\n"); 

    while(1) {
        sendToSTM32();
        pi_yield();
        vTaskDelay(100);
    }
}

/**
 * @brief transfer UART to stm32
 * - Retrieves FC frequencies
 * - Sends a buffer from gap8 to STM32
 * - Need a program on stm32 to read uart
 */

/**
 * typedef struct {
    uint16_t wireLength;
    CPXRoutingPacked_t route;
    uint8_t data[MTU - CPX_HEADER_SIZE];
} __attribute__((packed)) CPXPacketPacked_t;
*/

void sendToSTM32(void) {
    char buff[BUFFER_SIZE];
    int freq = pi_freq_get(PI_FREQ_DOMAIN_FC);
    sprintf(buff, "%d", freq);
    cpxPrintToConsole(LOG_TO_CRTP, "FC : %d\n", freq);
    cpxInit();
    CPXPacket_t packet;
    cpxInitRoute(CPX_T_GAP8, CPX_T_STM32, CPX_F_APP, &packet.route);
    packet.data[0] = buff;
    packet.dataLength = BUFFER_SIZE;
    cpxSendPacketBlocking(&packet);
}



/**
 * @brief Task wifi management
 * be able to:
 * - know if a PC is connected to the drone
 */
void rx_wifi_task(void *parameters) {
    
    /* TODO */
    while (1)
    {
        cpxReceivePacketBlocking(CPX_F_WIFI_CTRL, &rxp);

        WiFiCTRLPacket_t * wifiCtrl = (WiFiCTRLPacket_t*) rxp.data;

        switch (wifiCtrl->cmd)
        {
        case WIFI_CTRL_STATUS_WIFI_CONNECTED:
            cpxPrintToConsole(LOG_TO_CRTP, "Wifi connected (%u.%u.%u.%u)\n",
                            wifiCtrl->data[0], wifiCtrl->data[1],
                            wifiCtrl->data[2], wifiCtrl->data[3]);
            wifiConnected = 1;
            break;
        case WIFI_CTRL_STATUS_CLIENT_CONNECTED:
            cpxPrintToConsole(LOG_TO_CRTP, "Wifi client connection status: %u\n", wifiCtrl->data[0]);
            wifiClientConnected = wifiCtrl->data[0];
            break;
        default:
            break;
        }
    }
}

/**
 * @brief transfer WIFI gap8 to PC
 * Send a gap8 buffer to the PC
 * Need python code to receives data on PC
 */
void send_image_via_wifi(unsigned char *image, uint16_t width, uint16_t height) {
    /* TODO */
    if(wifiClientConnected){
        CPXPacket_t packet;
        img_header_t img = {.width = width, .height = height};
        memcpy(&packet.data, &img, sizeof(img_header_t));
        packet.dataLength = sizeof(img_header_t);
        cpxInitRoute(CPX_T_GAP8, CPX_T_WIFI_HOST, CPX_F_APP, &txp.route);
        cpxSendPacketBlocking(&packet);
    }
}


/**
 * @brief Callback called when a capture is completed by the camera
 * - must release the acquisition task in order to take a new capture
 */
static void capture_done_cb(void *arg) {
    pi_camera_control(&camera, PI_CAMERA_CMD_STOP, 0);
    xSemaphoreGive(capture_sem);
}

void cropImage(unsigned char *input, unsigned char *output) {
    // Calcul des coordonnées x et y pour le recadrage au centre
    int x = (CAM_WIDTH - CROP_WIDTH) / 2;
    int y = (CAM_HEIGHT - CROP_HEIGHT) / 2;

    // Vérification des dimensions de recadrage pour éviter les débordements
    if (x < 0 || y < 0 || CROP_WIDTH <= 0 || CROP_HEIGHT <= 0 ||
        x + CROP_WIDTH > CAM_WIDTH || y + CROP_HEIGHT > CAM_HEIGHT) {
        printf("Invalid cropping dimensions!\n");
        exit(1);
    }

    // Copie des pixels de l'image d'entrée vers la nouvelle image recadrée
    for (int i = 0; i < CROP_HEIGHT; i++) {
        for (int j = 0; j < CROP_WIDTH; j++) {
            // Calcul de l'indice du pixel dans l'image d'entrée
            int inputIndex = ((y + i) * CAM_WIDTH + (x + j)) * sizeof(unsigned char);
            // Calcul de l'indice correspondant dans la nouvelle image recadrée
            int outputIndex = (i * CROP_WIDTH + j) * sizeof(unsigned char);
            // Copie de la valeur du pixel
            output[outputIndex] = input[inputIndex];
        }
    }
}

/**
 * @brief Task enabling the acquisition/sending of an image
 * - Set the callback called at the end of a capture 
 * - Starts a new capture
 * - Calls the function for sending the image by wifi 
 */
void camera_task(void *parameters) {
    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Camera task A\n");

    pi_buffer_init(&buffer, PI_BUFFER_TYPE_L2, imgBuff);
    pi_buffer_set_format(&buffer, CAM_WIDTH, CAM_HEIGHT, 1, PI_BUFFER_FORMAT_GRAY);
    
    imgBuff = (unsigned char *)pmsis_l2_malloc(CAM_WIDTH*CAM_HEIGHT); 
    unsigned char *croppedImgBuff; 

    open_pi_camera_himax(&camera);

    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Camera task B\n");

    pi_task_t task;
    pi_camera_capture_async(&camera, imgBuff, CAM_WIDTH * CAM_HEIGHT, pi_task_callback(&task, capture_done_cb, NULL));
    pi_camera_control(&camera, PI_CAMERA_CMD_START, 0);
    cropImage(imgBuff, croppedImgBuff);


    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Camera task C\n");

    xSemaphoreTake(capture_sem, (TickType_t) 50);
    send_image_via_wifi(croppedImgBuff, CROP_WIDTH, CROP_HEIGHT);


    cpxPrintToConsole(LOG_TO_CRTP, "[LAB3] Camera task D\n");

    pmsis_l2_malloc_free(imgBuff, CAM_WIDTH*CAM_HEIGHT);

}

int open_pi_camera_himax(struct pi_device *device)
{
  struct pi_himax_conf cam_conf;

  pi_himax_conf_init(&cam_conf);

  cam_conf.format = PI_CAMERA_QVGA;

  pi_open_from_conf(device, &cam_conf);
  if (pi_camera_open(device))
    return -1;

  // rotate image
  pi_camera_control(device, PI_CAMERA_CMD_START, 0);
  uint8_t set_value = 3;
  uint8_t reg_value;
  pi_camera_reg_set(device, IMG_ORIENTATION, &set_value);
  pi_time_wait_us(1000000);
  pi_camera_reg_get(device, IMG_ORIENTATION, &reg_value);
  if (set_value != reg_value)
  {
    cpxPrintToConsole(LOG_TO_CRTP, "Failed to rotate camera image\n");
    return -1;
  }
  pi_camera_control(device, PI_CAMERA_CMD_STOP, 0);
  pi_camera_control(device, PI_CAMERA_CMD_AEG_INIT, 0);

  return 0;
}


/* Program Entry. */
int main(void) {
    pi_bsp_init();
    
    return pmsis_kickoff((void *) start);
}
