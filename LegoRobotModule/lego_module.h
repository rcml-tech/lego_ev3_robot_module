/*
* File: lego_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef LEGO_MODULE_H
#define	LEGO_MODULE_H

class LegoRobot : public Robot {
public:
	bool isAviable;  //ВВели 3 дополнительных свойства // Для занимания и освобождения робота
	int robot_index; //ВВели 2 дополнительных свойства // Для нумерованяи роботов

	bool is_locked; // Для осей
	std::vector<regval> axis_state; // Ввели опять для осей, содержит инфу об их состоянии

	LegoRobot(int robot_index) : robot_index(robot_index), isAviable(true) {} // Это описан конструктор с Инициализацией, такая форма записи иногда лучше и помогает некоторые моменты обойти. Точнее не помню.

	FunctionResult* executeFunction(regval command_index, regval *args);
	void axisControl(regval axis_index, regval value);  // Пока закомментил чтобы не мешалось не знаю нужно ли оно здесь
	~LegoRobot() {};
};

// Здесь сделаем описание типов которые будем использовать
typedef std::map<std::string, LegoRobot*> m_connections; // Таким образом создадим карту которая строке в соответствие ставит адрес объекта LegoRobot. Это хитрая такая штука потому что переопределены типы


class LegoRobotModule : public RobotModule{
public:
	CRITICAL_SECTION LRM_cs; // Критическая секция для LegoRobotModule
	m_connections aviable_connections; //Создаем карту возможных подключений к роботу
	FunctionData **lego_functions;
	AxisData **robot_axis; // Введем пока набор Осей, так же как и набор функций, пока заполним.

	const char *getUID();
	int init();
	FunctionData** getFunctions(int *count_functions);
	AxisData** getAxis(int *count_axis) ;
	Robot* robotRequire() ;
	void robotFree(Robot *robot) ;
	void final();
	void destroy() ;
	LegoRobotModule();
	~LegoRobotModule() {};
};

#endif	/* LEGO_MODULE_H */
