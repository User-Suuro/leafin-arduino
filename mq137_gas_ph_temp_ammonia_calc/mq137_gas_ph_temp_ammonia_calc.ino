    #include "MQ137Sensor.h"
    #include "pHSensor.h"
    #include "DS18B20Sensor.h"
    #include "AmmoniaEstimator.h"

    #define MQ137_PIN A0
    #define PH_PIN A1
    #define ONE_WIRE_BUS 2 // DS18B20 data pin

    MQ137Sensor mq(MQ137_PIN);
    pHSensor ph(PH_PIN);
    DS18B20Sensor temp(ONE_WIRE_BUS);

    AmmoniaEstimator ammonia(mq, ph, temp);

    void setup() {
        Serial.begin(9600);

        // Example calibration values for MQ137
        mq.setCalibration(10.0, -0.263, 0.42);

        ammonia.begin();
    }

    void loop() {
        ammonia.update();

        Serial.print("pH: "); Serial.print(ph.readPH());
        Serial.print(" | Temp: "); Serial.print(temp.readTemperature()); Serial.print(" C");
        Serial.print(" | NH3 gas PPM: "); Serial.print(ammonia.getNH3GasPPM());
        Serial.print(" | Fraction NH3: "); Serial.print(ammonia.getFractionNH3() * 100, 2); Serial.print("%");
        Serial.print(" | Total Ammonia PPM: "); Serial.println(ammonia.getTotalAmmoniaPPM());

        delay(2000);
    }
