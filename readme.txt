wrbot
=====
  wrbot - read location point from text file, perform distance calculation and maybe execute command to GPIO


hardware requirement
-------
  - Host        :   Mini-PC or SBCs that have GPIO
  - Controller  :   MCU with ADC, DAC built-in for sensor reading, drive motor
  - GPS module  :   NEO-6M or anything support NMEA output and work well with gpsd
  - 9-DoF IMU   :   Adafruit TDK InvenSense or separate digital compass, accelerator, gyro


hardware diagram
----------------
    
                              i2c   ┌─────┐
                              ┌───► │ imu │
                              │     └─────┘
                              │
    ┌─────┐    usb/uart    ┌──┴──┐  pwm  ┌──────────────┐
    │ RPi ├──────────────► │ mcu ├──────►│ motor-driver │
    └─┬───┘                └──┬──┘       └─┬────────────┘
      │                       │            │
      │ uart                  │            │  ┌───────────────┐
      │    ┌────────────┐     │            └─►│ bldc-motor x4 │
      └──► │ gps module │     │ i2c           └───────────────┘
           └────────────┘     │    ┌───────┐
                              ├───►│ laser │
                              │    └───────┘
                              │ digital
                              │    ┌────────────┐
                              └───►│ ultrasonic │
                                   └────────────┘

compile
-------
  build this project using `make`,  makefile is written with `bmake` compatible in mind but normal should also works


optional build flags
--------------------
  hardware   -  interface with hardware, primary written for raspberry pi gpio
  adv_method -  advance methods like a* , PID, ext kalman filter ... etc
  database   -  save history, mapping location, configuration


required for options build
--------------------------
  - libsqlite3-dev
  - libgpiod-dev
