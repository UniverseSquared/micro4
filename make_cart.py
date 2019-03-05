#!/usr/bin/env python
import sys
import struct

def main():
    if len(sys.argv) < 3:
        print("Usage: %s <output cart path> <lua code path>" % sys.argv[0])
        return

    code_file = open(sys.argv[2], "r")
    rom_file = open(sys.argv[1], "w")
    code = code_file.read()

    # Write the header and a newline.
    rom_file.write("MC4C\n")

    # Write the size of the code.
    code_size = len(code)
    rom_file.write(struct.pack(">h", code_size + 1).decode())

    # Write the code followed by a null-terminator.
    rom_file.write(code + "\x00")

    # Close the files.
    rom_file.close()
    code_file.close()

if __name__ == "__main__":
    main()
