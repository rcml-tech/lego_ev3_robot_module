/*
* File: lego_module.cpp
* Author: m79lol, iskinmike
*
*/
#include <windows.h>
#include <time.h>
#include <vector>
#include <map>

#include "module.h"
#include "robot_module.h"
#include "lego_robot_module.h"
#include "SimpleIni.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

const unsigned int COUNT_LEGO_FUNCTIONS = 22;
const unsigned int COUNT_AXIS = 11;


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
ADD_LEGO_FUNCTION("trackVehicleTurnRightForward", 2, false)\
ADD_LEGO_FUNCTION("trackVehicleBrake", 0, false)\
ADD_LEGO_FUNCTION("readSensor", 2, false);

#define ADD_ROBOT_AXIS(AXIS_NAME, UPPER_VALUE, LOWER_VALUE) \
robot_axis[axis_id] = new AxisData; \
robot_axis[axis_id]->axis_index = axis_id + 1; \
robot_axis[axis_id]->upper_value = UPPER_VALUE; \
robot_axis[axis_id]->lower_value = LOWER_VALUE; \
robot_axis[axis_id]->name = AXIS_NAME; \
axis_id++;

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
ADD_ROBOT_AXIS("straight", 100, -100)\
ADD_ROBOT_AXIS("rotation", 100, -100);


const char* LegoRobotModule::getUID() {
	return "Lego_Functions_dll";
};
FunctionData** LegoRobotModule::getFunctions(unsigned int *count_functions) {
	*count_functions = COUNT_LEGO_FUNCTIONS;
	return lego_functions;
};
LegoRobotModule::LegoRobotModule() {
	srand(time(NULL));
	lego_functions = new FunctionData*[COUNT_LEGO_FUNCTIONS];
	system_value function_id = 0;
	DEFINE_ALL_FUNCTIONS
		robot_axis = new AxisData*[COUNT_AXIS];
	system_value axis_id = 0;
	DEFINE_ALL_AXIS
};
char numToMotorLitera(variable_value num){
	if (num == 1.0) {
		return 'A';
	}
	else if (num == 2.0) {
		return 'B';
	}
	else if (num == 3.0) {
		return 'C';
	}
	else if (num == 4.0) {
		return 'D';
	}
	else {
		throw std::exception();
	}
};

inline void isSpeed(variable_value num){
	if ((num < (-100)) || (num > 100)) 
	{
		throw std::exception();
	}
};

inline void isPercent(variable_value num){
	if ((num < 0) || (num > 100))
	{
		throw std::exception();
	}
};

inline void isMotor(variable_value num){
	switch ((int)num)
	{
	case 1:
	case 2:
	case 3:
	case 4:{
		break;
	}
	default:
		throw std::exception();
	}
};

