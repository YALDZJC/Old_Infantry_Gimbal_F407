#define GIMBAL_GLOBALS
#include "includes.h"
int a;
PID_param_struct_t speed_PID_param_struct = { 13, 0.18, 0, 2500, 16384 };	/* Ħ����PID���� */	/* kp, ki, kd, �����޷�������޷� */
PID_param_struct_t dial_speed_PID_param_struct = { 20, 0, 0, 0, 10000 };	/* ����PID���� */ /* kp, ki, kd, �����޷�������޷� */

#ifndef infantry_2

/* �ǶȾ���ֵ�µ�yaw����PID���� */
primary_PID_param_struct_t absolute_yaw_primary_structure_gimbal = { 0.54, 0, 0.1, 500, 30000 };
secondary_PID_param_struct_t absolute_yaw_secondary_structure_gimbal = { 90, 0, 0, 0, 30000 };

/* �ǶȾ���ֵ�µ�pitch����PID���� */
primary_PID_param_struct_t absolute_pitch_primary_structure_gimbal = { 0.45, 0.02, 0, 1000, 30000 };//0.45 0.02
secondary_PID_param_struct_t absolute_pitch_secondary_structure_gimbal = { 100, 0.01, 0, 0, 30000 };

/* �������µ�yaw����PID���� */
primary_PID_param_struct_t gyro_yaw_primary_structure_gimbal = { 4, 0, 0, 1000, 30000 };
secondary_PID_param_struct_t gyro_yaw_secondary_structure_gimbal = { 24, 0, 0, 0, 30000 };

/* �������µ�pitch����PID���� */
primary_PID_param_struct_t gyro_pitch_primary_structure_gimbal = { 0, 0, 0, 0, 30000 };
secondary_PID_param_struct_t gyro_pitch_secondary_structure_gimbal = { 0, 0, 0, 0, 30000 };

primary_PID_param_struct_t dial_angle_PID_param_struct = {0.35, 0, 0.5, 0, 30000};  
secondary_PID_param_struct_t dial_speed_angle_PID_param_struct = {2, 0, 9, 0, 30000};

#endif /* infantry_2 */

int16_t absolute_value_0x205 = initial_absolute_angle_0x205;	/* yaw�������ԽǶ�Ŀ��ֵ */  /* ��ʼ��ʱ�ľ��ԽǶ�Ŀ��ֵ */
int16_t absolute_value_0x206 = initial_absolute_angle_0x206;	/* pitch�������ԽǶ�Ŀ��ֵ *//* ��ʼ��ʱ�ľ��ԽǶ�Ŀ��ֵ */
int32_t gyro_value_0x205;	/* ���������ǽǶȵ�yaw����Ŀ��ֵ */
int32_t gyro_value_0x206;	/* ���������ǽǶȵ�pitch����Ŀ��ֵ */

/* ���̺�Ħ���ֵ���ٶ�Ŀ��ֵ */
int16_t speed_value_0x201;
int16_t speed_value_0x202;
int16_t speed_value_0x203;
int32_t angle_value_0x201;

int32_t send_ms = 50;
int64_t vision_value_0x201;
int64_t vision_send_ms;


uint16_t test_high;
uint16_t test_low;

uint16_t heat_error;

uint16_t test_angle;
uint16_t test_friction_value1 = 1000;
uint16_t test_friction_value2 = 1000;
float buchang;
uint8_t SHOOT_HEAT_DEAD_BAND = 30;

uint8_t first_flag;
uint16_t shoot_flag;
uint8_t temp_shoot_flag;

uint16_t servo_angle = 420;

uint8_t C_latch_flag;
uint8_t V_latch_flag;
uint8_t press_V_lock;
bool vision_lock;



int16_t b;
/* ��̨��ת�ٶȿ��Ʊ��� */
// ң����
#ifndef infantry_2
float remote_yaw_PTZ_control_coefficient = 0.015f;
float remote_pitch_PTZ_control_coefficient = 0.003f;
#endif /* infantry_2 */
// ����
#ifndef infantry_2
float	mouse_yaw_PTZ_control_coefficient = 0.06f;
float mouse_pitch_PTZ_control_coefficient = 0.04f;
#endif /* infantry_2 */

