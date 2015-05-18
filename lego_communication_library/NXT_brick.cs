using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Threading;

using MonoBrick.NXT;


namespace lego_communication_library
{

    public class NXT_brick 
    {
        private OrderedDictionary connectedBricks;
        private OrderedDictionary connectedPoints;

        private int iteratorConnectIndex;
        private static NXT_brick instance;


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

        private NXT_brick()
        {
            connectedBricks = new OrderedDictionary();
            connectedPoints = new OrderedDictionary();
            iteratorConnectIndex = 0;
        }

        public static NXT_brick getInstance()
        {
            if (instance == null) {
                instance = new NXT_brick();
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

        public void waitMultiMotorsToStop(int indexBrick, bool MotorA, bool MotorB, bool MotorC)
        {
            List<char> motors = new List<char>();

            if (MotorA) { motors.Add('A'); };
            if (MotorB) { motors.Add('B'); };
            if (MotorC) { motors.Add('C'); };


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


        // NXT SENSORS
        // Touch
        private TouchSensor getNXTTouchSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new TouchSensor(SensorMode.Bool); }
                default:
                    { return new TouchSensor(SensorMode.Raw); }
            }
        }
        public double readNXTTouch(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
            switch (indexSensor) {
                case 1:
                    {
                        brick2.Sensor1 = getNXTTouchSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getNXTTouchSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getNXTTouchSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getNXTTouchSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Color
        private NXTColorSensor getNXTColorSensor(int mode)
        {
            switch (mode)
            {
                default:
                    { return new NXTColorSensor(ColorMode.Full, SensorMode.Raw); }
                case 2:
                    { return new NXTColorSensor(ColorMode.Red, SensorMode.Percent); }
                case 3:
                    { return new NXTColorSensor(ColorMode.Green, SensorMode.Percent); }
                case 4:
                    { return new NXTColorSensor(ColorMode.Blue, SensorMode.Percent); }
                case 5:
                    { return new NXTColorSensor(ColorMode.None, SensorMode.Percent); }
            }
        }
        public double readNXTColor(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getNXTColorSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getNXTColorSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getNXTColorSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getNXTColorSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Light
        private NXTLightSensor getNXTLightSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new NXTLightSensor(LightMode.On, SensorMode.Raw); }
                case 2:
                    { return new NXTLightSensor(LightMode.On, SensorMode.Percent); }
                case 3:
                    { return new NXTLightSensor(LightMode.Off, SensorMode.Raw); }
                default:
                    { return new NXTLightSensor(LightMode.Off, SensorMode.Percent); }
            }
        }
        public double readNXTLight(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
            switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getNXTLightSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getNXTLightSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getNXTLightSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getNXTLightSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Sound
        private NXTSoundSensor getNXTSoundSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new NXTSoundSensor(SoundMode.SoundDB); }
                default:
                    { return new NXTSoundSensor(SoundMode.SoundDBA); }
            }
        }
        public double readNXTSound(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getNXTSoundSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getNXTSoundSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getNXTSoundSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getNXTSoundSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Sonar
        private Sonar getNXTSonarSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new Sonar(SonarMode.Centimeter); }
                default:
                    { return new Sonar(SonarMode.CentiInch); }
            }
        }
        public double readNXTSonar(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getNXTSonarSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getNXTSonarSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getNXTSonarSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getNXTSonarSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }


        // HiTech SENSORS
        // Color
        public double readHiTecColor(int indexBrick, int indexSensor, int mode) 
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                default: { break; }
                case 1:
                    {
                        brick2.Sensor1 = new HiTecColor(); 
                        rez = brick2.Sensor1.ReadAsInt(mode);
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = new HiTecColor(); 
                        rez = brick2.Sensor2.ReadAsInt(mode);
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = new HiTecColor(); 
                        rez = brick2.Sensor3.ReadAsInt(mode);
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = new HiTecColor(); 
                        rez = brick2.Sensor4.ReadAsInt(mode);
                        break;
                    }
            }
            return rez;
        }
        // Compass
        public double readHiTecCompass(int indexBrick, int indexSensor)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                default: { break; }
                case 1:
                    {
                        brick2.Sensor1 = new HiTecCompass();
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = new HiTecCompass();
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = new HiTecCompass();
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = new HiTecCompass();
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Gyro
        public double readHiTecGyro(int indexBrick, int indexSensor)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                default: { break; }
                case 1:
                    {
                        brick2.Sensor1 = new HiTecGyro(); 
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = new HiTecGyro(); 
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = new HiTecGyro(); 
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = new HiTecGyro(); 
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        // Tilt
        public double readHiTecTilt(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                default: { break; }
                case 1:
                    {
                        brick2.Sensor1 = new HiTecTilt();
                        rez = brick2.Sensor1.ReadAsInt(mode);
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = new HiTecTilt();
                        rez = brick2.Sensor2.ReadAsInt(mode);
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = new HiTecTilt();
                        rez = brick2.Sensor3.ReadAsInt(mode);
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = new HiTecTilt();
                        rez = brick2.Sensor4.ReadAsInt(mode);
                        break;
                    }
            }
            return rez;
        }

        //RCX SENSORS
        //Angle
        public double readRCXRotation(int indexBrick, int indexSensor)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                default: { break; }
                case 1:
                    {
                        brick2.Sensor1 = new RCXRotationSensor();
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = new RCXRotationSensor();
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = new RCXRotationSensor();
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = new RCXRotationSensor();
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        //Light
        private RCXLightSensor getRCXLightSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new RCXLightSensor(SensorMode.Percent); }
                default:
                    { return new RCXLightSensor(SensorMode.Raw); }
            }
        }
        public double readRCXLight(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getRCXLightSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getRCXLightSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getRCXLightSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getRCXLightSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }
        //Temperature
        private RCXTemperatureSensor getRCXTemperatureSensor(int mode)
        {
            switch (mode)
            {
                case 1:
                    { return new RCXTemperatureSensor(TemperatureMode.Celsius); }
                default:
                    { return new RCXTemperatureSensor(TemperatureMode.Fahrenheit); }
            }
        }
        public double readRCXTemperature(int indexBrick, int indexSensor, int mode)
        {
            Brick<Sensor, Sensor, Sensor, Sensor> brick2 = getBrickByIndex(indexBrick);
            double rez = 0;
		    switch (indexSensor)
            {
                case 1:
                    {
                        brick2.Sensor1 = getRCXTemperatureSensor(mode);
                        rez = brick2.Sensor1.ReadAsInt();
                        break;
                    }
                case 2:
                    {
                        brick2.Sensor2 = getRCXTemperatureSensor(mode);
                        rez = brick2.Sensor2.ReadAsInt();
                        break;
                    }
                case 3:
                    {
                        brick2.Sensor3 = getRCXTemperatureSensor(mode);
                        rez = brick2.Sensor3.ReadAsInt();
                        break;
                    }
                case 4:
                    {
                        brick2.Sensor4 = getRCXTemperatureSensor(mode);
                        rez = brick2.Sensor4.ReadAsInt();
                        break;
                    }
            }
            return rez;
        }

        public bool isMotorRun(int indexBrick, char motor, bool is_Sleep)
        {
            if (is_Sleep)
            {
                Thread.Sleep(100);
            }
            Motor brickMotor = getMotorByIndexBreakAndLitera(indexBrick, motor);
            return brickMotor.IsRunning();
        }


    }
}




