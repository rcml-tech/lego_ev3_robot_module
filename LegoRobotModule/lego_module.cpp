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
int COUNT_LEGO_FUNCTIONS = 20; // Количество задействованных функций Всего их будет 20 не считая 3-х которые реализованы в init, rquireRobot, freeRobot
int COUNT_AXIS = 12;

char *PATH_TO_CONFIG = "robot_modules/lego/config.ini"; // Это путь к ini файлу в котором содержится инфа о том к какому порту подключаться и т.д.


////////// Теперь сделаем поределения макросов чтобы добавлять наши функции лего модуля
// Опишем макрос который будет заполнять наш массив структур с информацией о функциях. Ия функции, Присваивает уникальный индекс, число параметров и дает ли исключение.
#define ADD_LEGO_FUNCTION(FUNCTION_NAME, COUNT_PARAMS, GIVE_EXCEPTION) \
	lego_functions[function_id] = new FunctionData; \
	lego_functions[function_id]->command_index = function_id + 1; \
	lego_functions[function_id]->count_params = COUNT_PARAMS; \
	lego_functions[function_id]->give_exception = GIVE_EXCEPTION; \
	lego_functions[function_id]->name = FUNCTION_NAME; \
	function_id++;
// Конец макроса

// Опишем макросс который все наши функции заполнит/ Добавил Функцию - Увеличивай их число COUNT_MATH_FUNCTIONS. А то удалишь нафиг какой-нить процесс в памяти.
#define DEFINE_ALL_FUNCTIONS \
	ADD_LEGO_FUNCTION("motorBreak", 1, false)\
	ADD_LEGO_FUNCTION("motorGetDirection", 1, false)\
	ADD_LEGO_FUNCTION("motorGetTacho", 1, false)\
	ADD_LEGO_FUNCTION("motorMoveTo", 4, false)\
	ADD_LEGO_FUNCTION("motorOff", 1, false)\
	ADD_LEGO_FUNCTION("motorResetTacho", 1, false)\
	ADD_LEGO_FUNCTION("motorSetDirection", 2, false)\
	ADD_LEGO_FUNCTION("motorSetSpeed", 2, false)\
	ADD_LEGO_FUNCTION("setTrackVehicle", 4, false)\
	ADD_LEGO_FUNCTION("waitMotorToStop", 1, false)\
	ADD_LEGO_FUNCTION("waitMultiMotorsToStop", 4, false)\
	ADD_LEGO_FUNCTION("trackVehicleBackward", 1, false)\
	ADD_LEGO_FUNCTION("trackVehicleForward", 1, false)\
	ADD_LEGO_FUNCTION("trackVehicleOff", 0, false)\
	ADD_LEGO_FUNCTION("trackVehicleSpinLeft", 1, false)\
	ADD_LEGO_FUNCTION("trackVehicleSpinRight", 1, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnLeftForward", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnLeftReverse", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnRightForward", 2, false)\
	ADD_LEGO_FUNCTION("trackVehicleTurnRightForward", 2, false);
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
ADD_ROBOT_AXIS("speedMotorA", 100, -100)\
ADD_ROBOT_AXIS("speedMotorB", 100, -100)\
ADD_ROBOT_AXIS("speedMotorC", 100, -100)\
ADD_ROBOT_AXIS("speedMotorD", 100, -100)\
ADD_ROBOT_AXIS("moveMotorA", 1000, -1000)\
ADD_ROBOT_AXIS("moveMotorB", 1000, -1000)\
ADD_ROBOT_AXIS("moveMotorC", 1000, -1000)\
ADD_ROBOT_AXIS("moveMotorD", 1000, -1000)\
ADD_ROBOT_AXIS("straight", -100, 100)\
ADD_ROBOT_AXIS("rotation", -100, 100);
// Конец макроса

// Для начала самое простое - getUID
const char* LegoRobotModule::getUID() {
	return "Lego_Functions_dll";
};

// возвращает указатель на указатель на массив наших функций.
FunctionData** LegoRobotModule::getFunctions(int *count_functions) {
	*count_functions = COUNT_LEGO_FUNCTIONS;
	return lego_functions;
};

// Описание конструктора нашего класса
LegoRobotModule::LegoRobotModule() {
	srand(time(NULL));
	lego_functions = new FunctionData*[COUNT_LEGO_FUNCTIONS]; // Создание нового массива наших функций
	regval function_id = 0; // задаем начальный индекс функций
	DEFINE_ALL_FUNCTIONS // Макрос создания наших функций
	
	// Определим еще и оси
	robot_axis = new AxisData*[COUNT_AXIS];
	regval axis_id = 0;
	DEFINE_ALL_AXIS
};


char numToMotorLitera(regval num){
	switch (num){
		case 1:{
			return 'A';
		};
		case 2:{
			return 'B';
		};
		case 3:{
			return 'C';
		};
		case 4:{	
			return 'A';
		};
	};

};

// Описание Деструктора
void LegoRobotModule::destroy() {
	for (int j = 0; j < COUNT_LEGO_FUNCTIONS; ++j) {
		delete lego_functions[j];
	}
	delete[] lego_functions;
	delete this;
};

// Метод инициализации !!!!!!!!!!!!!!!!!!!!!!  
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
			cout << index_robot << endl;
			LegoRobot *lego_robot = new LegoRobot(index_robot);

			printf("DLL: connected to %s robot %p\n", connection.c_str(), lego_robot);
			OccRobot = lego_robot;
			aviable_connections[connection] = lego_robot; // Записываем найденного таким образом указатель на объект класса LegoRobot в нашу карту доступных подключений
		}
		catch (...) {
			printf("Cannot connect to robot with connection: %s\n", connection.c_str());

			singletoneBrick->disconnectBrick(index_robot);
		}
	}
	return 0;
};

