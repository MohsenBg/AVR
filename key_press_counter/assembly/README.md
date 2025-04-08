# Step-by-Step Guide to Compile AVR Assembly Code:

### Step 1: Assemble the Assembly Source Code
To start, you need to assemble the Assembly code into an object file (`.o`). This step converts your Assembly code into machine-readable code.

```bash
avr-as -mmcu=atmega8 -o key_press_counter.o key_press_counter.s
```

### Explanation:
- `-mmcu=atmega8`: Specifies that the target microcontroller is the ATmega8.
- `-o key_press_counter.o`: The output object file name (`key_press_counter.o`).
- `key_press_counter.s`: Your Assembly source file.

### Step 2: Link the Object File to Create the ELF File
After assembling the Assembly code into an object file, link it to create the final executable ELF (Executable and Linkable Format) file.

```bash
avr-gcc -mmcu=atmega8 key_press_counter.o -o key_press_counter.elf
```

### Explanation:
- `-mmcu=atmega8`: Specifies the target microcontroller (ATmega8).
- `key_press_counter.o`: The object file generated in the previous step.
- `-o key_press_counter.elf`: The output ELF file name (`key_press_counter.elf`).

### Step 3: Convert the ELF File to HEX File
Next, convert the ELF file into a HEX file that can be uploaded to the microcontroller.

```bash
avr-objcopy -O ihex -R .eeprom key_press_counter.elf key_press_counter.hex
```

### Explanation:
- `-O ihex`: Converts the ELF file into Intel HEX format.
- `-R .eeprom`: (Optional) Excludes the EEPROM data from the HEX file.
- `key_press_counter.elf`: The ELF file created in the previous step.
- `key_press_counter.hex`: The resulting HEX file for uploading.