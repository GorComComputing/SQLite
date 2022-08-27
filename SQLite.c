// СУБД SQLite

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Shell.h"



int main(int argc, char* argv[]) {
	// Создаем таблицу
	Table* table = new_table();
	// Возвращает ссылку на структуру буфера ввода
	InputBuffer* input_buffer = new_input_buffer();
	// Выводит приветствие
	printf("SQLite Gor.Com\nStarted\n");
	// Вечный цикл обработки ввода команд
	while (true) {
		// Выводит на экран приглашение ввода
		print_prompt();
		// Читает ввод в структуру переданную в функцию указателем
		read_input(input_buffer);
		
		// Проверка, если первый символ '.' - значит введена метакоманда
		if (input_buffer->buffer[0] == '.') {
			// Получает метакоманду из буфера ввода
			switch (do_meta_command(input_buffer, table)) {
				case (META_COMMAND_SUCCESS):
					// Заглушка
					continue;
				case (META_COMMAND_UNRECOGNIZED_COMMAND):
					// Выводит надпись: Недопустимая команда
					printf("Unrecognized command '%s'.\n", input_buffer->buffer);
					continue;
			}
		}
		
		// Для хранения оператора
		Statement statement;
		// Получает оператор из буфера ввода, кладет его по указателю в структуру
		switch (prepare_statement(input_buffer, &statement)) {
			case (PREPARE_SUCCESS):
				break;
			case (PREPARE_SYNTAX_ERROR):
				printf("Syntax error. Could not parse statement.\n");
				continue;
			case (PREPARE_UNRECOGNIZED_STATEMENT):
				// Выводит надпись: Недопустимый оператор
				printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
				continue;
		}
		
		// Передает оператор в виртуальную машину
		//execute_statement(&statement);
		//printf("Executed.\n");
		switch (execute_statement(&statement, table)) {
			case (EXECUTE_SUCCESS):
				printf("Executed.\n");
				break;
			case (EXECUTE_TABLE_FULL):
				printf("Error: TAble full.\n");
				break;
		}
		
	}
}




