/*
 * tareas.c
 *
 *  Created on: 12 jun. 2021
 *      Author: gabri
 */

#include "tareas.h"

/*
 *********************************************************************************************************
 *                                        TAREA 1:  LecturaPulsadores()
 *
 * Description : Esta tarea lee 4 pulsadores de entrada y realiza el broadcast de un mailbox llamado
 * mBox_Pulsadores. Este mensaje contiene el valor de la tecla pulsada (1, 2 , 3 o 4)
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' LecturaPulsadores()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */


void LecturaPulsadores (void *p_arg)
{
	/*********************************************/
	//Creacion del mBox que contendrá la tecla pulsada
	//Este Mbox está dirigido a la tarea principal
	mBox_Pulsadores = OSMboxCreate((void *)0);
	/*********************************************/

	Pulsadores teclaPulsada = P_None;
	OSTimeDlyHMSM(0, 0, 1, 0);

	while(DEF_TRUE)
	{
		// Las entradas están en modo invertido debido a que los pulsadores físicos son NC.
		if(HAL_GPIO_ReadPin(GPIOB, PULSADOR1_Pin) == GPIO_PIN_RESET)
		{
			teclaPulsada = P_Menu;
			OSMboxPost(mBox_Pulsadores, &teclaPulsada); //Envío el Mbox con el dato de la tecla
			OSTimeDlyHMSM(0u, 0u, 0u, 500u);
		}else
			if(HAL_GPIO_ReadPin(GPIOB, PULSADOR2_Pin) == GPIO_PIN_RESET)
			{
				teclaPulsada = P_Up;
				OSMboxPost(mBox_Pulsadores, &teclaPulsada); //Envío el Mbox con el dato de la tecla
				if(estadoActual.menuPantalla == ModoMan)	//En modo manual incrementa la vel de lectura
				{
					OSTimeDlyHMSM(0u, 0u, 0u, 50u);
				}
				else
				{
					OSTimeDlyHMSM(0u, 0u, 0u, 500u);
				}
			} else
				if (HAL_GPIO_ReadPin(GPIOB, PULSADOR3_Pin) == GPIO_PIN_RESET)
				{
					teclaPulsada = P_Down;
					OSMboxPost(mBox_Pulsadores, &teclaPulsada); //Envío el Mbox con el dato de la tecla
					if(estadoActual.menuPantalla == ModoMan)
					{
						OSTimeDlyHMSM(0u, 0u, 0u, 50u);
					}
					else
					{
						OSTimeDlyHMSM(0u, 0u, 0u, 500u);
					}
				} else
					if(HAL_GPIO_ReadPin(GPIOB, PULSADOR4_Pin) == GPIO_PIN_RESET)
					{
						teclaPulsada = P_Start;
						OSMboxPost(mBox_Pulsadores, &teclaPulsada); //Envío el Mbox con el dato de la tecla
						OSTimeDlyHMSM(0u, 0u, 0u, 500u);
					}
					else
					{
						teclaPulsada = P_None;
						OSMboxPost(mBox_Pulsadores, &teclaPulsada); //Envío el Mbox con el dato de la tecla
						if(estadoActual.menuPantalla == ModoMan)
						{
							OSTimeDlyHMSM(0u, 0u, 0u, 50u);
						}
						else
						{
							OSTimeDlyHMSM(0u, 0u, 0u, 100u);
						}
					}



	}
}

/*
 *********************************************************************************************************
 *                                        TAREA 2:  Menu()
 *
 * Description : Esta tarea contiene la máquina de estados del menú del programa
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' Menu()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 *                                        TAREA 3:  TareaPrincipal()
 *
 * Description : Esta tarea contiene la máquina de estados del menú del programa
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' TareaPrincipal()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */
void TareaPrincipal (void *p_arg)

