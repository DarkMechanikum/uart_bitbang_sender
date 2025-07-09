# uart_bitbang_sender
A bit bang uart send-only implementation on gpiod
Note: it onlhy works with very low baud-rates due to imprecise timings of gpiod (since it's in linux userspace). 
I recommend using baud rate 600.

Please see the main.cpp for usage example.
