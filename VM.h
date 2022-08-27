#ifndef VM_H
#define VM_H

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

#endif /*VM_H*/