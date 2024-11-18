# SYSTEM CALLS REFERENCE

Derived from Linux syscall list.
Calling convention:
- System call id: `%eax`
- Return value: `%eax`
- Parameters: `%ebx` `%ecx` `%edx` `%esi` `%edi` `%ebp`

All the registers are preserved across the 0x80 interrupt, including `EFLAGS`


| NAME 	| ID 	| SIGNATURE 				|
| ---  	| --- 	| ---						|
|time	| 201	| time_t time(time_t *tloc)	|