/* ���ڲ���Ħ����ת�� */
int16_t test_speed_max;
int16_t test_speed_min;

/* �����ж�ң��������ģʽ */
uint8_t remote_mode;

/* ���� */
uint8_t F_latch_flag;
uint8_t R_latch_flag;
uint8_t V_latch_flag;

uint8_t reverse_flag;
uint8_t cover_flag;
uint8_t armor_flag;

/* �Ӿ����Ʊ�ʶ�� */
uint8_t vision_control_flag;

enum vision_mode_enum
{
	armor = 0,
	robot = 1,
};
extern uint8_t vision_mode;
extern uint8_t vision_fire_flag;
uint32_t current_timestamp;
uint32_t mode_32_timestamp;

//ude������
float ff_out;
float ff_k = 100;
float ude_k = 0;
float ude_b;
float ude_u0;
float ude_out;
float ude_xn;
float ude_diff;
float ude_break = 0;
float ude_max = 1000;
float ude_min = -1000;

float Kp = 110, P_out ,Kd, D_out, Ki = 0.25, I_out, pid_out, err, pid_max = 25000, I_break = 4.5, break_k = 0, I_max = 500;
float speed_Kp = 15, speed_P_out ,speed_Kd, speed_D_out, speed_Ki, I_out, speed_pid_out;
float out_all;

float pitch_out;

float tar_ff = 1000, tar_ff_out;

