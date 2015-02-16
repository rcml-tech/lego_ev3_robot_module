#include <map>

#include "../rcml_compiler/module.h"
#include "../rcml_compiler/robot_module.h"
#include "../rcml_compiler/globals.h"

#include "../lego_robot_module/lego.h"

char *PATH_TO_CONFIG = "robot_modules/lego_track/config.ini";

LegoRobotModule::LegoRobotModule() {
	regval function_id = 1;
	FunctionData *function;

	ADD_ROBOT_FUNCTION("motor_set_direction", 2, false)
	ADD_ROBOT_FUNCTION("motor_get_direction", 1, false)
	ADD_ROBOT_FUNCTION("motor_reset_position", 1, false)
	ADD_ROBOT_FUNCTION("motor_get_position", 1, false)
	ADD_ROBOT_FUNCTION("motor_move_to", 3, false)
	ADD_ROBOT_FUNCTION("motor_move_and_wait", 3, false)
	ADD_ROBOT_FUNCTION("move_forward", 2, false)
	ADD_ROBOT_FUNCTION("move_back", 2, false)
	ADD_ROBOT_FUNCTION("turn_right", 1, false)
}


FunctionResult* LegoRobot::executeFunction(regval command_index, regval *args) {
	FunctionResult *fr = new FunctionResult(1, 0);
	switch (command_index) {
		case 1: {
			motorSetDirection(robot_index,args);
			break;
		}
		case 2: {
			fr->result = motorGetDirection(robot_index,args);
			break;
		}
		case 3: {
			motorResetTacho(robot_index,args);
			break;
		}
		case 4: {
			fr->result = motorGetTacho(robot_index, args);
			break;
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
			brick->motorMoveTo(robot_index, 'B', args[0], args[1], true);
			brick->motorMoveTo(robot_index, 'C', args[0], args[1], true);
			array <wchar_t> ^motors = { 'B', 'C' };
			brick->waitMultiMotorsToStop(robot_index, motors);
			break;
		}
		case 8: {
			lego_communication_library::lego_brick^ brick = lego_communication_library::lego_brick::getInstance();
			brick->motorMoveTo(robot_index, 'B', args[0], -args[1], true);
			brick->motorMoveTo(robot_index, 'C', args[0], -args[1], true);
			array <wchar_t> ^motors = { 'B', 'C' };
			brick->waitMultiMotorsToStop(robot_index, motors);
			break;
		}
		case 9: {
			lego_communication_library::lego_brick^ brick = lego_communication_library::lego_brick::getInstance();
			brick->motorResetTacho(robot_index, 'B');
			brick->motorResetTacho(robot_index, 'C');
			brick->motorMoveTo(robot_index, 'B', args[0], 1200, true);
			brick->motorMoveTo(robot_index, 'C', args[0], -1200, true);
			array <wchar_t> ^motors = { 'B', 'C' };
			brick->waitMultiMotorsToStop(robot_index, motors);
			break;
		}
		default: break;
	}
	return fr;
}