void LegoRobotModule::destroy() {
	for (unsigned int j = 0; j < COUNT_LEGO_FUNCTIONS; ++j) {
		delete lego_functions[j];
	}
	for (unsigned int j = 0; j < COUNT_AXIS; ++j) {
		delete robot_axis[j];
	}
	delete[] lego_functions;
	delete[] robot_axis;
	delete this;
};
int LegoRobotModule::init(){
	InitializeCriticalSection(&LRM_cs); // Инициализируем критическую секцию
	CSimpleIniA ini;
	ini.SetMultiKey(true);

	HMODULE lr_handle;

	lr_handle = GetModuleHandleW(L"lego_module.dll");

	WCHAR DllPath[MAX_PATH] = { 0 };
	
	//GetModuleFileNameW((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
	GetModuleFileNameW(lr_handle, DllPath, _countof(DllPath));
	WCHAR *tmp = wcsrchr(DllPath, L'\\');
	WCHAR ConfigPath[MAX_PATH] = { 0 };
	size_t path_len = tmp - DllPath;
	wcsncpy(ConfigPath, DllPath, path_len); // VStudio Reccomends wcsncpy_s() instead
	wcscat(ConfigPath, L"\\config.ini");

	if (ini.LoadFile(ConfigPath) < 0) {
		printf("Can't load '%s' file!\n", ConfigPath);
		return 1;
	}
	CSimpleIniA::TNamesDepend values;
	ini.GetAllValues("connections", "connection", values);
	CSimpleIniA::TNamesDepend::const_iterator ini_value;
	for ( ini_value = values.begin(); ini_value != values.end(); ++ini_value) {
		printf("- %s\n", ini_value->pItem);
		std::string connection(ini_value->pItem);
		System::String^ connection_c = gcnew System::String(connection.c_str());
		lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance();
		int index_robot = singletoneBrick->createBrick(connection_c);
		//std::cout << "ini works" << std::endl;
		colorPrintf(this, ConsoleColor(ConsoleColor::white), "Attemp to connect: %s\n", connection.c_str());
		try {
			singletoneBrick->connectBrick(index_robot);
			LegoRobot *lego_robot = new LegoRobot(index_robot);
			aviable_connections[connection] = lego_robot;
			colorPrintf(this, ConsoleColor(ConsoleColor::green), "Lego robot connected!\n");
		}
		catch (...) {
			colorPrintf(this, ConsoleColor(ConsoleColor::red), "Lego robot connect FAIL!\n");
			singletoneBrick->disconnectBrick(index_robot);
		}
	}
	return 0;
};
Robot* LegoRobotModule::robotRequire(){
	EnterCriticalSection(&LRM_cs);
	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second->isAviable) {
			LegoRobot *lego_robot = i->second;
			lego_robot->isAviable = false;
			Robot *robot = lego_robot;
			LeaveCriticalSection(&LRM_cs);
			return robot;
		};
	};
	LeaveCriticalSection(&LRM_cs);
	return NULL;
};
void LegoRobotModule::robotFree(Robot *robot){
	EnterCriticalSection(&LRM_cs);
	LegoRobot *lego_robot = reinterpret_cast<LegoRobot*>(robot);
	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second == lego_robot) {
			lego_robot->isAviable = true;
			break;
		}
	}
	LeaveCriticalSection(&LRM_cs);
};
AxisData **LegoRobotModule::getAxis(unsigned int *count_axis){
	(*count_axis) = COUNT_AXIS;
	return robot_axis;
};
void LegoRobotModule::final(){
	lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance();
	for (m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		singletoneBrick->disconnectBrick(i->second->robot_index);
		delete i->second;
	}
	aviable_connections.clear();
};
void LegoRobot::axisControl(system_value axis_index, variable_value value){
	switch (axis_index){
	case 1:{
		is_locked = !!value;
		break;
	};
	case 2:
	case 3:
	case 4:
	case 5:{ // speedMotor A.B.C.D
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->motorSetSpeed(robot_index, (wchar_t)(63 + axis_index), (int)value);
		}
		break;
	};
	case 6:
	case 7:
	case 8:
	case 9:{ // moveMotor A,B,C,D
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->motorMoveTo(robot_index, (wchar_t)(59 + axis_index), 50, (int)value, false); //
		}
		break;
	};
	case 10:{
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->trackVehicleForward(robot_index, (int)value);
		}
		break;
	};
	case 11:{
		if (!is_locked){
			lego_communication_library::lego_brick::getInstance()->trackVehicleSpinRight(robot_index, (int)value);
		}
		break;
	};
	};
};
void LegoRobotModule::prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p) {
	colorPrintf = colorPrintf_p;
}
FunctionResult* LegoRobot::executeFunction(system_value functionId, variable_value *args) {
	if ((functionId < 1) || (functionId > COUNT_LEGO_FUNCTIONS)) {
		return NULL;
	}
	
	variable_value rez=0;
	bool throw_exception = false;
	try {
		switch (functionId) {
		case 1: {
			isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->motorBreak(robot_index, numToMotorLitera(*args));
			
			break;
		}
		case 2: {
			isMotor(*args);
				rez = lego_communication_library::lego_brick::getInstance()->motorGetDirection(robot_index, numToMotorLitera(*args));
			break;
		}
		case 3: {
			isMotor(*args);
				rez = lego_communication_library::lego_brick::getInstance()->motorGetTacho(robot_index, numToMotorLitera(*args));
			break;
		}
		case 4: {
				isSpeed(*(args + 1));
				isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->motorMoveTo(robot_index, numToMotorLitera(*args), *(args + 1), *(args + 2), !!(*(args + 3))); // speed 
			break;
		}
		case 5: {
			isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->motorOff(robot_index, numToMotorLitera(*args));
			 
			break;
		}
		case 6: {
			isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->motorResetTacho(robot_index, numToMotorLitera(*args)); 
			break;
		}
		case 7: {
			isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->motorSetDirection(robot_index, numToMotorLitera(*args), !!(*(args + 1)));
			break;
		}
		case 8: {
			isMotor(*args);
			isSpeed(*(args + 1));
			lego_communication_library::lego_brick::getInstance()->motorSetSpeed(robot_index, numToMotorLitera(*args), *(args + 1)); // speed 
			break;
		}
		case 9: {
			isMotor(*args);
			isMotor(*(args + 1));
			if (*(args + 1) != *(args + 2)) {
				lego_communication_library::lego_brick::getInstance()->setTrackVehicle(robot_index, numToMotorLitera(*args), numToMotorLitera(*(args + 1)), (bool)(*(args + 2)), (bool)(*(args + 3)));
				is_trackVehicleOn = true;
			}
			else{ throw std::exception(); };
			break;
		}
		case 10: {
			isMotor(*args);
				lego_communication_library::lego_brick::getInstance()->waitMotorToStop(robot_index, numToMotorLitera(*args));
			break;
		}
		case 11: {
			lego_communication_library::lego_brick::getInstance()->waitMultiMotorsToStop(robot_index, !!(*args), !!(*(args + 1)), !!(*(args + 2)), !!(*(args + 3)));
			break;
		}
		case 12: {
				isSpeed(*args);
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleBackward(robot_index, *args); // speed
				}
				else{ throw std::exception(); };
			break;
		}
		case 13: {
				isSpeed(*args);
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleForward(robot_index, *args);  // speed
				}
				else{ throw std::exception(); };
			break;
		}
		case 14: {
			if (is_trackVehicleOn){
				lego_communication_library::lego_brick::getInstance()->trackVehicleOff(robot_index);
			}
			else{ throw std::exception(); };
			break;
		}
		case 15: {

			isSpeed(*args);
			if (is_trackVehicleOn){
				lego_communication_library::lego_brick::getInstance()->trackVehicleSpinLeft(robot_index, *args); // speed
			}
			else{ throw std::exception(); };
			break;
		}
		case 16: {
				isSpeed(*args);
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleSpinRight(robot_index, *args); // speed
				}
				else{ throw std::exception(); };
			break;
		}
		case 17: {
				isSpeed(*args);
				isPercent(*(args + 1));
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftForward(robot_index, *args, *(args + 1));  // speed, percent
				}
				else{ throw std::exception(); };
			break;
		}
		case 18: {
				isPercent(*(args + 1));
				isSpeed(*args);
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleTurnLeftReverse(robot_index, *args, *(args + 1));  // speed, percent
				}
				else{ throw std::exception(); };
			break;
		}
		case 19: {
			isSpeed(*args) ; 
			isPercent(*(args + 1));
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(robot_index, *args, *(args + 1));  // speed, percent
				}
				else{ throw std::exception(); };
			break;
		}
		case 20: {
			isSpeed(*args) ; 
			isPercent(*(args + 1));
				if (is_trackVehicleOn){
					lego_communication_library::lego_brick::getInstance()->trackVehicleTurnRightForward(robot_index, *args, *(args + 1));  // speed, percent
				}
				else{ throw std::exception(); };
			break;
		}
		case 21: {
			if (is_trackVehicleOn){
				lego_communication_library::lego_brick::getInstance()->trackVehicleBrake(robot_index);
			}
			else{ throw std::exception(); };
			break;
		}
		case 22:{
			if (lego_communication_library::lego_brick::getInstance()->testSensorMode(robot_index, *args, *(args + 1)) ) {
				rez = lego_communication_library::lego_brick::getInstance()->readSensor(robot_index, *args, *(args + 1));
			}
			else{ throw std::exception(); };
			break;
		}
		};
		return new FunctionResult(1, rez);
	}
	catch (...){
		return new FunctionResult(0);
	};
};

__declspec(dllexport) RobotModule* getRobotModuleObject() {
	return new LegoRobotModule();
};
