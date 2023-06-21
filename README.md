The latest commit adds an overlay and menu that I was making for another project which can be toggled via Insert. It is up to you to add any features that use this overlay. I haven't tested the menu extensively, but it should work.

Currently, this repository is more of a base to add your own features using the SDK and overlay code, **not** a ready to use cheat.

!! the offsets are probably not up to date, I will not update them !!

# ayypex

The external version of ayypex

What is missing:
* Wallhack
* Glow
* Aimbot

~~(will add the last 2 later... or you could do that yourself)~~

You'll need to add them yourself. They're mostly there, just commented out and with some bugs that I didn't fix.
Wallhack **should** work if you just copy+paste the files from the internal and change some things.

What isn't missing:
* Triggerbot (does not distinguish between full-auto and semiauto weapons)
* Recoil control (pasted from apexbot/apexdream... couldn't get my implementation working using mouse movement)

Keep in mind that this cheat makes no attempt at hiding its presence, except for it not being well known.

## Building

Generate build files with ```cmake .```

Build with ```make```

And finally, run the "ayypex" executable as root. ```sudo ./ayypex```
* Root privileges are needed for the features that move the mouse via /dev/input/mouse.

## Bugs
* Getting the player's held weapon is broken. This breaks semi-auto triggerbot as now there is no way to tell if the player's weapon is semi-auto or not. The triggerbot for now assumes that any weapon is just full-auto. I updated the offsets but Respawn changed something and now it doesn't work. Let me know if you find the issue.

## Todo
* Fix current weapon detection
* Fix aim
* Fix glow
* Write an overlay
* Get offsets from INI file offset dump

## Credits

* The UnKnoWnCheaTs forum.
