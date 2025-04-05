# Step 1: Compile the C source file to an object file
To start, you need to compile the C source file into an object file. This step converts your C code into machine code but does not yet create the final executable.

```bash
avr-gcc -mmcu=atmega8 -c blink_led.c -o blink_led.o
```

### Explanation:
- `-mmcu=atmega8`: Specifies that the target microcontroller is the ATmega8.
- `-c`: Tells `avr-gcc` to compile the C code without linking.
- `blink_led.c`: The C source file you are compiling.
- `-o blink_led.o`: The output object file name (`blink_led.o`).

# Step 2: Link the object file to create the ELF file
After compiling the C file to an object file, the next step is to link the object file to create the final ELF (Executable and Linkable Format) file. This file is ready for uploading to the microcontroller.

```bash
avr-gcc -mmcu=atmega8 blink_led.o -o blink_led.elf
```

### Explanation:
- `-mmcu=atmega8`: Specifies the target microcontroller (ATmega8).
- `blink_led.o`: The object file created in the previous step.
- `-o blink_led.elf`: The output ELF file name (`blink_led.elf`).

# Step 3: Convert ELF to HEX file
The next step is to convert the ELF file into a HEX file. The HEX file format is used to store the binary data that will be uploaded to the microcontroller.

```bash
avr-objcopy -O ihex -R .eeprom blink_led.elf blink_led.hex
```

### Explanation:
- `-O ihex`: Converts the ELF file into Intel HEX format, which is commonly used for AVR microcontrollers.
- `-R .eeprom`: (Optional) Excludes the EEPROM data from the HEX file (if you don’t need to include EEPROM data, you can omit this option).
- `blink_led.elf`: The ELF file created in the previous step.
- `blink_led.hex`: The resulting HEX file (`blink_led.hex`).