using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;

using MonoBrick.EV3;//use this to run the example on the EV3

//using MonoBrick.NXT;//use this to run the example on the NXT

namespace lego_communication_library
{
    public class lego_brick
    {
        private OrderedDictionary connectedBricks;
        private OrderedDictionary connectedPoints;

        //IDictionary<string, int> dict = new Dictionary<string, int>();
        private int iteratorConnectIndex;
        private static lego_brick instance;

        private Motor getMotorByLitera(Brick<Sensor, Sensor, Sensor, Sensor> brick, char motor) // вместо литер числа
        {
            switch (motor)
            {
                case 'A':
                    return brick.MotorA;
                case 'B': 
                    return brick.MotorB;
                case 'C':
                    return brick.MotorC;
                case 'D':
                    return brick.MotorD;
                default:
                    throw new Exception("Недопустимая литера мотора! " );
            }
        }

        private Brick<Sensor, Sensor, Sensor, Sensor> getBrickByIndex(int indexBrick) 
        {
            if (!connectedBricks.Contains((Object) indexBrick))
            {
                throw new Exception("Блок с запрашиваемым индексом не подключен!");
            }

            return (Brick<Sensor, Sensor, Sensor, Sensor>) connectedBricks[(Object) indexBrick];
        }

        private Motor getMotorByIndexBreakAndLitera(int indexBrick, char motor)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick = getBrickByIndex(indexBrick);
            Motor brickMotor = getMotorByLitera(brick, motor);
            return brickMotor;
        }

