# EV26-TMS
TMS firmwear functionality

TMS_SATELLITE:
Continuously sends tempatures to TMS_MAIN

TMS_MAIN:
Continuously receives tempatures from TMS_SATELLITE. Stores temperatures, sends them to BMS.


TMS_MAIN:

main.c
Start threads for getting temperature values, storing temperatures, formatting and sending temperatures via CAN.

comms_iso_spi.c
Receive satellite temperatures continuiously 

temp_table.c 
Store satellite temperatures continuiously



TMS_SATELLITE:

Main.c
Start thread for sending temperatures via CAN

Iso_spi.c
Send satellite temperatures continuiously 
