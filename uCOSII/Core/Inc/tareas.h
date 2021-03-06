/*
 * tareas.h
 *
 *
 *  Created on: May 22, 2021
 *      Author: Gabriel Martin
 *
 */

#ifndef TAREAS_H_
#define TAREAS_H_


#include <app_cfg.h>
#include <cpu_core.h>
#include <os.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "LiquidCrystal.h"

/***************************************************************
 *					PARÁMETROS DE CALIBRACIÓN
 **************************************************************/
#define DESPLAZAMIENTO_X_REV 0.1	//cuantos mm se desplaza linealmente por cada vuelta del mot
#define PUSLOS_X_REV  		 400			//seteado en el driver

#define timeout_Fc_Sup 		4000		//Tiempo en el que busca el final de carrera superior
#define timeout_Fc_Inf 		4000		//Tiempo en el que busca el final de carrera inferior
#define tiempoDescarga 		2000		//Tiempo de reposo de la descarga superior
/***************************************************************
 *					PARÁMETROS DE CALIBRACIÓN
 **************************************************************/

/***************************************************************
 *					ENUMERATIONS
 **************************************************************/
typedef enum
{
	ModoAut,
	ModoMan,
	Password1,
	Password2,
	Password3,
	Password4,
	CantRejillas,
	Desplazamiento,
	count
}menuStates;				//Los estados que tiene el menú del LCD

typedef enum
{
	P_None,
	P_Menu,
	P_Up,
	P_Down,
	P_Start
}Pulsadores;				//Los 4 pulsadores posibles

/***************************************************************
 *					ENUMERATIONS
 **************************************************************/
/***************************************************************
 *					VARIABLES GLOBALES
 **************************************************************/

OS_EVENT *mBox_Pulsadores;			//Mbox que envía la tarea pulsadores a Tarea Principal
OS_EVENT *sem_Rejillas;
OS_EVENT *mBox_Fc_Sup;
OS_EVENT *mBox_Fc_Inf;
OS_EVENT *sem_Pulsos;
OS_EVENT *sem_ModAutomatico;		//Sem que envia la maintask a la tarea modo automatico
OS_ERR ISR_os_err;

static const uint8_t contrasena[4] = {1,2,3,4};

struct programState
{
	bool start;
	bool accesoPermitido;
	bool errorTimeoutSup;
	bool errorTimeoutInf;
	bool giroHorario;
	bool giroAntiHorario;
	bool elevadorCargado;
	bool fc_Superior;
	bool fc_Inferior;
	bool rejillas_Ready;
	uint8_t digit0;
	uint8_t digit1;
	uint8_t digit2;
	uint8_t digit3;
	uint8_t puntero;
	uint8_t rejillasActuales;
	uint8_t nRejillas;				//Cantidad de rejillas default para un ciclo de elevador
	uint16_t distancia_mm;
	uint8_t div_Frecuencia;
	menuStates menuPantalla;


};
struct programState estadoActual;// Estructura que contiene el estado actual del programa

/***************************************************************
 *					VARIABLES GLOBALES
 **************************************************************/

/***************************************************************
 *							Prototipo funciones / Tareas
 **************************************************************/
void LecturaPulsadores  (void *p_arg);					//Tarea 1
void CicloAutomatico 	(void *p_arg);					//Tarea 2
void TareaPrincipal		(void *p_arg);					//Tarea 3
void LecturaFC			(void *p_arg);					//Tarea 4
void LecturaRejillas	(void *p_arg);					//Tarea 5
void ControlMotor		(void *p_arg);					//Tarea 6
void ActualizarDisplay  (void *p_arg);					//Tarea 7

void ValoresDeArranque();
void ResetPassword();
void IncrementarMenu();
bool ChequearContrasena();
void GiroHorario();
void GiroAntiHorario();
void ResumirTareaDisplay (Pulsadores teclaPulsada);
/***************************************************************
 *							Prototipo funciones / Tareas
 **************************************************************/





#endif /* TAREAS_H_ */






