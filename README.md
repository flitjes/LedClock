A project for creating ledclock. 
The project contains the source for the actual clock and is depended
on a couple of breakouts, RTC, LDR and PIR.

This is tied together with an msp430g2553 to display the time on a ledclock
and be energy sufficient.

Besides the software I've made a 3D drawing which will be milled out of a mix
of materials, plexiglas, carbonfiber and MDF.

Protocol:
Set the time
time=12:10:04
start

setcolor=[id][rgb code]
id 0 = HOUR
id 1 = MINUTE
id 2 = SECOND (not implemented)
id 3 = BACKGROUND (not implemented)

Set the color
setcolor=0ffddff
start

Example with netcat
nc -u 192.168.4.1 12345
setcolor=0ff00ff

