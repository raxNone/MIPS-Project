# MIPS Emulator Project

## Overview
This project is a MIPS emulator that simulates the hardware behavior of a computer processor based on the MIPS architecture. The software mimics the execution of instructions and provides an interface for interacting with registers, memory, and file uploads.

## Development Period
- **Start Date**: January 9, 2025
- **End Date**: February 12, 2025

## Team Members
- **최승환** (PM, Circuit Design and Implementation, Web Serving Implementation, [GitHub](https://github.com/raxNone/MIPS-Project/tree/develop))
- **조상혁** (Web Serving Implementation, [GitHub](https://github.com/p0tat019/p0tat019.github.io))
- **곽승헌** (Circuit.cpp Implementation, [GitHub](https://github.com/raxNone/MIPS-Project/tree/ksh_ing))

## Development Environment
- **Programming Language**: C++17
- **IDE**: Visual Studio Code 1.96.4
- **Framework**: Crow (C++ microframework)
- **Operating System**: Ubuntu 22.04 LTS

## Key Features
- **File Upload**: Allows users to upload necessary files for processing.
- **Clock Execution**: Emulates the clock cycles of the MIPS processor.
- **Register & Memory Inspection**: Provides a feature to query the status of registers and memory.

## How to Run

### Prerequisites
- Make sure you have `g++` and `make` installed on your system.

### Steps to Execute
1. Clone this repository:
    ```bash
    git clone https://github.com/raxNone/MIPS-Project.git
    ```

2. Navigate to the project directory:
    ```bash
    cd MIPS-Project
    ```

3. Build the project using `make`:
    ```bash
    make
    ```

4. Run the MIPS Emulator:
    ```bash
    make run
    ```
5. Clean build file:
    ```bash
    make clean
    ```

### File Upload
- Upload your MIPS program files via the web interface.

### Running the Emulator
- Drag File or Write Code at **textarea** under pc
- Click **Run Button** to start the emulation process.
- Click **Inspect Button** to inspect **Registers** and **Memory** to see the state of the MIPS processor.


## Project Structure

### Backend Server (`v3.0.0/`)

The backend is organized into the following modules:

- **`main/`**:  
  - Implements the **multithreaded pipeline circuit**.
  - Uses the **Crow** framework to create various API endpoints.

- **`circuit/`**:  
  - Implements **sequential circuits** at the bit level.

- **`memory/`**:  
  - Defines a class to handle **virtual address translation**.
  - Inherited classes for **instruction memory** and **data memory**.

- **`pipeline/`**:  
  - Implements the **pipeline registers** used in the MIPS processor emulation.

- **`register/`**:  
  - Implements the **registers** used by the processor.

- **`tool/`**:  
  - Contains utility functions for **readability** and **maintainability**.

### Frontend Server (`p0tat019.github.io/`)

The frontend server provides the **web interface** for interacting with the MIPS emulator.
I cloned the repository instead of using a submodule due to permission issues when trying to modify it.

### Test Folder (`test/`)

This folder is used to store **memory** and **assembly** code files for testing purposes.

---


## License (not yet)
<!-- This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. -->

## Future Enhancements
- Support for **ELF** and **EXE** file formats in addition to **ASM** files.
- **Circuit optimization** to reduce execution time.
- Implementation of **Verilog** for hardware descriptions.

## Acknowledgments
- Thanks to the Crow framework for providing the necessary tools for web server implementation.
- Special thanks to the team members for their contributions!
