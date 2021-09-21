# Dota2-Overlay-2.0
 A simple tool that will display an overlay text if you are Visible/NotVisible by the enemy side.

Note:
This program is made for educational purposes only.

The purpose if this program is to notify the user if his current position is visible to the enemy team.
There are 2 programs that does the job individualy, One checks the memory for updated information and the other display the current information through overlay.

![alt text](https://i.ibb.co/T8TFdsH/Screenshot-12.png)
![alt text](https://i.ibb.co/b2x1WW2/Screenshot-13.png)


Usage :
Run dota2 (offline and unsecured mode)
Run Program ingame
Press END to exit

<details>
<summary>How to update offsets?</summary>
<br>
Scan for p_TagVisibleByEnemy using these values (Not Visible = 06(radiant team) , 10(dire team) ; Visible = 14) should get at least 3 results.
There is only 1 address that we need, filter it out by using "Find out what accesses this address". there should only be 2 address listed(try to change the value)
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut1.PNG?raw=true)

Generate a pointermap, after that select pointer scan for this address
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut2.PNG?raw=true)
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut3.PNG?raw=true)

Select the generated pointermap, then press ok
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut4.PNG?raw=true)

Rescan pointerlist, input the address of vbe then check the Must start with offsets and insert the values shown in the img 
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut5.PNG?raw=true)

There should be only 2 results, either of them works but i recommend choosing the one with 0x170 value.
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut6.PNG?raw=true)

create/update the offsets.ini with your updated offset(should be in the same directory as the exe file).
![alt text](https://github.com/skrixx68/Dota2-Overlay-2.0/blob/master/tut%20img/tut7.PNG?raw=true)
</details>
