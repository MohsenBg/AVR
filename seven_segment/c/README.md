# Step-by-Step Guide to Compile AVR C Code:

### Step 1: Compile and Link the C Source File to Create the ELF File

You can directly compile and link your C source file into an ELF file with the following command:

```bash
avr-gcc -mmcu=atmega8 -o seven_segment.elf seven_segment.c
```

### Explanation:
- `-mmcu=atmega8`: Specifies the target microcontroller as the ATmega8.
- `-o seven_segment.elf`: The output file name for the ELF file (`seven_segment.elf`).
- `seven_segment.c`: The C source file you are compiling and linking.

---

### Step 2: Convert ELF File to HEX File

After generating the ELF file, the next step is to convert it into a HEX file. This HEX file can then be uploaded to your microcontroller. Use the following command to perform the conversion:

```bash
avr-objcopy -O ihex -R .eeprom seven_segment.elf seven_segment.hex
```

### Explanation:
- `-O ihex`: Converts the ELF file into Intel HEX format, which is commonly used for AVR microcontrollers.
- `-R .eeprom`: (Optional) Excludes the EEPROM data from the HEX file. If you don't need the EEPROM data, you can omit this option.
- `seven_segment.elf`: The ELF file generated in Step 1.
- `seven_segment.hex`: The resulting HEX file (`seven_segment.hex`) that can be uploaded to the microcontroller.
