#ifndef SHELL_H
#define SHELL_H



// Размер буфера ввода (обрабатывает-2)
#define BUF_SIZE 255

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


PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);






#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define TABLE_MAX_PAGES 100  // лимит страниц

// Структура указывает на страницы строк и отслеживает количество строк
typedef struct {
    uint32_t num_rows;              // количество строк всего в таблице
    void* pages[TABLE_MAX_PAGES];   // массив указателей на 100 страниц
} Table;

// Перечисление результата выполнения
typedef enum {
    EXECUTE_SUCCESS, 
    EXECUTE_TABLE_FULL
} ExecuteResult;


ExecuteResult execute_statement(Statement* statement, Table* table);
ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement* statement, Table* table);
void print_row(Row* row);
void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);
void* row_slot(Table* table, uint32_t row_num);
Table* new_table();
void free_table(Table* table);



InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);
int lenString(char* str);
MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table);

#endif /*SHELL_H*/