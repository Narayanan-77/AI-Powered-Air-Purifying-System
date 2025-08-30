
AI-Based Air Purifying System

This repository contains the source code, simulation files, model logic, and hardware overview of our AI-integrated indoor air purification system. The project is designed to monitor and purify indoor air using smart decision-making powered by a machine learning model, implemented on the ESP32-S3 microcontroller.

Project Overview : 

Indoor air pollution poses serious health risks in urban environments. Traditional purifiers operate continuously, ignoring real-time air quality, thus wasting energy. This project introduces an intelligent air purification system that activates only when necessary, using:

-  Sensors: MQ-135 (gas), DHT11 (temperature & humidity)
-  Controllers: Arduino Uno and ESP32-S3
-  ML Model: Decision Tree classifier to predict AQI
-  Dashboard: Real-time monitoring & manual override via web interface



Features :

-  Real-time air quality monitoring
-  AI-based decision making (ML model on edge)
-  Mobile/Web dashboard (Wi-Fi enabled)
-  Automatic fan control for purification
-  Energy-efficient, runs only when needed
-  Weather-integrated dataset with Open-Meteo API



Tech Stack :

Components used : 

| Component   | 		Usage            |
|-------------|----------------------------------|
| ESP32-S3    | Edge AI & communication          |
| Arduino Uno | Sensor interface & preprocessing |
| MQ-135      | Gas level monitoring 		 |
| DHT11       | Temp & humidity sensing          |
| Proteus     | Circuit simulation               |
| Google Colab| ML model training                |
| Decision Tree | Predictive logic               |
| HTML/CSS/JS | Dashboard interface (optional)   |


 System Architecture :

[ Sensors ] --> [ Arduino UNO ] --> [ ESP32-S3 (ML Model) ] --> [ Relay ] --> [ Fan + Cabin Air Filter ]
                                                        |
                                                  [ Wi-Fi Dashboard ]


 Simulation :

- Simulated in **Proteus** with three virtual chambers and real-time LCD display.
- Sensors before and after the filter compare air quality to evaluate purification efficiency.


 ML Model Details :

-  Trained on AQI, temperature, humidity, and PM2.5 datasets.
-  Model: Decision Tree (converted to C-style nested if-else for ESP32)
-  Feature Importance: PM2.5 > Humidity > Temperature
-  Split Criterion: Mean Squared Error (MSE)


 Live Dashboard :

- Built using ESP32-S3’s Wi-Fi.
- Displays:
  - Gas concentration (PPM)
  - Temperature and humidity
  - System status (Fan ON/OFF, Auto Mode)
- Allows manual override or auto mode selection.


 Applications :

-  Residential Homes
-  Offices
-  Educational Institutions
-  Clinics or Healthcare Settings



Limitations :

- Sensor accuracy (MQ-135, DHT11) may drift over time.
- Decision Tree model is simple; complex cases may need Random Forest or XGBoost.
- Real-time constraints on ESP32 (low memory, processing).



Future Work :

- Upgrade to advanced ML models like Random Forest
- Add additional air quality sensors (CO2, PM10)
- Integrate mobile app alerts
- Optimize power consumption for battery-powered use
- Enable cloud data logging for long-term trends


License :

This project is developed for academic purposes and is open for learning and non-commercial modifications.

