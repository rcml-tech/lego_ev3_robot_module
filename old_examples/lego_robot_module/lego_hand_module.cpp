#include <map>

#include "../robo_parser/module.h"
#include "../robo_parser/robot_module.h"
#include "../robo_parser/globals.h"

#include "lego.h"

char *PATH_TO_CONFIG = "robot_modules/lego_hand/config.ini";

LegoRobotModule::LegoRobotModule() {
	regval function_id = 1;
	FunctionData *function;

	ADD_ROBOT_FUNCTION("motor_set_direction", 2)
	ADD_ROBOT_FUNCTION("motor_get_direction", 1)
	ADD_ROBOT_FUNCTION("motor_reset_position", 1)
	ADD_ROBOT_FUNCTION("motor_get_position", 1)
	ADD_ROBOT_FUNCTION("motor_move_to", 3)
	ADD_ROBOT_FUNCTION("motor_move_and_wait", 3)
	ADD_ROBOT_FUNCTION("open_hand", 1)
	ADD_ROBOT_FUNCTION("close_hand", 1)
}

regval LegoRobot::executeFunction(regval command_index, regval *args) {
	switch (command_index) {
		case 1: {
			motorSetDirection(robot_index,args);
			break;
		}
		case 2: {
			return motorGetDirection(robot_index,args);
		}
		case 3: {
			motorResetTacho(robot_index,args);
			break;
		}
		case 4: {
			return motorGetTacho(robot_index,args);
		}
		case 5: {
			motorMoveTo(robot_index,args);
			break;
		}
		case 6: {
			motorMoveAndWait(robot_index,args);
			break;
		}
		case 7: {
			lego_communication_library::lego_brick^ brick = lego_communication_library::lego_brick::getInstance();
			brick->motorMoveTo(robot_index, 'D', args[0], 550, true); //5550
			brick->waitMotorToStop(robot_index,'D');
			break;
		}
		case 8: {
			lego_communication_library::lego_brick^ brick = lego_communication_library::lego_brick::getInstance();
			brick->motorMoveTo(robot_index, 'D', args[0], 0, true);
			brick->waitMotorToStop(robot_index,'D');
			break;
		}
		default: break;
	}
	return 0;
}