/* ��̨�������� */
void gimbal_task( void *pvParameters )
{
	for( ;; )
	{
		//��ȡϵͳʱ�������λΪms
		current_timestamp = HAL_GetTick();
		
		/* �򵥴ֱ��������ORE���� */
		if( __HAL_UART_GET_FLAG( &remote_uart, UART_FLAG_ORE ) != RESET )
		{
			__HAL_UART_CLEAR_OREFLAG( &remote_uart );
			HAL_UARTEx_ReceiveToIdle_IT( &remote_uart, remote_rx_buf, remote_data_size );
//			HAL_UARTEx_ReceiveToIdle_DMA( &remote_uart, remote_rx_buf, remote_data_size );
		}
		
		if( __HAL_UART_GET_FLAG( &communication_uart, UART_FLAG_ORE ) != RESET )
		{
			__HAL_UART_CLEAR_OREFLAG( &communication_uart );
			HAL_UARTEx_ReceiveToIdle_IT( &communication_uart, communication_rx_buf, communication_receive_data_size );
		}
		
		/* ��ʼ��״̬ */
		if( switch_flag == 0 )
		{
			/* pitch��yaw��ת����ʼλ�� */
			#ifndef infantry_2
			absolute_angle_cascade_calculation( 0x205, initial_absolute_angle_0x205, Both, absolute_yaw_primary_structure_gimbal, absolute_yaw_secondary_structure_gimbal );
			absolute_angle_cascade_calculation( 0x206, initial_absolute_angle_0x206, Both, absolute_pitch_primary_structure_gimbal, absolute_pitch_secondary_structure_gimbal );
			#endif /* infantry_2 */
			
			rotating_speed_calculation( 0x201, 0, 0, dial_speed_PID_param_struct );
			
			theta = 0;
			yaw_target = 0;
			pitch_target = 0;
			
			first_flag = 0;
		}
		/* ����״̬ */
		else if( switch_flag == 1 )
		{
			/* ��һ��������������Ǻ͵�����ۼƽǶ� */
			if( first_flag == false )
			{
				roll_cumulative_change_angle = 0;
				pitch_cumulative_change_angle = 0;
				yaw_cumulative_change_angle = 0;
				
				motor_angle_sum_clear( 0x205 );
				motor_angle_sum_clear( 0x206 );
				
				first_flag = true;
			}
			
			/* ���������̨�нǽǶ� */
			#ifndef infantry_2
			if( motor_info_list[0]->angle_change_sum > 0 )
				theta = 360 - ( ( motor_info_list[0]->angle_change_sum % 16384 ) / 45.511111f );
			else
				theta = abs( ( motor_info_list[0]->angle_change_sum % 16384 ) / 45.511111f );
			if( theta == 360 )
				theta = 0;
			#endif	
			/* ң���źŶ������� */
			if( remote_error == true )
			{
				RecMsg.remote.ch0 = 1024;
				RecMsg.remote.ch1 = 1024;
				RecMsg.remote.ch2 = 1024;
				RecMsg.remote.ch3 = 1024;
				RecMsg.remote.s1 = 1;
				RecMsg.remote.s2 = 1;
				speed_value_0x201 = 0;
			}
			
			/* �ж�ң��������ģʽ */
			switch( RecMsg.remote.s1 ){
				
				case 1:
					switch( RecMsg.remote.s2 ){
						case 1: remote_mode = 11; break;
						case 3: remote_mode = 13; break;
						case 2: remote_mode = 12; break;
					}
					break;
					
				case 3:
					switch( RecMsg.remote.s2 ){
						case 1: remote_mode = 31; break;
						case 3: remote_mode = 33; break;
						case 2: remote_mode = 32; break;
					}
					break;
					
				case 2:
					switch( RecMsg.remote.s2 ){
						case 1: remote_mode = 21; break;
						case 3: remote_mode = 23; break;
						case 2: remote_mode = 22; break;
					}
					break;
			}
			
			/* ����ģʽ */
			if( remote_mode == 12)
			{
				if( vision_enable == 0 )
				{
					/* yaw����ת�Ƕȼ��� */
					if( RecMsg.mouse.x_axis )
						yaw_target += (float) ( -RecMsg.mouse.x_axis * mouse_yaw_PTZ_control_coefficient );
					
					/* pitch����ת�Ƕȼ��� */
					if( RecMsg.mouse.y_axis )
						pitch_target += (float) ( RecMsg.mouse.y_axis * mouse_pitch_PTZ_control_coefficient );
				}
				
				/* ǰ���� */
				if( RecMsg.KeyBoard.key.F_key && F_latch_flag == 0 )
				{
					if( reverse_flag == 0 )
					{
						reverse_flag = 1;
					}
					else
						reverse_flag = 0;
					
					F_latch_flag = 1;
				}
				else if( !RecMsg.KeyBoard.key.F_key )
				{
					F_latch_flag = 0;
				}
				
				/* ���ص��ո� */
//				if( RecMsg.KeyBoard.key.R_key && R_latch_flag == 0 )
//				{
//					if( cover_flag == 0 )
//					{
//						cover_flag = 1;
//						#ifndef infantry_2
//						servo_angle = 465;
//						#else
//						servo_angle = 465;
//						#endif
//					}
//					else
//					{
//						cover_flag = 0;
//						#ifndef infantry_2
//						servo_angle = 330;
//						#else
//						servo_angle = 265;
//						#endif
//					}
//					
//					R_latch_flag = 1;
//				}
//				else if( !RecMsg.KeyBoard.key.R_key )
//				{
//					R_latch_flag = 0;
//				}
				if( RecMsg.KeyBoard.key.E_key)
				{
					#ifndef infantry_2
					servo_angle = 420;
					#else
					servo_angle = 465;
					#endif
				}
				if(RecMsg.KeyBoard.key.R_key)
				{
					#ifndef infantry_2
					servo_angle = 150;
					#else
					servo_angle = 265;
					#endif
				}
				
				/* ��� */
				if( RecMsg.mouse.press_left)
					speed_value_0x201 = -3500;  
				else
					speed_value_0x201 = 0;
				
			
				/* �Ӿ����� */
				if( RecMsg.mouse.press_right)
				{
					vision_enable = 1;
					if(vision_enable == 1)
					{
//						if(vision_fire_flag == 1 && vision_mode == armor && vision_lock == false && temp_shoot_flag == 1)
//						{
//							angle_value_0x201 -= 32768 * 1;
//							vision_lock =  true;
//						}
//						else if(vision_fire_flag == 0)
//						{
//							vision_lock = false;
//						}

					}
				}
				else
				{
					vision_enable = 0;
				}
				
//				if( RecMsg.KeyBoard.key.C_key )
//				{
//					windmill_enable=1;
//					vision_mode = robot;
//				}
				if( RecMsg.KeyBoard.key.V_key)
				{
					windmill_enable=0;
					
					if(armor_flag == 0)
					{
						angle_value_0x201 = motor_info_list[2]->angle_change_sum;
						armor_flag = 1;
					}
					
					vision_mode = armor;
				}
				else
				{
					vision_mode = robot;
					armor_flag = 0;
				}
				//���ָ��������أ������²��ָ���
//				else
//				{
//					windmill_enable=0;
//				}
				if( RecMsg.KeyBoard.key.CTRL_key )
				{
					switch_flag = 0;
					first_flag = false;
//					press_V_lock = 0;
					xTimerStart( xResetTimer, 0 );
					TIM8->CCR2 = 1000; //Ħ���ֵ����λ
					TIM8->CCR3 = 1000; //Ħ���ֵ����λ
				}
				if( RecMsg.KeyBoard.key.G_key )
				{
					temp_shoot_flag = 1;
					b=1;
				}
				if(RecMsg.KeyBoard.key.B_key)
				{
					speed_value_0x202 = 0;
					speed_value_0x203 = 0;
					shoot_flag = 0;
					temp_shoot_flag = 0;
					b=0;
				}
				if(temp_shoot_flag) {
					if(shoot_flag >= 0 && shoot_flag < 1000)
					{
						speed_value_0x202 = 7000;
						shoot_flag += 1;
					}
					if(shoot_flag == 1000)
					{
						speed_value_0x203 = 7000;
					}
				}
				
//				if(RecMsg.KeyBoard.key.C_key && C_latch_flag == 0)
//				{
//					C_latch_flag = 1;
//					TIM8->CCR2 = 1420;
//					TIM8->CCR3 = 1420;
//				}
//				else if(!RecMsg.KeyBoard.key.C_key)
//				{
//					C_latch_flag = 0;
//				}
				
//				if(RecMsg.KeyBoard.key.V_key && V_latch_flag == 0)
//				{
//					V_latch_flag = 1;
//					TIM8->CCR2 = 1230;
//					TIM8->CCR3 = 1230;
//					bullet_speed = 10;
//					press_V_lock = 1;
//				}
//				else if(!RecMsg.KeyBoard.key.V_key)
//				{
//					V_latch_flag = 0;
//				}
			}
			
			/* ң�����������ߡ����С�С����ģʽ */
			else if( remote_mode == 11 || remote_mode == 31 || remote_mode == 21||remote_mode == 23)
			{
				/* yaw����ת�Ƕȼ��� */
				if( RecMsg.remote.ch0 < 1024 - REMOTE_DEAD_BAND || RecMsg.remote.ch0 > 1024 + REMOTE_DEAD_BAND )
					yaw_target += (float) -( ( RecMsg.remote.ch0 - 1024 ) * remote_yaw_PTZ_control_coefficient );
				
				/* pitch����ת�Ƕȼ��� */
				if( RecMsg.remote.ch1 < 1024 - REMOTE_DEAD_BAND || RecMsg.remote.ch1 > 1024 + REMOTE_DEAD_BAND )
					pitch_target += (float) -( ( RecMsg.remote.ch1 - 1024 ) * remote_pitch_PTZ_control_coefficient );
				
				/* ���̡�Ħ���ֿ��� */
				speed_value_0x201 = 0;
				TIM8->CCR2 = 1000;
				TIM8->CCR3 = 1000;
				
				speed_value_0x202 = 0;
				speed_value_0x203 = 0;
				
				shoot_flag = 0;
				
				/* ���ոǹ� */
				#ifndef infantry_2
				servo_angle = 420;
				#else
				servo_angle = 465;
				#endif
				temp_shoot_flag=0;
			}

			/* ң�����������ģʽ */
			else if( remote_mode == 13 )
			{
					if( motor_info_list[4]->rpm > test_speed_max )
						test_speed_max = motor_info_list[4]->rpm;
					else if( motor_info_list[4]->rpm < test_speed_min )
						test_speed_min = motor_info_list[4]->rpm;
					
					/* yaw����ת�Ƕȼ��� */
					if( RecMsg.remote.ch0 < 1024 - REMOTE_DEAD_BAND || RecMsg.remote.ch0 > 1024 + REMOTE_DEAD_BAND )
						yaw_target += (float) -( ( RecMsg.remote.ch0 - 1024 ) * remote_yaw_PTZ_control_coefficient );
					
					/* pitch����ת�Ƕȼ��� */
					if( RecMsg.remote.ch1 < 1024 - REMOTE_DEAD_BAND || RecMsg.remote.ch1 > 1024 + REMOTE_DEAD_BAND )
						pitch_target += (float) -( ( RecMsg.remote.ch1 - 1024 ) * remote_pitch_PTZ_control_coefficient );
					
					/* ������ת�ٶȼ��� */
					if( RecMsg.remote.ch3 < 1024 - REMOTE_DEAD_BAND || RecMsg.remote.ch3 > 1024 + REMOTE_DEAD_BAND)
						speed_value_0x201 = ( RecMsg.remote.ch3 - 1024 ) * 8;
					else
						speed_value_0x201 = 0;
					
					if(shoot_flag >= 0 && shoot_flag < 1000)
					{
						speed_value_0x202 = 7000;
						shoot_flag += 1;
					}
					if(shoot_flag == 1000)
					{
						speed_value_0x203 = 7000;
					}
					
					/* ���ոǿ� */
					#ifndef infantry_2
					servo_angle = 150;
					#else
					servo_angle = 265;
					#endif
					temp_shoot_flag=1;
					
			}
			else if(remote_mode == 22)
			{
				motor_info_list[0]->final_output = 0;
				motor_info_list[1]->final_output = 0;
				motor_info_list[2]->final_output = 0;
				motor_info_list[3]->final_output = 0;
				motor_info_list[4]->final_output = 0;
			}
			
			else
			{
				RecMsg.remote.ch0 = 1024;
				RecMsg.remote.ch1 = 1024;
				RecMsg.remote.ch2 = 1024;
				RecMsg.remote.ch3 = 1024;
				speed_value_0x201 = 0;
				speed_value_0x202 = 0;
				speed_value_0x203 = 0;
				temp_shoot_flag=0;
			}
				
	//			/* ���� */
				if( reverse_flag )
					theta = ( theta + 180 ) % 360;
				
	//			/* pitch����ת�Ƕ��޷� */
				if( pitch_target > 788 )
					pitch_target = 788;
				else if( pitch_target < -1075 )
					pitch_target = -1075;
							//			/*Ħ���ֱ�����������Ħ���ֲ��̲�ת��*/
				if(temp_shoot_flag==0)
				{
					speed_value_0x201 = 0;
				}
				
				if( TIM8->CCR2 > 1420 )
					TIM8->CCR2 = 1420;
				if( TIM8->CCR3 > 1420 )
					TIM8->CCR3 = 1420;
				
				bullet_speed = 28;
				
				/* ��ת�Ƕ�ת��Ϊ������ԽǶȺ���Ƶ����ת */
				if( gyro_error == false )
				{
					#ifndef infantry_2
					/* ������������������ǽǶȿ���yaw */
					tar_ff_out = tar_ff*(yaw_target - yaw_target_last);
					gyro_value_0x205 = yaw_target;
					yaw_target_last = yaw_target;

//					CH110_gyro_angle_cascade_calculation( 0x205, gyro_value_0x205, yaw, gyro, false, gyro_yaw_primary_structure_gimbal, gyro_yaw_secondary_structure_gimbal );
					
					if( motor_info_list[0]->rpm < 32767 )
					{
						ude_xn = motor_info_list[0]->rpm;
					}
					else if( motor_info_list[0]->rpm > 32767 )
					{
						ude_xn = motor_info_list[0]->rpm - 65535 ;
					}		
					
					ff_out = ff_k * ude_xn;

					err = (gyro_value_0x205/22.75) - yaw_cumulative_change_angle;
					
					P_out = Kp * (err);
					D_out = Kd * (0 - CH110_data.Z_axisAngularVelocity);
					
					if(fabs(err) > I_break)
					{
						I_out += break_k*Ki * (err);
					}
					else
						I_out += Ki * (err);
					
					if(I_out > 500)
						I_out = 500;
					if(I_out <-500)
						I_out = -500;					
					pid_out  = P_out+I_out+D_out;
					
					speed_P_out = speed_Kp * (pid_out - CH110_data.Z_axisAngularVelocity);
					
					if(abs(gyro_angle_cascade_list[0]->primary_error) > ude_break)
					{
						ude_u0 += 0.3*(speed_P_out/1.52587891)*0.00018311*0.741; 
					}
					else
						ude_u0 += (speed_P_out/1.52587891)*0.00018311*0.741; 

					if(ude_u0 > ude_max)
						ude_u0 = ude_max;
					if(ude_u0 < -ude_max)
						ude_u0 = -ude_max;
					
					ude_out = ude_k * (ude_xn - ude_u0);
					
					out_all = tar_ff_out + ff_out + speed_P_out - ude_out;
					if(out_all > pid_max)
						out_all = pid_max;
					if(out_all < -pid_max)
						out_all = -pid_max;			
					
					motor_info_list[0]->final_output = out_all;
					
					/* ������������������ǽǶȿ���pitch */
					absolute_value_0x206 = absolute_angle_remainder( (int32_t) initial_absolute_angle_0x206 + (int32_t) pitch_target);
					absolute_angle_cascade_calculation( 0x206, absolute_value_0x206, Both, absolute_pitch_primary_structure_gimbal, absolute_pitch_secondary_structure_gimbal );
					#endif /* infantry_2 */
				}
				else if( gyro_error == true )
				{
					#ifndef infantry_2
					/* ������������þ���ֵ����yaw */
					absolute_value_0x205 = absolute_angle_remainder( (int32_t) initial_absolute_angle_0x205 + (int32_t) yaw_target );
					absolute_angle_cascade_calculation( 0x205, absolute_value_0x205, Both, absolute_yaw_primary_structure_gimbal, absolute_yaw_secondary_structure_gimbal );
			
					
					/* ������������þ���ֵ����pitch */
					absolute_value_0x206 = absolute_angle_remainder( (int32_t) initial_absolute_angle_0x206 + (int32_t) pitch_target );
					absolute_angle_cascade_calculation( 0x206, absolute_value_0x206, Both, absolute_pitch_primary_structure_gimbal, absolute_pitch_secondary_structure_gimbal );
					#endif /* infantry_2 */
				}
				
				/* ���Ʋ��̺�Ħ����ת�� */
				
				rotating_speed_calculation( 0x202, speed_value_0x202, 9000, speed_PID_param_struct );
				rotating_speed_calculation( 0x203, -speed_value_0x203, 9000, speed_PID_param_struct );
				
//				if(vision_mode == armor)
//					relative_angle_cascade_calculation( 0x201, angle_value_0x201,0 ,0 , dial_angle_PID_param_struct, dial_speed_angle_PID_param_struct);
//				if(vision_mode == robot)
				rotating_speed_calculation( 0x201, speed_value_0x201, 4000, dial_speed_PID_param_struct );
			}
		/* ���ոǿ��� */
		TIM1->CCR1 = servo_angle;
		
		if(remote_mode == 33 || remote_mode == 32 || remote_mode == 22)
		{
			motor_info_list[0]->final_output = 0;
			motor_info_list[1]->final_output = 0;
			motor_info_list[2]->final_output = 0;
			motor_info_list[3]->final_output = 0;
			motor_info_list[4]->final_output = 0;
		}
			
		/* ���͵������ */
		motor_control_send( motor_can1 );
		
		/* ��������2ms */
		vTaskDelay( pdMS_TO_TICKS( 1 ) );
	}
}

void vResetTimerCallback( TimerHandle_t xResetTimer )
{
	switch_flag = 1;
}