        private Vehicle getVehicleByIndexBrick(int indexBrick)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick = getBrickByIndex(indexBrick);
            return brick.Vehicle;
        }

        private lego_brick()
        {
            connectedBricks = new OrderedDictionary();
            connectedPoints = new OrderedDictionary();
            iteratorConnectIndex = 0;
        }

        public static lego_brick getInstance()
        {
            if (instance == null) {
                instance = new lego_brick();
            }
            return instance;
        }

        public int createBrick(string connectPoint) 
        {
            if (connectedPoints.Contains(connectPoint))
            {
                return (int) connectedPoints[connectPoint];
            }

            iteratorConnectIndex++;
            connectedBricks.Add((Object) iteratorConnectIndex, new Brick<Sensor, Sensor, Sensor, Sensor>(connectPoint));
            connectedPoints.Add(connectPoint, iteratorConnectIndex);

            return iteratorConnectIndex;
        }

        public void connectBrick(int indexBrick)
        {
            getBrickByIndex(indexBrick).Connection.Open();
        }

        public void disconnectBrick(int indexBrick)
        {
            getBrickByIndex(indexBrick).Connection.Close();
        }

        public void motorSetSpeed(int indexBrick, char motor , sbyte speed)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.On(speed);
        }

        public void motorOff(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.Off();
        }

        public void motorBreak(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.Brake();
        }

        public void motorSetDirection(int indexBrick, char motor, bool isForward)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.Reverse = isForward;
        }

        public bool motorGetDirection(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            return brickMotor.Reverse;
        }

        public void motorResetTacho(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.ResetTacho();
        }

        public int motorGetTacho(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            return brickMotor.GetTachoCount();
        }

        public void motorMoveTo(int indexBrick, char motor, sbyte speed, int position, bool brake)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.MoveTo((byte)speed, position, brake);
        }

        public void setTrackVehicle(int indexBrick, char leftMotor, char rightMotor, bool leftReverse, bool rightReverse)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            switch (leftMotor)
            {
                case 'A':
                    vehicle.LeftPort = MotorPort.OutA;
                    break;
                case 'B':
                    vehicle.LeftPort = MotorPort.OutB;
                    break;
                case 'C':
                    vehicle.LeftPort = MotorPort.OutC;
                    break;
                case 'D':
                    vehicle.LeftPort = MotorPort.OutD;
                    break;
            }

            switch (rightMotor)
            {
                case 'A':
                    vehicle.RightPort = MotorPort.OutA;
                    break;
                case 'B':
                    vehicle.RightPort = MotorPort.OutB;
                    break;
                case 'C':
                    vehicle.RightPort = MotorPort.OutC;
                    break;
                case 'D':
                    vehicle.RightPort = MotorPort.OutD;
                    break;
            }

            vehicle.ReverseLeft = leftReverse;
            vehicle.ReverseRight = rightReverse;
        }

        public void trackVehicleForward(int indexBrick, sbyte speed)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.Forward(speed); 
        }

        public void trackVehicleBackward(int indexBrick, sbyte speed)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.Backward(speed);
        }

        public void trackVehicleSpinRight(int indexBrick, sbyte speed)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.SpinRight(speed);
        }


        public void trackVehicleSpinLeft(int indexBrick, sbyte speed)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.SpinLeft(speed);
        }

        public void trackVehicleTurnLeftForward(int indexBrick, sbyte speed, sbyte percent)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.TurnLeftForward(speed, percent);
        }

        public void trackVehicleTurnRightForward(int indexBrick, sbyte speed, sbyte percent)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.TurnRightForward(speed, percent);
        }

        public void trackVehicleTurnLeftReverse(int indexBrick, sbyte speed, sbyte percent)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.TurnLeftReverse(speed, percent);
        }

        public void trackVehicleTurnRightReverse(int indexBrick, sbyte speed, sbyte percent)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.TurnRightReverse(speed, percent);
        }

        public void trackVehicleOff(int indexBrick)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.Off();
        }

        public void trackVehicleBrake(int indexBrick)
        {
            Vehicle vehicle = getVehicleByIndexBrick(indexBrick);
            vehicle.Brake();
        }

        public void waitMotorToStop(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            
            Thread.Sleep(500);
            while (brickMotor.IsRunning())
            {
                Thread.Sleep(50);
            }
        }

        //public void waitMultiMotorsToStop(int indexBrick, char[] motors)
        public void waitMultiMotorsToStop(int indexBrick, char MotorA, char MotorB, char MotorC, char MotorD){
        
            Thread.Sleep(500);

            //char[] motors = new char[0];

            List<char> motors = new  List<char> ();

            if (MotorA == 1) { motors.Add('A'); };
            if (MotorB == 1) { motors.Add('B'); };
            if (MotorC == 1) { motors.Add('C'); };
            if (MotorD == 1) { motors.Add('D'); };

            bool allMotorsStopped;
            do {
                allMotorsStopped = true;
                Thread.Sleep(50);
                foreach (char motor in motors)
                {
                    Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
                
                    if (brickMotor.IsRunning())
                    {
                        allMotorsStopped = false;
                    }
                }
            } while (!allMotorsStopped);
        }

        public void testSensor(int indexBrick)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick = getBrickByIndex(indexBrick); // Записали в переменную типа Brick наш созданный brick
            //brick.Sensor1 = new UltrasonicSensor(UltrasonicMode.Centimeter);
            brick.Sensor1 = new IRSensor(IRMode.Seek);  // Создаем в нашем Brick сенсор. Это свойство/поле Sensor1.  Создаем объект Инфракрасный сенсор.
            Console.WriteLine(brick.Sensor1.ReadAsString());
            //brick.Sensor2 = new IRSensor(IRMode.Proximity);
            //Console.WriteLine(brick.Sensor2.ReadAsString());
        }

        private Sensor getSensorObject(SensorType typeIndexSensor, int mode) 
        {
            switch (typeIndexSensor) {
                case SensorType.UltraSonic:
                    {
                        switch (mode) {
                            case 1: { return new UltrasonicSensor(UltrasonicMode.Centimeter); }
                            case 2: { return new UltrasonicSensor(UltrasonicMode.Inch); }
                            case 3: { return new UltrasonicSensor(UltrasonicMode.Listen); }
                            default: { return null; };
                        }
                    }
                case SensorType.IR:
                    {
                        switch (mode)
                        {
                            case 1: { return new IRSensor(IRMode.Proximity); }
                            case 2: { return new IRSensor(IRMode.Remote); }
                            case 3: { return new IRSensor(IRMode.Seek); }
                            default: { return null; };  
                        }
                    }
                case SensorType.Color:
                    {
                        switch (mode)
                        {
                            case 1: { return new ColorSensor(ColorMode.Ambient); }
                            case 2: { return new ColorSensor(ColorMode.Color); }
                            case 3: { return new ColorSensor(ColorMode.Raw); }
                            default: { return null; };
                        }
                    }
                case SensorType.Gyro:
                    {
                        switch (mode)
                        {
                            case 1: { return new GyroSensor(GyroMode.Angle); }
                            case 2: { return new GyroSensor(GyroMode.AngularVelocity); }
                            case 3: { return new GyroSensor(GyroMode.Angle); }
                            default: { return null; };

                        }
                       
                    }
                case SensorType.Touch:
                    {
                        switch (mode)
                        {
                            case 1: { return new TouchSensor(TouchMode.Boolean); }
                            case 2: { return new TouchSensor(TouchMode.Count); }
                            case 3: { return new TouchSensor(TouchMode.Boolean); }
                            default: { return null; };
                        }
                    }
                default: { return null; };
            }; // End Switch
        } // End Func

        protected double resultMod(string str) {
            switch (str) {
                case "Black": { return 1; }
                case "White": { return 2; }
                case "Blue": { return 3; }
                case "Green": { return 4; }
                case "Yellow": { return 5; }
                case "Red": { return 6; }
                default:
                    {
                        string temps;
                        temps = str;
                        int tempi = temps.Length;

                        if (temps.IndexOf(' ') > 0)
                        {
                            temps = temps.Remove(temps.IndexOf(' '), tempi - temps.IndexOf(' '));
                        }
                        return Convert.ToDouble(temps);
                    }
            }
        }

        private bool isURMode(int mod) {
            switch (mod) {
               case 1:
               case 2:
               case 3:
                   {return true;}
                default :{return false;}
            }
        }
        private bool isGTMode(int mod) {
            switch (mod) {
               case 1:
               case 2:
                   {return true;}
                default : {return false;}
            }
        }
        private bool isColorMode(int mod) {
            switch (mod) {
               case 1:
               case 2:
               case 3:
               case 4:
                   {return true;}
                default: {return false;}
            }
        }

        private bool isSensorMode(SensorType senst,int mode )
        {
            switch (senst){
               case SensorType.UltraSonic:
               case SensorType.IR:        
                   { return isURMode(mode);}
               case SensorType.Gyro:
               case SensorType.Touch:
                   {return isGTMode(mode); }
               case SensorType.Color:
                   {return isColorMode(mode);}
                default: {return false;}
             }
        }
        // Returns False if mode is wrong
        public bool testSensorMode(int indexBrick, int indexSensor, int mode) // Пусть возвращает False если ввели неверное значение
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            switch (indexSensor)
            {
                case 1:
                    {
                        return isSensorMode(brick2.Sensor1.GetSensorType(),mode);
                    }
                case 2:
                    {
                        return isSensorMode(brick2.Sensor2.GetSensorType(),mode);
                    }
                case 3:
                    {
                        return isSensorMode(brick2.Sensor3.GetSensorType(),mode);
                    }
                case 4:
                    {
                        return isSensorMode(brick2.Sensor4.GetSensorType(), mode);
                    }
                default: { return false; }
            };
        } // end test SensorMode

        public double readSensor(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double ret = 0;

            switch (indexSensor)
            {
                case 1:
                    {   
                        brick2.Sensor1 = getSensorObject(brick2.Sensor1.GetSensorType(), mode);
                        ret = resultMod(brick2.Sensor1.ReadAsString());
                        break;
                    }
                case 2: 
                    {
                        brick2.Sensor2 = getSensorObject(brick2.Sensor2.GetSensorType(), mode);
                        ret = resultMod(brick2.Sensor2.ReadAsString());
                        break;
                    }
                case 3: 
                    {
                        brick2.Sensor3 = getSensorObject(brick2.Sensor3.GetSensorType(), mode);
                        ret = resultMod(brick2.Sensor3.ReadAsString());
                        break;
                    }
                case 4: 
                    {
                        brick2.Sensor4 = getSensorObject(brick2.Sensor4.GetSensorType(), mode);
                        ret = resultMod(brick2.Sensor4.ReadAsString());
                        break;
                    }
                default: { break; }
            };
            return ret;
        } // End readSensor
    }
}
