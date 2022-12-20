<<<<<<< HEAD
# ECEGY6483-EmbeddedChallenge
=======
# ECE-GY 6483 - Embedded Challenge Fall 2022 Term Project - Just Breathe

Project Members: Ayushman Singh (as16513), Dimitrios Kaoutzanis (dk4572), Rachel Abreu (ra2466)

# Project Description
The purpose of this project is to use a STM32F429 microcontroller to detect when a baby stops breathing by activating a trigger. We used the STM32F429 microcontroller along with the MAX30102 sensor. For this project, we decided to build an app that will help display the values that are read from the sensor. Raw data from the sensor is sent to a HM10 Bluetooth module that is connected to the microcontroller using UART (PC_6 and PC_7 pins) and then shown on the app. The app also provides a warning that let's the patient/caregiver know when the breathing has stopped.

# Code Description
The coding is split into two parts. The first part is based on the connection of the microcontroller and the MAX30102 (which is connected to the microcontroller over I2C using Pins PF_0 and PF_1). The second part is what was used to construct the application. The application was developed using a fork of a React Native BLE sample app (https://github.com/ayush1399/SIDS-App). For the first part, we constructed the connection between the microcontroller, and the MAX30102 sensor to read values of the heart rate and blood oxygen of a patient. For the second part, we developed an application for iPhone to display the data in a more understanding way that would make it easy for the average person to understand. The application receives the raw data from the microcontroller and sensor, showing the values as they increase/decrease. In addition, it indicates a warning when the oxygen measurement is normal or abnormal, such as "BREATH IS NORMAL" or "NOT BREATHING!!!". Graphs are also included to make the data more presentable. The app also displays whether sensor is calibrated (giving accurate values) or is being calibrated.
 
# Future Developments
While this application is useful and helps users interpret data more easily, we hope to add more features to it in the future. For example, we wanted to add a sleep tracking feature that would show the user the baby's current breathing rate and heart rate. What's more, it would display the amount of sleep time that the infant got. Another feature is the ability to store results each time a user runs a test so they have a record of any instance.
We also want to explore using the IR sensor on the MAX30102 to detect breathing patterns of the baby from a distance (without touching the baby) - this would be configurable from the app allowing the user to choose between 2 modes, sensing from a distance or sensing when in contact with the infant.
We also want apply more filters to make the breathing detection even more robust and switch to the MAX30101 sensor which is more accurate than the MAX30102 sensor (due to the presence of an additional green LED).
>>>>>>> origin/main
