using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;

using MonoBrick.EV3;

namespace lego_communication_library
{

    public class EV3_brick
    {
        private OrderedDictionary connectedBricks;
        private OrderedDictionary connectedPoints;

        private int iteratorConnectIndex;
        private static EV3_brick instance;

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
                    throw new Exception("Wrong motor litera! ");
            }
        }

        private Brick<Sensor, Sensor, Sensor, Sensor> getBrickByIndex(int indexBrick)
        {
            if (!connectedBricks.Contains((Object)indexBrick))
            {
                throw new Exception("Brick not connected!");
            }

            return (Brick<Sensor, Sensor, Sensor, Sensor>)connectedBricks[(Object)indexBrick];
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

        private EV3_brick()
        {
            connectedBricks = new OrderedDictionary();
            connectedPoints = new OrderedDictionary();
            iteratorConnectIndex = 0;
        }

        public static EV3_brick getInstance()
        {
            if (instance == null)
            {
                instance = new EV3_brick();
            }
            return instance;
        }

        public int createBrick(string connectPoint)
        {
            if (connectedPoints.Contains(connectPoint))
            {
                return (int)connectedPoints[connectPoint];
            }

            iteratorConnectIndex++;
            connectedBricks.Add((Object)iteratorConnectIndex, new Brick<Sensor, Sensor, Sensor, Sensor>(connectPoint));
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

        public void motorSetSpeed(int indexBrick, char motor, sbyte speed)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.On(speed);
        }

        public void motorOff(int indexBrick, char motor)
        {
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            brickMotor.Off();
        }

        public void motorBrake(int indexBrick, char motor)
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

            while (brickMotor.IsRunning())
            {
                Thread.Sleep(50);
            }
        }

        public void waitMultiMotorsToStop(int indexBrick, bool MotorA, bool MotorB, bool MotorC, bool MotorD)
        {
            List<char> motors = new List<char>();

            if (MotorA) { motors.Add('A'); };
            if (MotorB) { motors.Add('B'); };
            if (MotorC) { motors.Add('C'); };
            if (MotorD) { motors.Add('D'); };

            bool allMotorsStopped;
            do
            {
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

        private Sensor getSensorObject(SensorType typeIndexSensor, int mode)
        {
            switch (typeIndexSensor)
            {
                case SensorType.UltraSonic:
                case SensorType.NXTUltraSonic:
                    {
                        switch (mode)
                        {
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
                case SensorType.NXTColor:
                    {
                        switch (mode)
                        {
                            case 1: { return new ColorSensor(ColorMode.Ambient); }
                            case 2: { return new ColorSensor(ColorMode.Color); }
                            case 3: { return new ColorSensor(ColorMode.Raw); }
                            case 4: { return new ColorSensor(ColorMode.Reflection); }
                            default: { return null; };
                        }
                    }
                case SensorType.Gyro:
                    {
                        switch (mode)
                        {
                            case 1: { return new GyroSensor(GyroMode.Angle); }
                            case 2: { return new GyroSensor(GyroMode.AngularVelocity); }
                            default: { return null; };

                        }

                    }
                case SensorType.NXTTouch:
                case SensorType.Touch:
                    {
                        switch (mode)
                        {
                            case 1: { return new TouchSensor(TouchMode.Boolean); }
                            case 2: { return new TouchSensor(TouchMode.Count); }
                            default: { return null; };
                        }
                    }
                case SensorType.NXTTemperature:
                    {
                        switch (mode)
                        {
                            case 1: { return new TemperatureSensor(TemperatureMode.Celcius); }
                            case 2: { return new TemperatureSensor(TemperatureMode.Fahrenheit); }
                            default: { return null; };
                        }
                    }
                case SensorType.NXTLight:
                    {
                        switch (mode)
                        {
                            case 1: { return new LightSensor(LightMode.Ambient); }
                            case 2: { return new LightSensor(LightMode.Relection); }
                            default: { return null; };
                        }
                    }
                case SensorType.NXTSound:
                    {
                        switch (mode)
                        {
                            case 1: { return new SoundSensor(SoundMode.SoundDB); }
                            case 2: { return new SoundSensor(SoundMode.SoundDBA); }
                            default: { return null; };
                        }
                    }
                default: { return null; };
            }; // End Switch
        } // End Func

        private bool isURMode(int mod)
        {
            switch (mod)
            {
                case 1:
                case 2:
                case 3:
                    { return true; }
                default: { return false; }
            }
        }
        private bool isGTMode(int mod)
        {
            switch (mod)
            {
                case 1:
                case 2:
                    { return true; }
                default: { return false; }
            }
        }
        private bool isColorMode(int mod)
        {
            switch (mod)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                    { return true; }
                default: { return false; }
            }
        }

        private bool isSensorMode(SensorType senst, int mode)
        {
            switch (senst)
            {
                case SensorType.NXTUltraSonic:
                case SensorType.UltraSonic:
                case SensorType.IR:
                    { return isURMode(mode); }
                case SensorType.Gyro:
                case SensorType.Touch:
                case SensorType.NXTTouch:
                case SensorType.NXTTemperature:
                case SensorType.NXTLight:
                case SensorType.NXTSound:
                    { return isGTMode(mode); }
                case SensorType.Color:
                case SensorType.NXTColor:
                    { return isColorMode(mode); }
                default: { return false; }
            }
        }
        // Returns False if mode is wrong
        public bool testSensorMode(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            switch (indexSensor)
            {
                case 1:
                    {
                        return isSensorMode(brick2.Sensor1.GetSensorType(), mode);
                    }
                case 2:
                    {
                        return isSensorMode(brick2.Sensor2.GetSensorType(), mode);
                    }
                case 3:
                    {
                        return isSensorMode(brick2.Sensor3.GetSensorType(), mode);
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
                        ret = brick2.Sensor1.ReadAsFloat();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getSensorObject(brick2.Sensor2.GetSensorType(), mode);
                        ret = brick2.Sensor2.ReadAsFloat();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getSensorObject(brick2.Sensor3.GetSensorType(), mode);
                        ret = brick2.Sensor3.ReadAsFloat();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getSensorObject(brick2.Sensor4.GetSensorType(), mode);
                        ret = brick2.Sensor4.ReadAsFloat();
                        break;
                    }
                default: { break; }
            };
            return ret;
        } // End readSensor


        public bool isMotorRunning(int indexBrick, char motor, bool is_Sleep)
        {
            if (is_Sleep)
            {
                Thread.Sleep(200);
            }
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            return brickMotor.IsRunning();
        }

        public sbyte getMotorSpeed(int indexBrick, char motor, bool is_Sleep)
        {
            if (is_Sleep)
            {
                Thread.Sleep(200);
            }
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            return brickMotor.GetSpeed();
        }
    }
}