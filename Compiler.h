#ifndef COMPILER_H
#define COMPILER_H

// Размеры полей в таблице
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

// Перечисление результата обработки операторов
typedef enum {
	PREPARE_SUCCESS,
	PREPARE_SYNTAX_ERROR,
	PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

// Перечисление всех операторов
typedef enum {
	STATEMENT_INSERT,
	STATEMENT_SELECT
} StatementType;

// Структура записи в таблицу для insert
typedef struct {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
} Row;

// Структура хранящая оператор
typedef struct {
	StatementType type;
	Row row_to_insert;  //используется только для insert
} Statement;


#endif /*COMPILER_H*/