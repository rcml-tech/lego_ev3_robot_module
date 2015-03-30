/*
* File: lego_module.cpp
* Author: m79lol, iskinmike
*
*/
#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>
#include <map>

#include <array>

#include "module.h"
#include "function_module.h"
#include "robot_module.h"
#include "lego_module.h"

#include "SimpleIni.h"

using namespace std;

////////////// Установка Глобальных переменных.
int COUNT_LEGO_FUNCTIONS = 20; 
int COUNT_AXIS = 3;

char *PATH_TO_CONFIG = "robot_modules/config.ini"; 


#define ADD_LEGO_FUNCTION(FUNCTION_NAME, COUNT_PARAMS, GIVE_EXCEPTION) \
	lego_functions[function_id] = new FunctionData; \
	lego_functions[function_id]->command_index = function_id + 1; \
	lego_functions[function_id]->count_params = COUNT_PARAMS; \
	lego_functions[function_id]->give_exception = GIVE_EXCEPTION; \
	lego_functions[function_id]->name = FUNCTION_NAME; \
	function_id++;
// Конец макроса

#define DEFINE_ALL_FUNCTIONS \
	ADD_LEGO_FUNCTION("motorBreak", 2, false)\
	ADD_LEGO_FUNCTION("motorGetDirection", 2, false)\
	ADD_LEGO_FUNCTION("motorGetTacho", 2, false)\
	ADD_LEGO_FUNCTION("motorMoveTo", 5, false)\
	ADD_LEGO_FUNCTION("motorOff", 2, false)\
	ADD_LEGO_FUNCTION("motorResetTacho", 2, false)\
	ADD_LEGO_FUNCTION("motorSetDirection", 3, false)\
	ADD_LEGO_FUNCTION("motorSetSpeed", 3, false)\
	ADD_LEGO_FUNCTION("setTrackVehicle", 5, false)\
	ADD_LEGO_FUNCTION("waitMotorToStop", 2, false)\
	ADD_LEGO_FUNCTION("waitMultiMotorsToStop", 5, false)\
	ADD_LEGO_FUNCTION("trackVehicleBackward", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleForward", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleOff", 1, false)\
	ADD_LEGO_FUNCTION("trackVehicleSpinLeft", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleSpinRight", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnLeftForward", 3, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnLeftReverse", 3, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnRightForward", 3, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnRightForward", 3, false);
// Конец макроса

///////////////// Добавим Оси роботов
#define ADD_ROBOT_AXIS(AXIS_NAME, UPPER_VALUE, LOWER_VALUE) \
robot_axis[axis_id] = new AxisData; \
robot_axis[axis_id]->axis_index = axis_id + 1; \
robot_axis[axis_id]->upper_value = UPPER_VALUE; \
robot_axis[axis_id]->lower_value = LOWER_VALUE; \
robot_axis[axis_id]->name = AXIS_NAME; \
axis_id++;
// Конец макроса

#define DEFINE_ALL_AXIS \
ADD_ROBOT_AXIS("locked", 1, 0)\
ADD_ROBOT_AXIS("straight", 2, 0)\
ADD_ROBOT_AXIS("rotation", 2, 0);
// Конец макроса

const char* LegoRobotModule::getUID() {
	return "Lego_Functions_dll";
};

FunctionData** LegoRobotModule::getFunctions(int *count_functions) {
	*count_functions = COUNT_LEGO_FUNCTIONS;
	return lego_functions;
};

LegoRobotModule::LegoRobotModule() {
	srand(time(NULL));
	lego_functions = new FunctionData*[COUNT_LEGO_FUNCTIONS]; 
	regval function_id = 0; 
	DEFINE_ALL_FUNCTIONS 
	
	robot_axis = new AxisData*[COUNT_AXIS];
	regval axis_id = 0;
	DEFINE_ALL_AXIS
};

void LegoRobotModule::destroy() {
	for (int j = 0; j < COUNT_LEGO_FUNCTIONS; ++j) {
		delete lego_functions[j];
	}
	delete[] lego_functions;
	delete this;
};

int LegoRobotModule::init(){
	printf("init from dll\n");

	InitializeCriticalSection(&LRM_cs); // Инициализируем критическую секцию

	CSimpleIniA ini;
	ini.SetMultiKey(true);
	if (ini.LoadFile(PATH_TO_CONFIG) < 0) {
		printf("Can't load '%s' file!\n", PATH_TO_CONFIG);
		return 1;
	}

	CSimpleIniA::TNamesDepend values;
	ini.GetAllValues("connections", "connection", values);

	CSimpleIniA::TNamesDepend::const_iterator ini_value;
	printf("Aviable connections for LEGO robots:\n");

	for (ini_value = values.begin(); ini_value != values.end(); ++ini_value) {
		printf("- %s\n", ini_value->pItem);

		std::string connection(ini_value->pItem);

		System::String^ connection_c = gcnew System::String(connection.c_str());

		lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance(); // Получаем указатель на синглтон объект в переменную singletoneBrick 

		int index_robot = singletoneBrick->createBrick(connection_c); // Возвращает соединение с Лего роботом(через библиотеку Brick)

		try {
			singletoneBrick->connectBrick(index_robot); // Здесь создаем соединение с лего Brick

			LegoRobot *lego_robot = new LegoRobot(index_robot);

			printf("DLL: connected to %s robot %p\n", connection.c_str(), lego_robot);

			aviable_connections[connection] = lego_robot; // Записываем найденного таким образом указатель на объект класса LegoRobot в нашу карту доступных подключений
		}
		catch (...) {
			printf("Cannot connect to robot with connection: %s\n", connection.c_str());

			singletoneBrick->disconnectBrick(index_robot);
		}
	}
	return 0;
};


Robot* LegoRobotModule::robotRequire(){
	EnterCriticalSection(&LRM_cs); // Вход в критическую секцию
	printf("DLL: new robot require\n");

	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second->isAviable) { 
			printf("DLL: finded free robot: %p\n", i->second);

			LegoRobot *lego_robot = i->second; 
			lego_robot->isAviable = false; 

			Robot *robot = lego_robot;
			LeaveCriticalSection(&LRM_cs); 
			return robot;
		};
	};
	LeaveCriticalSection(&LRM_cs); // Выходим из критической секции
	return NULL;
};

