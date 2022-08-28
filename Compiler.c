#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Shell.h"
#include "Compiler.h"
#include "VM.h"
#include "SQLite.h"



// Получает оператор из буфера ввода
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
	if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
		// Вызывает специальный обработчик
		return prepare_insert(input_buffer, statement);
		/*statement->type = STATEMENT_INSERT;
		int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", 
			&(statement->row_to_insert.id), statement->row_to_insert.username, 
			statement->row_to_insert.email);
		if (args_assigned < 3) {
			return PREPARE_SYNTAX_ERROR;
		}
		return PREPARE_SUCCESS;*/
	}
	if (strcmp(input_buffer->buffer, "select") == 0) {
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZED_STATEMENT;
}


// Обрабатываем insert
PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
	statement->type = STATEMENT_INSERT;

	// Распарсиваем строку с командой insert и аргументами
	char* keyword = strtok(input_buffer->buffer, " ");
	char* id_string = strtok(NULL, " ");
	char* username = strtok(NULL, " ");
	char* email = strtok(NULL, " ");

	// проверяем на неправильный ввод аргументов
	if (id_string == NULL || username == NULL || email == NULL) {
		return PREPARE_SYNTAX_ERROR;
	}

	int id = atoi(id_string);
	if (strlen(username) > COLUMN_USERNAME_SIZE) {
		return PREPARE_STRING_TOO_LONG;
	}
	if (strlen(email) > COLUMN_EMAIL_SIZE) {
		return PREPARE_STRING_TOO_LONG;
	}

	statement->row_to_insert.id = id;
	strcpy(statement->row_to_insert.username, username);
	strcpy(statement->row_to_insert.email, email);

	return PREPARE_SUCCESS;
}