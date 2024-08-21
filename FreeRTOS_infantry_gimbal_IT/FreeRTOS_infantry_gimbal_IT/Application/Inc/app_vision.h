#ifdef APP_VISION_GLOBALS
#define APP_VISION_EXTERN
#else
#define APP_VISION_EXTERN extern
#endif

typedef struct{
	float vision_pitch_increment_angle;
	float vision_yaw_increment_angle;
}vision_struct_t;

APP_VISION_EXTERN uint8_t vision_fire;
APP_VISION_EXTERN uint8_t vision_ready;

APP_VISION_EXTERN uint8_t vision_send_buf[12];

APP_VISION_EXTERN vision_struct_t vision_struct;

/* �Ӿ����Ա�ʶ�� */
APP_VISION_EXTERN uint8_t vision_enable , windmill_enable;//�Ӿ�������������������

APP_VISION_EXTERN uint8_t bullet_speed ;

void vision_recive_task( void *pvParameters );
void vision_send_task( void *pvParameters );
void receive_task();