void LegoRobotModule::robotFree(Robot *robot){
	EnterCriticalSection(&LRM_cs); // Вход в критическую секцию
	LegoRobot *lego_robot = reinterpret_cast<LegoRobot*>(robot); // Эта функция пзволяет преобразовать указатель Robot *robot к виду казателя на LegoRobot.

	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second == lego_robot) {
			printf("DLL: free robot: %p\n", lego_robot);
			lego_robot->isAviable = true; // Сделали его свободным(осовободили робота)
		}
	}
	LeaveCriticalSection(&LRM_cs); // Выходим из критической секции
};

AxisData **LegoRobotModule::getAxis(int *count_axis){
	(*count_axis) = COUNT_AXIS;
	return robot_axis;
};

void LegoRobotModule::final(){
	lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance(); // Здесь обращение к внешней библиотеке на C#, поэтому есть ^ - Это значит что система будет заниматься выделением под него памяти
	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		singletoneBrick->disconnectBrick(i->second->robot_index); // Дисконнектится с нашим лего роботом, вообще-то походу закрывает вообще возможность дальше работать с роботами.
		delete i->second;
	}
	aviable_connections.clear();
};

void LegoRobot::axisControl(regval axis_index, regval value){
	bool need_send = false;
	if (axis_index == 1) {
		if (
			((is_locked) && (!value))
			|| ((!is_locked) && (value))
			) {
			is_locked = (bool)value;
			need_send = true;
		}
	}
	else {
		need_send = (!is_locked) && (axis_state[axis_index - 1] != value);
	}
	if (need_send) {
		axis_state[axis_index - 1] = value;
		std::string command_for_robot = "H";
		command_for_robot += std::to_string(axis_index);
		command_for_robot += std::to_string(value);
		command_for_robot += "&";
		//send(socket, command_for_robot.c_str(), command_for_robot.length(), 0); // Пока закомментил чтобы не мешалась функция, с ней надо подумать потому что сокета пока нет. точнее он есть но я не уверен в верности формата
		printf("%s\n", command_for_robot.c_str());
	}

};

void LegoRobotModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	colorPrintf = colorPrintf_p;
}

FunctionResult* LegoRobot::executeFunction(regval functionId, regval *args) {
	if ((functionId < 1) || (functionId > 21)) {
		return NULL;
	}
	FunctionResult *rez = new FunctionResult(1);
	bool throw_exception = false;
	switch (functionId) {
	case 1: {
		lego_communication_library::lego_brick::getInstance()->motorBreak(*args,(wchar_t)*(args+1) ); //   2 аргументов // По идее символы можно получить из чисел а значит массива int нам достаточно
		 
		break;
	}
	case 2: {
		rez->result = lego_communication_library::lego_brick::getInstance()->motorGetDirection(*args, (wchar_t)*(args + 1)); //   2 аргументов// возвращает bool
		break;
	}
	case 3: {
		rez->result = lego_communication_library::lego_brick::getInstance()->motorGetTacho(*args, (wchar_t)*(args+1)); //   2 аргументов// возвращает Int
		break;
	}
	case 4: {
		lego_communication_library::lego_brick::getInstance()->motorMoveTo(*args, (wchar_t)*(args+1), *(args + 2), *(args + 3), *(args + 4) ); //   5 аргументов // Естественно надо потом будет все поментяь но теперь все видно хотя бы сколько аргументов ти т.д. и т.п.
		
		break;
	}
	case 5: {
		lego_communication_library::lego_brick::getInstance()->motorOff(*args, (wchar_t)*(args+1) );//   2 аргументов
		
		break;
	}
	case 6: {
		lego_communication_library::lego_brick::getInstance()->motorResetTacho(*args, (wchar_t)*(args+1) ); //   2 аргументов
		
		break;
	}
	case 7: {
		lego_communication_library::lego_brick::getInstance()->motorSetDirection(*args, (wchar_t)*(args+1), *(args + 2) ); //   3 аргументов
		
		break;
	}
	case 8: {
		lego_communication_library::lego_brick::getInstance()->motorSetSpeed(*args, (wchar_t)*(args+1), *(args + 2) ); //   3 аргументов
		
		break;
	}
	case 9: {
		lego_communication_library::lego_brick::getInstance()->setTrackVehicle(*args, (wchar_t)*(args + 1), (wchar_t)*(args + 2), *(args + 3), *(args + 4)); //   5 аргументов
		
		break;
	}
	case 10: {
		lego_communication_library::lego_brick::getInstance()->waitMotorToStop(*args, (wchar_t)*(args+1)); //   2 аргументов
		
		break;
	}
	case 11: {
		lego_communication_library::lego_brick::getInstance()->waitMultiMotorsToStop(*args, (wchar_t)*(args + 1), (wchar_t)*(args + 2), (wchar_t)*(args + 3), (wchar_t)*(args + 4)); // Тут проблема с тем чсто в аргументе должен быть массив символов
		
		break;
	}
	case 12: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleBackward(*args, *(args+1)); //   2 аргументов
		
		break;
	}
	case 13: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleForward(*args, *(args+1));//   2 аргументов
		
		break;
	}
	case 14: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleOff(*args); //   1 аргументов
		//cout << "works@" << endl;
		 
		break;
	}
	case 15: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleSpinLeft(*args, *(args+1)); //   2 аргументов
			
		break;
	}
	case 16: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleSpinRight(*args, *(args + 1)); //   2 аргументов
			
		break;
	}
	case 17: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftForward(*args, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 18: { 
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftReverse(*args, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 19: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(*args, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 20: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(*args, *(args + 1), *(args + 2)); //   3 аргументов
		
		break;
	}
	};// End Switch

	return rez;
};


__declspec(dllexport) RobotModule* getRobotModuleObject() {
	return new LegoRobotModule();
};