{
	ValoresDeArranque(); //inicializa los valores del programa


	int *dataPulsadores;	//Esta variable recibe mensajes
	int dataRejillas;
	int *dataFc_Sup;
	int *dataFc_Inf;

	/*//////////////////////////LOOP DE LA TAREA//////////////////////////////////////*/
	while(DEF_TRUE)
	{
		/*Recepción del mensaje de la tarea pulsadores */
		dataPulsadores=OSMboxAccept(mBox_Pulsadores);
		/*Recepción del mensaje de la tarea pulsadores */
		if (dataPulsadores != (void *)0) //Si hay contenido en el mensaje quiere decir que se pulsó una tecla
		{
			Pulsadores teclaPulsada = *dataPulsadores;
			if(teclaPulsada == P_Menu && !estadoActual.start )IncrementarMenu();

			switch(estadoActual.menuPantalla)
			{
			case ModoAut:
				ResetPassword();
				if(teclaPulsada == P_Start)
				{
					estadoActual.start = !estadoActual.start;
					//Iniciar modo automátcio
				}

				break;

			case ModoMan:
				if(teclaPulsada == P_Start)
				{
					estadoActual.start = !estadoActual.start;
				} else
					if(teclaPulsada == P_Up)
					{
						if(!estadoActual.giroHorario)
						{
							estadoActual.giroHorario = true;
							estadoActual.giroAntiHorario = false;
						}
					}
					else
						if(teclaPulsada == P_Down)
						{
							if(!estadoActual.giroAntiHorario)
							{
								estadoActual.giroHorario = false;
								estadoActual.giroAntiHorario = true;
							}
						}
						else
							if (teclaPulsada == P_None)
							{
								estadoActual.giroHorario = false;
								estadoActual.giroAntiHorario = false;
							}
				break;

			case Password1:
				if(teclaPulsada == P_Up)
				{
					estadoActual.digit0++;
					if(estadoActual.digit0 >= 10) estadoActual.digit0 = 0;
				}
				if(teclaPulsada == P_Down)
				{
					estadoActual.digit0--;
					if(estadoActual.digit0 >= 10 || estadoActual.digit0 < 0 ) estadoActual.digit0 = 9;
				}
				break;

			case Password2:
				if(teclaPulsada == P_Menu) estadoActual.puntero ++;
				else
					if(teclaPulsada == P_Up)
					{
						estadoActual.digit1++;
						if(estadoActual.digit1 >= 10) estadoActual.digit1 = 0;
					}
					else
						if(teclaPulsada == P_Down)
						{
							estadoActual.digit1--;
							if(estadoActual.digit1 >= 10 || estadoActual.digit1 < 0 ) estadoActual.digit1 = 9;
						}
				break;

			case Password3:
				if(teclaPulsada == P_Menu) estadoActual.puntero ++;
				else
					if(teclaPulsada == P_Up)
					{
						estadoActual.digit2++;
						if(estadoActual.digit2 >= 10) estadoActual.digit2 = 0;
					}
					else
						if(teclaPulsada == P_Down)
						{
							estadoActual.digit2--;
							if(estadoActual.digit2 >= 10 || estadoActual.digit2 < 0 ) estadoActual.digit2 = 9;
						}
				break;

			case Password4:
				if(teclaPulsada == P_Menu) estadoActual.puntero ++;
				else
					if(teclaPulsada == P_Up)
					{
						estadoActual.digit3++;
						if(estadoActual.digit3 >= 10) estadoActual.digit3 = 0;
					}
					else
						if(teclaPulsada == P_Down)
						{
							estadoActual.digit3--;
							if(estadoActual.digit3 >= 10 || estadoActual.digit3 < 0 ) estadoActual.digit3 = 9;
						}
				if(teclaPulsada == P_Start)
				{
					if(ChequearContrasena())
					{
						estadoActual.accesoPermitido = true;
						IncrementarMenu();
					}
					else
					{
						ResetPassword();
					}
				}

				break;

			case CantRejillas:
				if(teclaPulsada == P_Up)
				{
					estadoActual.nRejillas ++;
					if(estadoActual.nRejillas > 255) estadoActual.nRejillas =0;
				}
				else
					if(teclaPulsada == P_Down)
					{
						estadoActual.nRejillas --;
						if(estadoActual.nRejillas < 0 || estadoActual.nRejillas > 255 )
						{
							estadoActual.nRejillas =255;
						}
					}

				break;

			case Desplazamiento:
				if(teclaPulsada == P_Up)
				{
					estadoActual.distancia_mm ++;
					if(estadoActual.distancia_mm > 65.535) estadoActual.nRejillas =0;

				}
				else
					if(teclaPulsada == P_Down)
					{
						estadoActual.distancia_mm --;
						if(estadoActual.distancia_mm < 0 || estadoActual.distancia_mm > 65.535 )
						{
							estadoActual.nRejillas =65.535;
						}
					}
				break;
			case count:
			default:
				break;
			} // END SWITCH estadoActual.menuPantalla

			ResumirTareaDisplay(teclaPulsada);
		} // END if teclaPulsada

		/*Recepción del mensaje de la tarea rejillas */
		dataRejillas = OSSemAccept(sem_Rejillas);
		if (dataRejillas > 0)
		{
			estadoActual.rejillasActuales ++;
			if (estadoActual.rejillasActuales >= estadoActual.nRejillas)
			{
				//Elevador cargado
				estadoActual.rejillasActuales = 0;
			}
			ResumirTareaDisplay(P_Menu);
		}
		/*Recepción del mensaje de la tarea pulsadores */
		dataFc_Sup=OSMboxAccept(mBox_Fc_Sup);
		dataFc_Inf=OSMboxAccept(mBox_Fc_Inf);
		if (dataFc_Sup != (void *)0)
		{
			estadoActual.fc_Superior = *dataFc_Sup;
		}
		if (dataFc_Inf != (void *)0)
		{
			estadoActual.fc_Inferior = *dataFc_Inf;
		}
		/*Recepción del mensaje de la tarea pulsadores */








		OSTimeDlyHMSM(0, 0, 0u, 1u);


	}
	/*//////////////////////////LOOP DE LA TAREA//////////////////////////////////////*/
}

