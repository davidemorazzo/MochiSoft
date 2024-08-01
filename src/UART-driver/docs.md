# Serial communication via UART

[Serial ports - OS dev](https://wiki.osdev.org/Serial_Ports)


Su un sistema x86 l'indirizzo della porta seriale UART è 
|COM Port |	IO Port|
| ---       | --- |
|COM1	|0x3F8|
|COM2	|0x2F8|
|COM3	|0x3E8|
|COM4	|0x2E8|
|COM5	|0x5F8|
|COM6	|0x4F8|
|COM7	|0x5E8|
|COM8	|0x4E8|

|IO Port Offset | Setting of DLAB | I/O Access | Register mapped to this port|
|---            |---              | ---         | ---                        |
| +0	| 0	| Read	|Receive buffer.
| +0	| 0	| Write	|Transmit buffer.
| +1	| 0	| Read/Write	|Interrupt Enable Register.
| +0	| 1	| Read/Write	|With DLAB set to 1, this is the least significant byte of the divisor value for setting the baud rate.
| +1	| 1	| Read/Write	|With DLAB set to 1, this is the most significant byte of the divisor value.
| +2	| -	| Read	|Interrupt Identification
| +2	| -	| Write	|FIFO control registers
| +3	| -	| Read/Write	|Line Control Register. The most significant bit of this register is the DLAB.
| +4	| -	| Read/Write	|Modem Control Register.
| +5	| -	| Read	|Line Status Register.
| +6	| -	| Read	|Modem Status Register.
| +7	| -	| Read/Write	|Scratch Register.