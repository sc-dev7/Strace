/*
** EPITECH PROJECT, 2022
** my.h
** File description:
** my
*/

#include "../include/syscall.h"

const char *ascii_table[128] =
{
    "\\0", "\\1", "\\2", "\\3", "\\4", "\\5", "\\6", "\\a",
    "\\b", "\\t", "\\n", "\\v", "\\f", "\\r", "\\e", "\\f",
    "\\10", "\\11", "\\12", "\\13", "\\14", "\\15", "\\16", "\\17",
    "\\18", "\\19", "\\1a", "\\1b", "\\1c", "\\1d", "\\1e", "\\1f",
    " ", "!", "\"", "#", "$", "%", "&", "'",
    "(", ")", "*", "+", ",", "-", ".", "/",
    "0", "1", "2", "3", "4", "5", "6", "7",
    "8", "9", ":", ";", "<", "=", ">", "?",
    "@", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "[", "\\\\", "]", "^", "_",
    "`", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "{", "|", "}", "~", "\\7f"
};

flag_info_t op_flags[] = {
    {O_RDONLY, "O_RDONLY"},
    {O_WRONLY, "O_WRONLY"},
    {O_RDWR, "O_RDWR"},
    {O_CREAT, "O_CREAT"},
    {O_EXCL, "O_EXCL"},
    {O_NOCTTY, "O_NOCTTY"},
    {O_TRUNC, "O_TRUNC"},
    {O_APPEND, "O_APPEND"},
    {O_NONBLOCK, "O_NONBLOCK"},
    {O_DIRECTORY, "O_DIRECTORY"},
};

void handle_unk_syscall(registers_t *regs, const pid_t *pid)
{
    (void)pid;
    printf("%s(", table[regs->orig_rax].syscall_name);
    for (int i = 0; i < table[regs->orig_rax].argument_count; ++i) {
        printf("0x%x", (int)((size_t *)(regs))[i]);
        if (i != table[regs->orig_rax].argument_count - 1)
            printf(", ");
    }
    printf(") = 0x%x\n", (int)(regs->rax));
}

void handle_write_syscall(registers_t *regs, const pid_t *pid)
{
    long data;
    bool etc = false;

    printf("%s(%d, \"", table[regs->orig_rax].syscall_name, (int)regs->rdi);
    for (int i = 0; i < (int)((size_t *)(regs))[2]; ++i) {
        if (i >= 32) {
            etc = true;
            break;
        }
        data = ptrace(PTRACE_PEEKTEXT, *pid, ((size_t *)(regs))[1] + i, NULL);
        if (data == -1)
            break;
        if ((char)data < 127) {
            printf("%s", ascii_table[(char) data]);
        } else {
            printf("\\x%02x", (unsigned char)(char)data);
        }
    }
    printf("\"%s, %d", (etc) ? "..." : "", (int)((size_t *)(regs))[2]);
    printf(") = %d\n", (int)(regs->rax));
}

void print_flags(int flags)
{
    int printed = 0;

    for (size_t i = 0; i < sizeof(op_flags) / sizeof(op_flags[0]); ++i) {
        if (!(flags & op_flags[i].flag))
            continue;
        if (printed)
            printf("|");
        ++printed;
        printf("%s", op_flags[i].name);
    }
    if (!printed)
        printf("0");
}

void handle_openat_syscall(registers_t *regs, const pid_t *pid)
{
    int flags = (int)((size_t *)(regs))[2];
    long data;
    size_t i;
    char path[4096];

    for (i = 0; i < sizeof(path) - 1; ++i) {
        data = ptrace(PTRACE_PEEKTEXT, *pid, ((size_t *)(regs))[1] + i, NULL);
        if (data == -1 || (char)data == '\0')
            break;
        path[i] = (char)data;
    }
    path[i] = '\0';
    printf("%s(AT_FDCWD, \"%s\", ", table[regs->orig_rax].syscall_name, path);
    print_flags(flags);
    printf(", %zd) = %d\n", ((size_t *)(regs))[4], (int)(regs->rax));
}

void handle_open_syscall(registers_t *regs, const pid_t *pid)
{
    int flags = (int)((size_t *)(regs))[2];
    long data;
    size_t i;
    char path[4096];

    for (i = 0; i < sizeof(path) - 1; ++i) {
        data = ptrace(PTRACE_PEEKTEXT, *pid, ((size_t *)(regs))[1] + i, NULL);
        if (data == -1 || (char)data == '\0')
            break;
        path[i] = (char)data;
    }
    path[i] = '\0';
    printf("%s(\"%s\", ", table[regs->orig_rax].syscall_name, path);
    print_flags(flags);
    printf(") = %d\n", (int)(regs->rax));
}