/*
 *********************************************************************************************************
 *                                        TAREA 4:  LecturaFC()
 *
 * Description : Esta tarea contiene la máquina de estados del menú del programa
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' LecturaFC()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */
void LecturaFC (void *p_arg)

{
	mBox_Fc_Sup = OSMboxCreate((void *)0);
	mBox_Fc_Inf = OSMboxCreate((void *)0);
	int Fc_Sup_Activo = false; // Para leer solo flanco ascendente
	int Fc_Inf_Activo = false; // Para leer solo flanco ascendente
	OSTimeDlyHMSM(0u, 0u, 1u, 500u);

	while(DEF_TRUE)
	{
		if((!Fc_Sup_Activo) && HAL_GPIO_ReadPin(GPIOA, Fc_Sup_Pin))
		{
			Fc_Sup_Activo = true;
			OSMboxPost(mBox_Fc_Sup, &Fc_Sup_Activo); //Le envio un mensaje a la mainTask con el estado del FC
			OSTimeDlyHMSM(0u, 0u, 1u, 500u);
		}
		else
		{
			if (Fc_Sup_Activo && (! HAL_GPIO_ReadPin(GPIOA, Fc_Sup_Pin)))
			{
				Fc_Sup_Activo = false; //Flanco descendente
				OSMboxPost(mBox_Fc_Sup, &Fc_Sup_Activo); //Le envio un mensaje a la mainTask con el estado del FC
				OSTimeDlyHMSM(0u, 0u, 1u, 500u);
			}
		}
		if(!Fc_Inf_Activo && HAL_GPIO_ReadPin(GPIOA, Fc_Inf_Pin))
		{
			Fc_Inf_Activo = true;
			OSMboxPost(mBox_Fc_Inf, &Fc_Inf_Activo); //Le envio un mensaje a la mainTask con el estado del FC
			OSTimeDlyHMSM(0u, 0u, 1u, 500u);
		}
		else
		{
			if (Fc_Inf_Activo && ! HAL_GPIO_ReadPin(GPIOA, Fc_Inf_Pin))
			{
				Fc_Inf_Activo = false; //Flanco descendente
				OSMboxPost(mBox_Fc_Inf, &Fc_Inf_Activo); //Le envio un mensaje a la mainTask con el estado del FC
				OSTimeDlyHMSM(0u, 0u, 1u, 500u);
			}
		}


		OSTimeDlyHMSM(0, 0, 0u, 100u); // Leo los finales de carrera 100 veces por segundo

	}
}
/*
 *********************************************************************************************************
 *                                        TAREA 5:  LecturaRejillas()
 *
 * Description : Esta tarea contiene la máquina de estados del menú del programa
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' LecturaRejillas()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */
void LecturaRejillas (void *p_arg)

