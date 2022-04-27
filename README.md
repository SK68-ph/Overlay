# Dota2-Overlay-3.0
 A simple tool that will display an overlay text if you are Visible by the enemy side.

Note:
This program is made for educational purposes only.

The purpose if this program is to notify the user if the current position is visible to the enemy team.

Usage :
Run dota2 (offline and unsecured mode)
Run Program ingame
Press END to exit

How to build :
Download vstudio then install Desktop dev c++.
After that clone this repo and open project(.sln file).
Change Configuration from debug to release x64.
Then press Ctrl+Shift+b 

<details>
<summary>How to update offsets?</summary>
<br>
Scan for isVisibleByEnemy using these values (Not Visible = 06(radiant team), 10(dire team), Visible = 14) filter it out by changing the values/state ingame, should get at least 3 results.
There is only 1 address that we need, filter it out by using "Find out what accesses this address". there should only be 2 address listed(filter it out by changing the values/state ingame)
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
