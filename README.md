# Dota2-Overlay-2.0
 A simple tool that will display an overlay text if you are Visible/NotVisible by the enemy side.

Note:
This program is meant to be an experimental purposes is considered as a 3rd party program, im not responsible for any actions you make.

The purpose if this program is to notify the user if his current position is visible to the enemy team.
There are 2 programs that does the job individualy, One checks the memory for updated information and the other display the current information through overlay.

![alt text](https://i.ibb.co/T8TFdsH/Screenshot-12.png)
![alt text](https://i.ibb.co/b2x1WW2/Screenshot-13.png)

The overlay is designed simply using c# window forms to lessen CPU/GPU usage while still updating fast realtime.

Memory reading is also lightweight by minimizing privilege and is done externally.

Usage :
Run Program ingame
Press END to exit

TODO :
- [x] Show overlay only when Dota 2 is highligted as window
- [x] Pattern Scan
- [x] Resize and move
- [x] Save current location and size 
- [x] Restore from config saved location and size 
- [] ...