{
	sem_Rejillas = OSSemCreate(0);
	bool mismaPieza = false; // Para leer solo flanco ascendente

	while(DEF_TRUE)
	{
		if(!mismaPieza && HAL_GPIO_ReadPin(GPIOA, Sens_Rejillas_Pin))
		{
			mismaPieza = true;
			OSSemPost(sem_Rejillas); //Le aviso a la tarea principal que llegó una rejillas
		}
		else
		{
			if (mismaPieza && ! HAL_GPIO_ReadPin(GPIOA, Sens_Rejillas_Pin))
			{
				mismaPieza = false; //Flanco descendente
			}
		}

		OSTimeDlyHMSM(0, 0, 0u, 200u); // Escaneo el sensor de rejillas 5 veces por segundo

	}
}
/*
 *********************************************************************************************************
 *                                        TAREA 6:  ControlMotor()
 *
 * Description : Esta tarea contiene la máquina de estados del menú del programa
 *
 *
 *
 *
 * Argument(s) : p_arg       Argument passed to ' ControlMotor()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */
void ControlMotor (void *p_arg)

{

	sem_Pulsos = OSSemCreate(0);
	OS_ERR os_err;
	uint16_t count = 0;
	bool run = false;

	while(DEF_TRUE)
	{

		/*Recepción del semáforo de la ISR */
		OSSemPend(sem_Pulsos, 0, &os_err);
		/*Recepción del semáforo de la ISR */

		if ((estadoActual.giroHorario || estadoActual.giroAntiHorario))
		{
			if(!run)
			{
				if(estadoActual.giroHorario)GiroHorario();
				else
					if(estadoActual.giroAntiHorario)GiroAntiHorario();
				run = true;
			}
		}
		else
		{
			run = false;
		}
		count++;

		if (count >= estadoActual.div_Frecuencia)
		{
			if(estadoActual.start && run)
			{
				HAL_GPIO_TogglePin(GPIOB, PUL_Pin );
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, PUL_Pin, GPIO_PIN_RESET);
			}
			count = 0;
		}


	}
}
/*
 *********************************************************************************************************
 *                                        TAREA 7:  ActualizarDisplay()
 *
 * Description : Esta tarea recibe un mensaje de la tarea pulsadores e imprime en el display
 * 				 La tecla que fue pulsada
 *
 *
 * Argument(s) : p_arg       Argument passed to ' ActualizarDisplay()' by 'OSTaskCreate()'.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : This is a task.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */
