# Step-by-Step Guide to Compile AVR Assembly Code:

### Step 1: Assemble the Assembly Source Code
To start, you need to assemble the Assembly code into an object file (`.o`). This step converts your Assembly code into machine-readable code.

```bash
avr-as -mmcu=atmega8 -o blink_led.o blink_led.asm
```

### Explanation:
- `-mmcu=atmega8`: Specifies that the target microcontroller is the ATmega8.
- `-o blink_led.o`: The output object file name (`blink_led.o`).
- `blink_led.asm`: Your Assembly source file.

### Step 2: Link the Object File to Create the ELF File
After assembling the Assembly code into an object file, link it to create the final executable ELF (Executable and Linkable Format) file.

```bash
avr-gcc -mmcu=atmega8 blink_led.o -o blink_led.elf
```

### Explanation:
- `-mmcu=atmega8`: Specifies the target microcontroller (ATmega8).
- `blink_led.o`: The object file generated in the previous step.
- `-o blink_led.elf`: The output ELF file name (`blink_led.elf`).

### Step 3: Convert the ELF File to HEX File
Next, convert the ELF file into a HEX file that can be uploaded to the microcontroller.

```bash
avr-objcopy -O ihex -R .eeprom blink_led.elf blink_led.hex
```

### Explanation:
- `-O ihex`: Converts the ELF file into Intel HEX format.
- `-R .eeprom`: (Optional) Excludes the EEPROM data from the HEX file.
- `blink_led.elf`: The ELF file created in the previous step.
- `blink_led.hex`: The resulting HEX file for uploading.