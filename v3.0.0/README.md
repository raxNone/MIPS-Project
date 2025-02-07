# Pipeline MIPS Emulator (ver 3)

## Features
- **Defined at the bit level**
- **Implemented with multithreading**

## Memory Structure
| Component   | Size  |
|------------|------|
| `PAGE_SIZE`  | 16 bytes |
| `BLOCK_SIZE` | 4 bytes  |
| `WORD`       | 4 bytes  |

## Memory Access
### Instruction Memory (IMEM)
- **Read**: 4-byte units
- **Write**: 4-byte units

### Data Memory (DMEM)
- **Read**: 4-byte units  
- **Write**: Supports 1, 2, 4, and 8-byte units

## Forwarding Unit
The emulator includes two types of forwarding units to optimize pipeline performance:

- ### Branch Forwarding Unit (ID Stage)
  - Operates in the **Instruction Decode (ID) stage**.  
  - Forwards values needed for branch instructions early to reduce control hazards.  
  - Helps resolve dependencies before branch decisions are made, minimizing stalls.  

- ### Data Forwarding Unit (EX Stage)
  - Operates in the **Execute (EX) stage**.  
  - Resolves data hazards by forwarding results from later pipeline stages (EX/MEM or MEM/WB) back to the EX stage.  
  - Prevents stalls by ensuring operands are available when needed.  
