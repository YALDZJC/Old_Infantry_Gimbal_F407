#ifdef FREERTOS_ABOUT_GLOBALS
#define FREERTOS_ABOUT_EXTERN
#else
#define FREERTOS_ABOUT_EXTERN extern
#endif

FREERTOS_ABOUT_EXTERN SemaphoreHandle_t xCAN1_Semaphore;
FREERTOS_ABOUT_EXTERN SemaphoreHandle_t xCAN2_Semaphore;

FREERTOS_ABOUT_EXTERN xQueueHandle RemoteMsgQueue;
FREERTOS_ABOUT_EXTERN xQueueHandle CanMsgQueue;
FREERTOS_ABOUT_EXTERN xQueueHandle CH110MsgQueue;
FREERTOS_ABOUT_EXTERN xQueueHandle CommunicationMsgQueue;
FREERTOS_ABOUT_EXTERN xQueueHandle VisionMsgQueue;

void freertos_init(void);
