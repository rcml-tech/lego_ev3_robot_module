#include <stdio.h>
#include <map>

#include "../rcml_compiler/module.h"
#include "../rcml_compiler/robot_module.h"
#include "../rcml_compiler/globals.h"

#include "lego.h"

#include "F:\Office\simpleini\SimpleIni.h"

extern char *PATH_TO_CONFIG;

inline char getMotorLitera(int motor_index) {
	switch (motor_index) {
		case 1: return 'A';
		case 2: return 'B';
		case 3: return 'C';
		case 4: return 'D';
		default: {
			printf("Error: undefined motor index!\n");
			return 0;
		}
	}
}

void motorSetDirection(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		lego_communication_library::lego_brick::getInstance()->motorSetDirection(robot_index, motor_litera, !!args[1]);
	}
}

regval motorGetDirection(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		return lego_communication_library::lego_brick::getInstance()->motorGetDirection(robot_index, motor_litera);
	}
	return 0;
}

void motorResetTacho(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		lego_communication_library::lego_brick::getInstance()->motorResetTacho(robot_index, motor_litera);
	}
}

regval motorGetTacho(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		return lego_communication_library::lego_brick::getInstance()->motorGetTacho(robot_index, motor_litera);
	}
	return 0;
}

void motorMoveTo(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		lego_communication_library::lego_brick::getInstance()->motorMoveTo(robot_index, motor_litera, args[1], args[2], true);
	}
}

void motorMoveAndWait(regval robot_index, regval *args) {
	char motor_litera = getMotorLitera(args[0]);
	if (motor_litera) {
		lego_communication_library::lego_brick^ brick = lego_communication_library::lego_brick::getInstance();
		brick->motorMoveTo(robot_index, motor_litera, args[1], args[2], true);
		brick->waitMotorToStop(robot_index, motor_litera);
	}
}

int LegoRobotModule::init() {
	printf("init from dll\n");

	CSimpleIniA ini;
	ini.SetMultiKey(true);
	if (ini.LoadFile(PATH_TO_CONFIG) < 0) {
		printf("Can't load '%s' file!\n",PATH_TO_CONFIG);
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

		lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance();
		int index_robot = singletoneBrick->createBrick(connection_c);
		
		try {
			singletoneBrick->connectBrick(index_robot);

			LegoRobot *lego_robot = new LegoRobot(index_robot);
			printf("DLL: connected to %s robot %p\n", connection.c_str(), lego_robot);
			aviable_connections[connection] = lego_robot;
		} catch (...) {
			printf("Cannot connect to robot with connection: %s\n", connection.c_str());
			singletoneBrick->disconnectBrick(index_robot);
		}
	}

	return 0;
}

FunctionData* LegoRobotModule::checkAviableFunction(const char *function_name) {
	for(function_list::iterator i = robot_functions.begin(); i != robot_functions.end(); i++) {
		if (!strcmp(i->first.c_str(), function_name)) {
			return i->second;
		}
	}
	return NULL;
}

Robot* LegoRobotModule::robotRequire() {
	printf("DLL: new robot require\n");

	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second->isAviable) {
			printf("DLL: finded free robot: %p\n",i->second);
			
			LegoRobot *lego_robot = i->second;
			lego_robot->isAviable = false;

			Robot *robot = lego_robot;
			return robot;
		}
	}
	return NULL;
}

void LegoRobotModule::robotFree(Robot *robot) {
	LegoRobot *lego_robot = reinterpret_cast<LegoRobot*>(robot);

	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		if (i->second == lego_robot) {
			printf("DLL: free robot: %p\n",lego_robot);
			lego_robot->isAviable = true;
		}
	}
}

void LegoRobotModule::final() {
	lego_communication_library::lego_brick^ singletoneBrick = lego_communication_library::lego_brick::getInstance();
	for(m_connections::iterator i = aviable_connections.begin(); i != aviable_connections.end(); ++i) {
		singletoneBrick->disconnectBrick(i->second->robot_index);
		delete i->second;
	}
	aviable_connections.clear();
}

void LegoRobotModule::destroy() {
	for (function_list::iterator i = robot_functions.begin(); i != robot_functions.end(); i++) {
		delete i->second;
	}
	robot_functions.clear();
}

__declspec(dllexport) RobotModule* getRobotModuleObject() {
	LegoRobotModule *lrm = new LegoRobotModule();
	RobotModule *rm = lrm;
	return rm;
}