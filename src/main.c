/*
** EPITECH PROJECT, 2022
** my.h
** File description:
** my
*/

#include "../include/syscall.h"

static void help(void)
{
    puts("USAGE: ./strace [-s] [-p <pid>|<command>]");
}

static void child(const char *const *av)
{
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    kill(getpid(), SIGSTOP);
    execvp(*av, (char *const *) av);
    perror("execvp");
    exit(EXIT_FAILURE);
}

static void handle_unknown_syscall(size_t k)
{
    printf("\033[31mSyscall %zu unknown\033[0m\n", k);
}

static void handle_syscall(registers_t *regs, int options, const pid_t *pid)
{
    if (!(regs->orig_rax <= 335 && table[regs->orig_rax].index != -1)) {
        handle_unknown_syscall(regs->orig_rax);
        return;
    }
    if (options & 1) {
        table[regs->orig_rax].fn(regs, pid);
        return;
    }
    printf("%s(", table[regs->orig_rax].syscall_name);
    for (int i = 0; i < table[regs->orig_rax].argument_count; ++i) {
        printf("0x%x", (int)((size_t *)(regs))[i]);
        if (i != table[regs->orig_rax].argument_count - 1)
            printf(", ");
    }
    printf(") = 0x%x\n", (int)(regs->rax));
}

static void parent(const pid_t *pid, int options)
{
    int status;
    struct user_regs_struct regs;

    while (1) {
        wait4(*pid, &status, 0, NULL);
        if (WIFEXITED(status))
            break;
        ptrace(PTRACE_GETREGS, *pid, NULL, &regs);
        if (regs.orig_rax != (size_t)(-1))
            handle_syscall(&(registers_t){regs.rdi, regs.rsi,
            regs.rdx, regs.rcx, regs.r8, regs.r9, regs.orig_rax, regs.rax},
            options, pid);
        ptrace(PTRACE_SINGLESTEP, *pid, NULL, NULL);
    }
}

int my_strace(const char *const *av, int options, int p_value)
{
    pid_t pid;

    if (options & 2) {
        ptrace(PTRACE_ATTACH, p_value, NULL, NULL);
        parent(&p_value, options);
    } else {
        pid = fork();
        switch (pid) {
            case -1:
                perror("fork");
                return EXIT_FAILURE;
            case 0:
                child(av);
                break;
            default:
                parent(&pid, options);
                break;
        }
    }
    return EXIT_SUCCESS;
}

static int get_pid(const char *input)
{
    char *next;
    long val = strtol(input, &next, 10);

    if ((next == input) || (*next != '\0')) {
        return -1;
    } else {
        return (int)val;
    }
}

static int handle_options(int option, int *p_value)
{
    switch (option) {
        case 's':
            return 1;
        case 'p':
            *p_value = get_pid(optarg);
            if (*p_value == -1)
                return 84;
            return 2;
        default:
            return 84;
    }
}

static int get_options(int ac, char const *const *av)
{
    int option;
    int p_value = 0;
    int r_value = 0;

    while (1) {
        option = getopt(ac, (char *const *)av, "p:s");
        if (option == -1)
            break;
        r_value = handle_options(option, &p_value);
        if (r_value >= 84)
            return 84;
    }
    if (r_value & 2 && optind < ac)
        return 84;
    return my_strace(&av[optind], r_value, p_value);
}

int main(int ac, char const *av[])
{
    if (ac == 1) {
        return EXIT_FAILURE;
    } else if (ac == 2 && strcmp(av[1], "--help") == 0) {
        help();
    } else {
        return get_options(ac, av);
    }
    return EXIT_SUCCESS;
}
