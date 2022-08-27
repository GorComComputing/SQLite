#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Shell.h"


// Создает в динамической памяти структуру буфера ввода
InputBuffer* new_input_buffer() {
	// Создает в динамической памяти структуру буфера ввода
	InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	// Инициализирует нулевые значения в структуре буфера ввода
	//input_buffer->buffer = NULL;
	input_buffer->buffer_length = 0;
	input_buffer->input_length = 0;
	
	// Возвращает указатель на структуру буфера ввода
	return input_buffer;
}


// Выводит на экран приглашение ввода
void print_prompt() {
	printf("db > ");
}


// Читает ввод в структуру переданную в функцию указателем
void read_input(InputBuffer* input_buffer) {
	// Читаем ввод (передаем в функцию указатели на структуру)
	// (указатель буфера ввода, указатель на длину буфера ввода, входной поток для чтения)
	//size_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin); //ssize_t
	
	// Читает из стандартного ввода BUF_SIZE символов в буфер ввода в структуре
	fgets(input_buffer->buffer, BUF_SIZE, stdin);
	
	// Получаем длину полученного буфера ввода
	int bytes_read = lenString(input_buffer->buffer);
	// Если длина ввода меньше или равна 0
	if (bytes_read <= 0) {
		// Печатает сообщение об ошибке и выходит из программы с ошибкой
		printf("Error reading input\n");
		exit(EXIT_FAILURE);
	}
	
	// Пишет в структуру длину прочитанного ввода
	input_buffer->input_length = bytes_read - 1;
	// Дописывает в структуру последний символ 0 (конец строки)
	input_buffer->buffer[bytes_read-1] = 0;
	//printf("%d\n%s\n", input_buffer->input_length, input_buffer->buffer);
}


// Освобождаем память под структурой буфера ввода
void close_input_buffer(InputBuffer* input_buffer) {
	//printf("Free begin malloc...\n");
	// Освобождаем память под буфером элемента структуры
	//free(input_buffer->buffer);
	// Освобождаем память под экземпляром структуры
	free(input_buffer);
	//printf("Free end malloc...\n");
}


// Возвращает количество символов в строке
int lenString(char* str) {
    int i = 0;
    while (str[i]) i++;
    return i;
}


// Обработчик метакоманд
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) {
	// Проверка, если в буфере метакоманда ".exit" 
	if (strcmp(input_buffer->buffer, ".exit") == 0) {
		//printf("End program...\n%d\n", input_buffer);
		// Освобождаем память под структурой буфера ввода
		close_input_buffer(input_buffer);
		// Освобождаем память под таблицей
		free_table(table);
		// Выход из программы
		exit(EXIT_SUCCESS);
	}
	else {
		// Возвращаем: Недопустимая команда
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}








