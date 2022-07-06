// So, for example, `CALLBACK(on_result)(success);` goes to `if (on_result) on_result(success);`
#define CALLBACK(function) if (function) function
#define WIDGET(base, type, name) (*base->find_descendant_of_type_named<type>(name))
