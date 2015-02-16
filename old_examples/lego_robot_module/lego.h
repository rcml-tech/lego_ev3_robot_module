#ifndef LEGO_H
#define	LEGO_H

inline char getMotorLitera(int motor_index);
void motorSetDirection(regval robot_index, regval *args); //args - 2
regval motorGetDirection(regval robot_index, regval *args); //args - 1
void motorResetTacho(regval robot_index, regval *args); //args - 1
regval motorGetTacho(regval robot_index, regval *args); //args - 1
void motorMoveTo(regval robot_index, regval *args); //args - 3
void motorMoveAndWait(regval robot_index, regval *args); //args - 3

typedef std::map<std::string, FunctionData*> function_list;

class LegoRobot : public Robot {
    public: 
		bool isAviable;
		int robot_index;
		LegoRobot(int robot_index) : robot_index(robot_index), isAviable(true) {}
		FunctionResult* executeFunction(regval command_index, regval *args);
        ~LegoRobot() {}
};
typedef std::map<std::string, LegoRobot*> m_connections;

class LegoRobotModule : public RobotModule {
	m_connections aviable_connections;
	function_list robot_functions;

	public:
		LegoRobotModule();
		int init();
		FunctionData* checkAviableFunction(const char *function_name);
		Robot* robotRequire();
		void robotFree(Robot *robot);
		void final();
		void destroy();
		~LegoRobotModule() {};
};

extern "C" {
	__declspec(dllexport) RobotModule* getRobotModuleObject();
}

#define ADD_ROBOT_FUNCTION(FUNCTION_NAME, COUNT_PARAMS, GIVE_EXCEPTION) \
	function = new FunctionData(function_id, COUNT_PARAMS, GIVE_EXCEPTION); \
	function_id++; \
	robot_functions[FUNCTION_NAME] = function;

#endif	/* LEGO_H */