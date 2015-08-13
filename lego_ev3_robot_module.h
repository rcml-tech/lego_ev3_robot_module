/*
* File: lego_module.h
* Author: m79lol, iskinmike
*
*/
#ifndef LEGO_MODULE_H
#define LEGO_MODULE_H
class LegoRobot : public Robot {
 protected:
  std::string connection;
  bool is_aviable;
  bool is_locked;
  bool is_trackVehicleOn;
  bool allow_dynamic;

  std::vector<variable_value> axis_state;

  char *uniq_name;
  colorPrintfRobotVA_t *colorPrintf_p;

 public:
  int robot_index;

  LegoRobot(std::string connection, bool allow_dynamic);
  void prepare(colorPrintfRobot_t *colorPrintf_p,
               colorPrintfRobotVA_t *colorPrintfVA_p);
  FunctionResult *executeFunction(CommandMode mode, system_value command_index,
                                  void **args);
  void axisControl(system_value axis_index, variable_value value);
  ~LegoRobot();

  bool connect();
  void disconnect();

  bool require();
  void free();

  void colorPrintf(ConsoleColor colors, const char *mask, ...);
};
typedef std::map<std::string, LegoRobot *> m_connections;
class LegoRobotModule : public RobotModule {
 protected:
  CRITICAL_SECTION LRM_cs;
  m_connections aviable_connections;
  FunctionData **lego_functions;
  AxisData **robot_axis;
  colorPrintfModuleVA_t *colorPrintf_p;
  bool allow_dynamic;

 public:
  LegoRobotModule();

  // init
  const char *getUID();
  void prepare(colorPrintfModule_t *colorPrintf_p,
               colorPrintfModuleVA_t *colorPrintfVA_p);

  // compiler only
  FunctionData **getFunctions(unsigned int *count_functions);
  AxisData **getAxis(unsigned int *count_axis);
  virtual void *writePC(unsigned int *buffer_length);

  // intepreter - devices
  int init();
  Robot *robotRequire();
  void robotFree(Robot *robot);
  void final();

  // intepreter - program & lib
  void readPC(void *buffer, unsigned int buffer_length);

  // intepreter - program
  int startProgram(int uniq_index);
  int endProgram(int uniq_index);

  // destructor
  void destroy();
  ~LegoRobotModule(){};

  void colorPrintf(ConsoleColor colors, const char *mask, ...);
};
#endif /* LEGO_MODULE_H */