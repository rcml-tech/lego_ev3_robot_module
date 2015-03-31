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

typedef std::map<std::string, LegoRobot*> m_connections; // Таким образом создадим карту которая строке из config.ini в соответствие ставит адрес объекта LegoRobot. 


class LegoRobotModule : public RobotModule{
public:
	CRITICAL_SECTION LRM_cs; // Критическая секция для LegoRobotModule
	m_connections aviable_connections; 
	FunctionData **lego_functions;
	AxisData **robot_axis; 
	colorPrintf_t *colorPrintf;
	LegoRobot *OccRobot;

	LegoRobotModule();

	const char *getUID();
	void prepare(colorPrintf_t *colorPrintf_p, colorPrintfVA_t *colorPrintfVA_p);
	int init();
	FunctionData** getFunctions(int *count_functions);
	AxisData** getAxis(int *count_axis) ;
	Robot* robotRequire() ;
	void robotFree(Robot *robot) ;
	void final();
	void destroy() ;
	~LegoRobotModule() {};
};

#endif	/* LEGO_MODULE_H */
