# ayypex

The external version of ayypex

What is missing:
* Wallhack
* Glow
* Aimbot

(will add the last 2 later... or you could do that yourself)

What isn't missing:
* Triggerbot (probably works, can't test it due to absolutely insane lag at the moment)
* Recoil control (couldn't get it working using mouse movement so I pasted it instead)

Keep in mind that this cheat makes no attempt at hiding its presence, except for it not being well known.

## Building

Generate build files with ```cmake .```

Build with ```make```

And finally, run the "ayypex" executable as root. ```sudo ./ayypex```

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