// Метода Запроса робота
Robot* LegoRobotModule::robotRequire(){
	EnterCriticalSection(&LRM_cs); // Вход в критическую секцию
	printf("DLL: new robot require\n");

	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second->isAviable) { // Похоже это такая хитрая штука что карта avaliable_connections воспринимается как пара значений pair  и поэтому можно обратиться к второй части пары через ->second-> и это у нас будет объект класса LegoRobot у него будет свойство IsAvaliable
			printf("DLL: finded free robot: %p\n", i->second);

			LegoRobot *lego_robot = i->second; // Здесь мы нашему указателю на объект передаем адрес свободного объекта типа LegoRobot
			lego_robot->isAviable = false; // И теперь мы его застолбили

			Robot *robot = lego_robot; // Теперь указателю на объект класса Robot присваиваем адрес оъекта класса LegoRobot
			LeaveCriticalSection(&LRM_cs); // Выходим из критической секции
			return robot;
		};
	};
	LeaveCriticalSection(&LRM_cs); // Выходим из критической секции
	return NULL;
};

// Метод освобождения робота
void LegoRobotModule::robotFree(Robot *robot){
	// На всякий случай сделаем выход из критической секции
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
	cout << "AxisGo!" << endl;
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

// Метод Управления осями, относится к LegoRobot
void LegoRobot::axisControl(regval axis_index, regval value){

	switch (axis_index){
	case 1:{
		is_locked = (bool)value;
		break;
	};

	case 2:
	case 3:
	case 4:
	case 5:{ // speedMotorA .B.C.D
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->motorSetSpeed(robot_index, (wchar_t)(63+axis_index), value);
		}// End If
		else {
			cout << "robot is locked!" << endl;
		} // End else
		break;
	};
	case 6:
	case 7:
	case 8:
	case 9:{ // moveMotorA ,B,C,D                                           // (int indexBrick, char motor, sbyte speed, int position, bool brake)       
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->motorMoveTo(robot_index, (wchar_t)(59+axis_index), 1, value, false); // 
		}
		else {
			cout << "robot is locked!" << endl;
		} // End else
		break;
	};
	case 10:{
		if (!is_locked){
			if (value > 0){ // в принципе работать должно, оставим пока так
				lego_communication_library::lego_brick::getInstance()->trackVehicleForward(robot_index, value);
			}
			else{
				lego_communication_library::lego_brick::getInstance()->trackVehicleBackward(robot_index, value);
			}; //end If

			if (value == 0) {
				lego_communication_library::lego_brick::getInstance()->trackVehicleOff(robot_index); //   1 аргументов
			};
		} // End If
		else {
			cout << "robot is locked!" << endl;
		} // End else
		break;
	};
	case 11:{
		if (!is_locked){
			if (value > 0){ // в принципе работать должно, оставим пока так
				lego_communication_library::lego_brick::getInstance()->trackVehicleSpinRight(robot_index, value);
			}
			else{
				lego_communication_library::lego_brick::getInstance()->trackVehicleSpinLeft(robot_index, value);
			}; //end If

			if (value == 0) {
				lego_communication_library::lego_brick::getInstance()->trackVehicleOff(robot_index); //   1 аргументов
			};
		} // End If
		else {
			cout << "robot is locked!" << endl;
		} // End else
		break;
	};

	}; // end Switch
};

void LegoRobotModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	colorPrintf = colorPrintf_p;
}

//Теперь самое интересное - executeFunction
// Здесь в зависимости от введенного пара метра через оператор switch будем выполнять разные функции из проекта Lego_communication
FunctionResult* LegoRobot::executeFunction(regval functionId, regval *args) {
	if ((functionId < 1) || (functionId > 21)) {
		return NULL;
	}
	FunctionResult *rez = new FunctionResult(1);
	bool throw_exception = false;
	switch (functionId) {
	case 1: {

		lego_communication_library::lego_brick::getInstance()->motorBreak(robot_index, numToMotorLitera(*args)); //   2 аргументов // По идее символы можно получить из чисел а значит массива int нам достаточно
		 
		break;
	}
	case 2: {
		rez->result = lego_communication_library::lego_brick::getInstance()->motorGetDirection(robot_index, numToMotorLitera(*args)); //   2 аргументов// возвращает bool
		break;
	}
	case 3: {
		rez->result = lego_communication_library::lego_brick::getInstance()->motorGetTacho(robot_index, numToMotorLitera(*args)); //   2 аргументов// возвращает Int
		break;
	}
	case 4: {
		lego_communication_library::lego_brick::getInstance()->motorMoveTo(robot_index, numToMotorLitera(*args), *(args + 1), *(args + 2), *(args + 3) ); //   5 аргументов // Естественно надо потом будет все поментяь но теперь все видно хотя бы сколько аргументов ти т.д. и т.п.
		break;
	}
	case 5: {

		lego_communication_library::lego_brick::getInstance()->motorOff(robot_index, numToMotorLitera(*args));//   2 аргументов
		
		break;
	}
	case 6: {
		lego_communication_library::lego_brick::getInstance()->motorResetTacho(robot_index, numToMotorLitera(*args) ); //   2 аргументов
		
		break;
	}
	case 7: {
		lego_communication_library::lego_brick::getInstance()->motorSetDirection(robot_index, numToMotorLitera(*args), *(args + 1) ); //   3 аргументов
		
		break;
	}
	case 8: {
		lego_communication_library::lego_brick::getInstance()->motorSetSpeed(robot_index, numToMotorLitera(*args), *(args + 1)); //   3 аргументов
		
		break;
	}
	case 9: {
		lego_communication_library::lego_brick::getInstance()->setTrackVehicle(robot_index, numToMotorLitera(*args), numToMotorLitera(*(args + 1)), *(args + 2), *(args + 3)); //   5 аргументов
		
		break;
	}
	case 10: {
		lego_communication_library::lego_brick::getInstance()->waitMotorToStop(robot_index, numToMotorLitera(*args)); //   2 аргументов
		
		break;
	}
	case 11: {
		lego_communication_library::lego_brick::getInstance()->waitMultiMotorsToStop(robot_index, numToMotorLitera(*args), numToMotorLitera(*(args + 1)), numToMotorLitera(*(args + 2)), numToMotorLitera(*(args + 3))); // Тут проблема с тем чсто в аргументе должен быть массив символов
		
		break;
	}
	case 12: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleBackward(robot_index,  *args); //   2 аргументов
		
		break;
	}
	case 13: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleForward(robot_index,  *args);//   2 аргументов
		
		break;
	}
	case 14: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleOff(robot_index); //   1 аргументов
		 
		break;
	}
	case 15: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleSpinLeft(robot_index,  *args); //   2 аргументов
			
		break;
	}
	case 16: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleSpinRight(robot_index, *args); //   2 аргументов
			
		break;
	}
	case 17: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftForward(robot_index, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 18: { 
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftReverse(robot_index, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 19: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(robot_index, *(args + 1), *(args + 2)); //   3 аргументов
			
		break;
	}
	case 20: {
		lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(robot_index, *(args + 1), *(args + 2)); //   3 аргументов
		
		break;
	}
	};// End Switch

	return rez;
};


__declspec(dllexport) RobotModule* getRobotModuleObject() {
	return new LegoRobotModule();
};

