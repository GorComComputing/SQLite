#ifndef SHELL_H
#define SHELL_H

// Размер буфера ввода (обрабатывает-2)
#define BUF_SIZE 1024

// Структура буфера ввода
typedef struct {
	char buffer[BUF_SIZE];
	size_t buffer_length;
	size_t input_length;  // ssize_t
} InputBuffer;

// Перечисление результата обработки метакоманд
typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;


#endif /*SHELL_H*/