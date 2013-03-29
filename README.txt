Google Hangout Renice: Sets "idle" priority and 0 CPU affinity mask
for Google Hangout plugin to lower the system load
(C) dkLab, http://en.dklab.ru/lib/GoogleHangoutRenice

This simple program for Windows watches for Googhe Hangout plugin appearance
and sets low priority and single-CPU affinity mask to it. This makes the
system much more responsible on slow computers with no side effects.

If your computer becomes slow when you use Google Hangout, just put
GoogleHangoutRenice.exe to your Startup folder. It will remain in background
to make Google Hangout less greedy.
