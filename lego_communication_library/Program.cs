using System;
using System.Collections;
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

        private Motor getMotorByLitera(Brick<Sensor, Sensor, Sensor, Sensor> brick, char motor)
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
                    throw new Exception("Недопустимая литера мотора!");
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

        public void waitMotorToStop(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            
            Thread.Sleep(500);
            while (brickMotor.IsRunning())
            {
                Thread.Sleep(50);
            }
        }

        public void waitMultiMotorsToStop(int indexBrick, char[] motors)
        {
            Thread.Sleep(500);

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
            Brick<Sensor, Sensor, Sensor, Sensor> brick = getBrickByIndex(indexBrick);
            //brick.Sensor1 = new UltrasonicSensor(UltrasonicMode.Centimeter);
            brick.Sensor1 = new IRSensor(IRMode.Seek);
            Console.WriteLine(brick.Sensor1.ReadAsString());

        }
    }
}