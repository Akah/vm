#ifndef value_h
#define value_h

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} Value_Array;

void value_array_init(Value_Array *array);
void value_array_push(Value_Array *array, Value value);
void value_array_free(Value_Array *array);
void value_print(Value value);

#endif