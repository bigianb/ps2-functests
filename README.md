# ps2-functests
Functional tests for PS2 emulation

This program calls PS2 bios functions in a manner similar to how games use them. By comparing the output from a real BIOS running under, say, PCSX2 against an HLE BIOS running under, say, Play! then we can validate the behavior of the HLE implementation.
This is necessary because there is not much good documentation on the PS2 BIOS interface and games often relay on edge cases and bugs.

## Setup
Refer to [setup.md](setup.md) for details on how to configure a suitable compilation environment.

## Running

Run make to generate an ISO in the build directory.
