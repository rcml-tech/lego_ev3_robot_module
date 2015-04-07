/*
* File: lego_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef LEGO_MODULE_H
#define	LEGO_MODULE_H
class LegoRobot : public Robot {
public:
	int robot_index;
	bool isAviable;
	bool is_locked;
	bool is_trackVehicleOn;

	std::vector<variable_value> axis_state;
	LegoRobot(int robot_index) : robot_index(robot_index), isAviable(true), is_trackVehicleOn(false), is_locked(false){}
	FunctionResult* executeFunction(system_value command_index, variable_value *args);
	void axisControl(system_value axis_index, variable_value value);
	~LegoRobot() {};
};
typedef std::map<std::string, LegoRobot*> m_connections;
class LegoRobotModule : public RobotModule{
public:
	CRITICAL_SECTION LRM_cs;
	m_connections aviable_connections;
	FunctionData **lego_functions;
	AxisData **robot_axis;
	colorPrintf_t *colorPrintf;
	LegoRobotModule();
	const char *getUID();
	void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);
	int init();
	FunctionData** getFunctions(unsigned int *count_functions);
	AxisData** getAxis(unsigned int *count_axis);
	Robot* robotRequire();
	void robotFree(Robot *robot);
	void final();
	void destroy();
	~LegoRobotModule() {};
};
#endif	/* LEGO_MODULE_H */
