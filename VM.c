#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Shell.h"
#include "Compiler.h"
#include "VM.h"
#include "SQLite.h"



// компактное представление строки
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = 4;  //ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = 36;    //USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = 291;       //ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

// параметры страницы строк
const uint32_t PAGE_SIZE = 4096;  // совпадает с размером страниц памяти в ОС
const uint32_t ROWS_PER_PAGE = 14;  //PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = 1400; //ROWS_PER_PAGE * TABLE_MAX_PAGES;


// Виртуальная машина обрабатывает оперторы
ExecuteResult execute_statement(Statement* statement, Table* table) {
	switch (statement->type) {
		case (STATEMENT_INSERT):
			return execute_insert(statement, table);
		case (STATEMENT_SELECT):
			return execute_select(statement, table);
	}
}


// Выполнение insert
ExecuteResult execute_insert(Statement* statement, Table* table) {
	if (table->num_rows >= TABLE_MAX_ROWS) {
		return EXECUTE_TABLE_FULL;
	}

	Row* row_to_insert = &(statement->row_to_insert);

	serialize_row(row_to_insert, row_slot(table, table->num_rows));
	table->num_rows++;

	return EXECUTE_SUCCESS;
}


// Выполнение select
ExecuteResult execute_select(Statement* statement, Table* table) {
	Row row;
	for (uint32_t i = 0; i < table->num_rows; i++) {
		deserialize_row(row_slot(table, i), &row);
		print_row(&row);
	}
	return EXECUTE_SUCCESS;
}


// Вывод на экран записи
void print_row(Row* row) {
	printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}


//
void serialize_row(Row* source, void* destination) {
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}


//
void deserialize_row(void* source, Row* destination) {
	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}


// Определяет адрес в памяти для конкретной строки
void* row_slot(Table* table, uint32_t row_num) {
	uint32_t page_num = row_num / ROWS_PER_PAGE;
	void* page = table->pages[page_num];
	if (page == NULL) {
		// Выделяем память только, когда пытаемся получить доступ к странице
		page = table->pages[page_num] = malloc(PAGE_SIZE);
	}
	uint32_t row_offset = row_num % ROWS_PER_PAGE;
	uint32_t byte_offset = row_offset * ROW_SIZE;
	return page + byte_offset;
}


// Создание новой таблицы
Table* new_table() {
	Table* table = (Table*)malloc(sizeof(Table));
	table->num_rows = 0;
	for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
		table->pages[i] = NULL;
	}
	return table;
}


// Освобождение памяти под таблицей
void free_table(Table* table) {
	for (int i = 0; table->pages[i]; i++) {
		free(table->pages[i]);
	}
	free(table);
}