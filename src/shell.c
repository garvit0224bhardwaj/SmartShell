#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_LINE 1024
#define TOKEN_DELIM " \t\r\n\a"

void shell_loop() {
    char *input;
    char **args;
    int status;

    do {
        printf("mysh> ");
        input = read_input();
        args = parse_input(input);
        status = execute_command(args);

        free(input);
        free(args);
    } while (status);
}

char *read_input() {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

void print_help() {
    printf("Available commands:\n");
    printf("cd [dir]     - Change directory\n");
    printf("cd ..        - Move one directory level up\n");
    printf("exit         - Exit the shell\n");
    printf("help         - Show this help message\n");
    printf("ls           - List files in current working directory\n");
    printf("pwd          - Show current working directory\n");
    printf("ai: <prompt> - Run AI prompt using llama model\n");
}

int execute_command(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "exit") == 0) return 0;

    if (strcmp(args[0], "help") == 0) {
        print_help();
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL || strlen(args[1]) == 0) {
    printf("cd: expected argument\n");
} else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    }
   if (strncmp(args[0], "ai:", 3) == 0) {
    char prompt[1024] = {0};
    strcat(prompt, args[0] + 3);

    for (int i = 1; args[i] != NULL; i++) {
        strcat(prompt, " ");
        strcat(prompt, args[i]);
    }

    run_llama_model(prompt);
    return 1;
   }
    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "Error: '%s' is not a recognized command.\n", args[0]);
            fprintf(stderr, "Tip: Type 'help' to see available commands.\n");
        }
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }

    return 1;
}
void run_llama_model(const char *prompt) {
    char formatted_prompt[2048];
    snprintf(formatted_prompt, sizeof(formatted_prompt),
        "<|im_start|>user\n%s\n<|im_end|>\n<|im_start|>assistant\n", prompt);

    char command[4096];
    snprintf(command, sizeof(command),
        "/home/khushi/llama.cpp/build/bin/llama-run /home/khushi/llama.cpp/models/mistral-7b-instruct-v0.1.Q4_K_M.gguf \"%s\" --n_predict 50",
        formatted_prompt);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run llama-run");
        return;
    }

    char output[4096];
    while (fgets(output, sizeof(output), fp) != NULL) {

        if (strstr(output, "<|im_end|>") == NULL && strstr(output, "<|im_start|>") == NULL) {
            printf("%s", output);
        }
    }

    pclose(fp);
}




char **parse_input(char *input) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(input, TOKEN_DELIM);
    while (token != NULL) {
        tokens[position++] = token;
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int main(int argc, char **argv) {
    shell_loop();
    return EXIT_SUCCESS;
}
