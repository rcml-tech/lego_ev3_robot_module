/*
* File: lego_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef LEGO_MODULE_H
#define	LEGO_MODULE_H
class LegoRobot : public Robot {
protected:
	std::string connection;
	bool is_aviable;
	bool is_locked;
	bool is_trackVehicleOn;
	bool allow_dynamic;
	
	std::vector<variable_value> axis_state;
public:
	int robot_index;

	LegoRobot(std::string connection, bool allow_dynamic);
	FunctionResult* executeFunction(system_value command_index, void **args);
	void axisControl(system_value axis_index, variable_value value);
	~LegoRobot() {};

	bool connect();
	void disconnect();

	bool require();
	void free();
};
typedef std::map<std::string, LegoRobot*> m_connections;
class LegoRobotModule : public RobotModule{
protected:
	CRITICAL_SECTION LRM_cs;
	m_connections aviable_connections;
	FunctionData **lego_functions;
	AxisData **robot_axis;
	colorPrintf_t *colorPrintf;
	bool allow_dynamic;
public:
	
	LegoRobotModule();

	//init
	const char *getUID();
	void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);

	//compiler only
	FunctionData** getFunctions(unsigned int *count_functions);
	AxisData** getAxis(unsigned int *count_axis);
	virtual void *writePC(unsigned int *buffer_length);

	//intepreter - devices
	int init();
	Robot* robotRequire();
	void robotFree(Robot *robot);
	void final();

	//intepreter - program
	int startProgram(int uniq_index, void *buffer, unsigned int buffer_length);
	int endProgram(int uniq_index);

	//destructor
	void destroy();
	~LegoRobotModule() {};
};
#endif	/* LEGO_MODULE_H */