void ActualizarDisplay (void *p_arg)
{
	char buffer[15];			//Este buffer contiene los números de los parametros

	LiquidCrystal(GPIOA, LCD_RS_Pin, LCD_RW_Pin, LCD_EN_Pin, LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin);
	OSTimeDlyHMSM(0u, 0u, 1u, 0u);


	while(DEF_TRUE)
	{

		clear();
		setCursor(0,0);
		switch(estadoActual.menuPantalla)
		{
		case ModoAut:

			print("Modo Automatico");
			setCursor(0,1);
			print("Piezas ");
			sprintf(buffer,"%d",estadoActual.rejillasActuales);
			print(buffer);
			print("/");
			sprintf(buffer,"%d",estadoActual.nRejillas);
			print(buffer);


			if (estadoActual.errorTimeoutSup)
			{
				setCursor(0,1);
				print("Err Fc Sup");
			}
			else
			{
				if (estadoActual.errorTimeoutInf)
				{
					setCursor(0,1);
					print("Err Fc Inf");
				}

			}
			break;
		case ModoMan:

			print("Modo Manual");

			break;

		case Password1:
		case Password2:
		case Password3:
		case Password4:
			print("Clave de acceso");
			setCursor(0,1);
			sprintf(buffer,"%d %d %d %d",
					estadoActual.digit0,
					estadoActual.digit1,
					estadoActual.digit2,
					estadoActual.digit3);
			print(buffer);

			setCursor((estadoActual.puntero *2) + 1,1);
			print("<");
			break;

		case CantRejillas:

			print("Cant de Piezas");
			setCursor(0,1);
			sprintf(buffer,"%d",estadoActual.nRejillas);
			print(buffer);
			break;

		case Desplazamiento:
			print("Distancia en mm");
			setCursor(0,1);
			sprintf(buffer,"%d",estadoActual.distancia_mm);
			print(buffer);
			break;


		case count:
			break;

		}
		if(estadoActual.start)
		{
			setCursor (12,1);
			print("RUN");
			blink();
		}
		else
		{
			noBlink();
		}

		OSTaskSuspend(APP_CFG_TASK7_PRIO); //Se autosuspende
	}






}

void ValoresDeArranque()
{
	estadoActual.start = false;
	estadoActual.accesoPermitido = false;
	estadoActual.errorTimeoutSup = false;
	estadoActual.errorTimeoutInf = false;
	estadoActual.giroHorario = false;
	estadoActual.giroAntiHorario = false;
	estadoActual.elevadorCargado = false;
	estadoActual.digit0 = 0;
	estadoActual.digit1 = 0;
	estadoActual.digit2 = 0;
	estadoActual.digit3 = 0;
	estadoActual.puntero=0;

	estadoActual.rejillasActuales=0;
	estadoActual.nRejillas=5;
	estadoActual.distancia_mm = 3000;
	estadoActual.div_Frecuencia = 1;
	estadoActual.menuPantalla = ModoAut;

}

void ResetPassword()
{
	estadoActual.digit0 = 0;
	estadoActual.digit1 = 0;
	estadoActual.digit2 = 0;
	estadoActual.digit3 = 0;
	estadoActual.puntero =0;
	estadoActual.accesoPermitido = false;

}

void IncrementarMenu()
{
	estadoActual.menuPantalla++;

	if(!estadoActual.accesoPermitido &&
			(estadoActual.menuPantalla == CantRejillas || estadoActual.menuPantalla == Desplazamiento ) )
	{
		estadoActual.menuPantalla = ModoAut;		//No permite el acceso a  no ser que digite contraseñla
	}

	if (estadoActual.menuPantalla == count)
	{
		estadoActual.menuPantalla = ModoAut;		//El menú es circular
	}
}

bool ChequearContrasena()
{
	if(		estadoActual.digit0 == contrasena[0] &&
			estadoActual.digit1 == contrasena[1] &&
			estadoActual.digit2 == contrasena[2] &&
			estadoActual.digit3 == contrasena[3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GiroHorario()
{
	HAL_GPIO_WritePin(GPIOB, ENA_Pin, GPIO_PIN_RESET); 		//HABILITO DRIVER
	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, GPIO_PIN_RESET); 		//GIRO PA UN LADO
	OSTimeDlyHMSM(0, 0, 0, 1u);
	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void GiroAntiHorario()
{
	HAL_GPIO_WritePin(GPIOB, ENA_Pin, GPIO_PIN_RESET); 		//HABILITO DRIVER
	OSTimeDlyHMSM(0, 0, 0, 1u);
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, GPIO_PIN_SET); 		//GIRO PA OTRO LADO
	OSTimeDlyHMSM(0, 0, 0, 1u);
	//HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void ResumirTareaDisplay (Pulsadores teclaPulsada)
{
	if((teclaPulsada != P_None) &&
			!((estadoActual.menuPantalla == ModoMan )&&(teclaPulsada == P_Up || teclaPulsada == P_Down ))
	)
	{
		OSTaskResume(APP_CFG_TASK7_PRIO);
	}
}
