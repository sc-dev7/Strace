# SCHOOL - Project

## DESCRIPTION  
The **Strace** project involves creating a simplified version of the `strace` command-line utility, which is used to trace system calls made by a program.  
Your implementation will intercept and display system calls, their arguments, and return values for a given binary.

---

## FEATURES  
- **System call tracing:**  
  - Display the system calls invoked by a program.  
- **Argument inspection:**  
  - Show the arguments passed to each system call.  
- **Return values:**  
  - Display the return values of the system calls.  
- **Execution transparency:**  
  - Run the program without altering its behavior while collecting system call information.

---

## FUNCTIONS ALLOWED  
- **System calls:**  
  - `fork`, `execve`, `waitpid`, `ptrace`, `kill`, `signal`.  
- Standard library functions are limited to:  
  - `malloc`, `free`, `write`, `read`, `sprintf`, `snprintf`.

---

## USAGE  
### Syntax:  
```bash
./strace [program] [args...]
