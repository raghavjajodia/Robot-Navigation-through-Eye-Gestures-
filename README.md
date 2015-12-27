Readme

This project was built by 
1. Raghav Jajodia
2. Saurabh Satnalika

in Inspire Lab at Birla Institute of Technology and Science, Pilani Campus under the professors

1. Prof. Avinash Gautam
2. Dr. Sudeept Mohan

This project was build in OpenCv 2.4.8, Visual Studio 2010 (32 bit), SFML (msvc2010-2.3.2-32bit)

Installing instructions
1) Install OpenCv, (preferrably 2.4.8). If any other version of opencv is installed, then make changes to properties as mentioned later.
2)Install Visual Studio (32bit, and preferrably 2010). 
3) Install SFML library (32bit and the version should be compatible with Visual studio)

Changing properties

1) Go to properties->configuration Property->VC++ Directories->Library Directories. Change the paths to the approriate paths.
2) Go to properties->configuration Property-> C/C++ ->General -> Additional Include. Change the paths to the approriate paths. Here the last path is the path to INCLUDE directory of SFML.
3) Go to properties->configuration Property-> Linker ->General -> Additional Library directories. Change the paths to the approriate paths. Here the last path is the path to Lib directory of SFML.
3) If opencv version is 2.4.8 then skip this step. Else, Go to properties->configuration Property-> Linker -> Input -> Additional Dependencies. Replace 248 with the version of opencv in all the items. Ex. if version is 2.4.9 then write .....249d
4) In PORTCHAT class, replace "COM7" with the port of Bluetooth.
5) In playSound function, replace the path with the path of the folder of audio (.wav) files.
6)Replace the value of Global variable face_cascade_name with the path of the .xml file.
7) If SFML version is 2.3.2, then skip this step. Else, replace the same .dll files from SFML->bin folder and paste it to debug folder of the project.  

Running the project

NOTE: Before running the project, make sure the .hex file(Present along with the project) is burnt to Epuck.

1) Switch on the Epuck, and turn it to mode 2.
2)Run the project.
3) You will find 2 options in console window
a) Manual Caliberation
b) Automatic Caliberation
Choose 2nd option by typing 2 and press Enter.
4) That's it. If your pupil is on the left corner of the Eye, then you will hear the sound "Left". Similary for up and right. After you hear the sound, Epuck will 
turn left, move forward and turn right respectively.
NOTE:- It might take some time for Epuck to react, because of slow Bluetooth communication.