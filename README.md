# ECEGY6483-EmbeddedChallenge

Project Members: Ayushman Singh(as16513), Dimitrios Kaoutzanis (dk4572), Rachel Abreu (ra2466)


# Project Description
The purpose of this project is to detect when an infant has stopped breathing by activating a trigger, using the STM32F429i microcontroller. We used the STM32F429i microcontroller along with the MAX30102 sensor. For this project, we decided to build an app that will help display the values that are read from the sensor. The app also provides a warning that let's the patient/caregiver know when the breathing has stopped.

Here is a quick look at the application:
<insert image>

# Code Description
The coding is split into two parts. The first part is based on the connection of the microcontroller and the MAX30102. The second part is what was used to construst the application.
For the first part, we constructed the connection between the microcontroller and the MAX30102 sensor to read values of the heart rate and breathing rate of a patient.

For the second part, we developed an application for iPhone to display the data in a more understanding way that would make it easy for the average person to understand. The application receives the data from the microcontroller and sensor, showing the throw values as they increase/decrease. Graphs are also included to make the data more presentable.

  
# Future Developments
 While this application is useful and helps users interpret the data easier, we hope to add more features to it in the future. For instance, we would like to add a sleep tracking feature that shows the user what the infants current breath rate and heart rate are. In addition, it would display the amount of sleep time that the infant got. Another feature is being able to store the results every time the user runs a test so that they have records of any instances.
