/*
* File: lego_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef LEGO_MODULE_H
#define	LEGO_MODULE_H

class LegoRobot : public Robot {
public:
	bool isAviable;  
	int robot_index; 

	bool is_locked; 
	std::vector<regval> axis_state;

	LegoRobot(int robot_index) : robot_index(robot_index), isAviable(true) {} 

	FunctionResult* executeFunction(regval command_index, regval *args);
	void axisControl(regval axis_index, regval value);  
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

	const char *getUID();
	int init();
	FunctionData** getFunctions(int *count_functions);
	AxisData** getAxis(int *count_axis) ;
	Robot* robotRequire() ;
	void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);
	void robotFree(Robot *robot) ;
	void final();
	void destroy() ;
	LegoRobotModule();
	~LegoRobotModule() {};
};



#endif	/* LEGO_MODULE_H */
