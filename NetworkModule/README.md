## How it'll work

1. Needs the Interrupts for RXComplete on both USART0 and USART1

INFO - 8 bytes

4 bytes - Unique Address

2 bytes - Mod Type

2 bytes - Mod Version

2. Packet types

All packets are at least 2 Bytes

1 byte - Length of Packet

1 byte - Type of Packet

HUB 0x9 - 2 Bytes

Comes from the hub, a general minimal informational request for the MOD packet, probably intermittent

MOD 0x18 - 10 Bytes

Comes from the modules, has all the info about them

ACT 0x25 - 6 + info

Contains information for the modules at given address

3. Mod types

1 - LED

3 - SERVO