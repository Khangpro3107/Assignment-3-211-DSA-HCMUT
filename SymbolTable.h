#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
enum Mode { db, quad, linear };
enum Type { num, str, any, func_any, func_str, func_void, func_num };

inline string label_to_key(string& label, int scope_level) {
    int n = label.size();
    string res = "";
    res += to_string(scope_level);
    for (int i = 0; i < n; i++) {
        int char_to_int = int(label[i] - '0');
        res += to_string(char_to_int);
    }
    return res;
}
inline int pow10_mod(int power, int table_size) {
    int res = 1;
    for (int i = 0; i < power; i++) {
        res = res * 10;
        res %= table_size;
    }
    return res;
}
inline int hash1(string& label, int table_size, int scope) {
    int res = 0, n = int(label.size()), power = 0;
    string str = label_to_key(label, scope);
    for (int i = n - 1; i >= 0; i--) {
        res += pow10_mod(power, table_size) * int(str[i] - '0');
        power++;
        res %= table_size;
    }
    return res;
}
inline int hash1(string& key, int table_size) {
    int res = 0, n = int(key.size()), power = 0;
    for (int i = n - 1; i >= 0; i--) {
        res += pow10_mod(power, table_size) * int(key[i] - '0');
        power++;
        res %= table_size;
    }
    return res;
}
inline int hash2(string& label, int table_size, int scope) {
    string str = label_to_key(label, scope);
    if (table_size == 2) throw invalid_argument("Table size is 2?");
    return 1 + hash1(label, table_size - 2, scope);
}
inline int hash2(string& key, int table_size) {
    if (table_size == 2) throw invalid_argument("Table size is 2?");
    return 1 + hash1(key, table_size - 2);
}
inline int linear_probing(string& key, int i, int table_size, int hash_const1) {
    int res = hash1(key, table_size) % table_size;
    int res1 = hash_const1 % table_size;
    res1 *= (i % table_size);
    res += res1;
    res %= table_size;
    return res;
}
inline int quadratic_probing(string& key, int i, int table_size, int hash_const1, int hash_const2) {
    int res = hash1(key, table_size) % table_size;
    int res1 = hash_const1 % table_size;
    res1 *= (i % table_size);
    res1 %= table_size;
    int res2 = hash_const2 % table_size;
    res2 *= (i % table_size);
    res2 *= (i % table_size);
    res2 %= table_size;
    res = res + res1 + res2;
    res %= table_size;
    return res;
}
inline int double_probing(string& key, int i, int table_size, int hash_const1) {
    int res = hash1(key, table_size) % table_size;
    int res1 = hash2(key, table_size) % table_size;
    res1 *= (hash_const1 % table_size);
    res1 *= (i % table_size);
    res = (res + res1) % table_size;
    return res;
}
class Regex {
public:
    string regexString = "^'[a-zA-Z0-9 ]*'$";
    string regexNumber = "^[0-9]+$";
    string regexIdentifier = "^[a-z][a-zA-Z0-9_]*$";
    string regexFunctionCall = "^[a-z][a-zA-Z0-9_]*\\([a-zA-Z0-9 _,']*\\)$";
    Regex() {}
    bool isString(string s)
    {
        return regex_match(s, regex(regexString));
    }
    bool isNumber(string s)
    {
        return regex_match(s, regex(regexNumber));
    }
    bool isIdentifier(string s)
    {
        return regex_match(s, regex(regexIdentifier));
    }
    bool isFunctionCall(string s)
    {
        return regex_match(s, regex(regexFunctionCall));
    }
};

template<class T>
struct LL
{
    struct node
    {
        T data;
        node* next;
        node(T data, node* next = NULL) {
            this->data = data;
            this->next = next;
        }
    };
    node* head;
    node* tail;
    int count;
    LL() {
        head = NULL;
        tail = NULL;
        count = 0;
    }

    void push_front(node* added) {
        if (!head) {
            head = added;
            tail = added;
            count++;
            return;
        }
        added->next = head;
        head = added;
        count++;
    }
    void push_front(T data) {
        node* added = new node(data);
        push_front(added);
    }
    void pop_front()
    {
        if (!head) return;
        if (count == 1) {
            delete head;
            head = NULL;
            tail = NULL;
            count = 0;
            return;
        }
        node* temp = head;
        head = head->next;
        temp->next = NULL;
        delete temp;
        count--;
        return;
    }
    void push_back(node* added) {
        if (!head) {
            head = added;
            tail = added;
            count++;
            return;
        }
        tail->next = added;
        tail = added;
        count++;
    }
    void push_back(T data) {
        node* added = new node(data);
        push_back(added);
    }
    void copy(LL<T>& src) {
        node* i = src.head;
        while (i) {
            this->push_back(i->data);
            i = i->next;
        }
    }
    T operator[](int i) {
        if (i == 0) return head->data;
        if (i == count - 1) return tail->data;
        int j = 0;
        node* ite = head;
        while (j < i) {
            j++;
            ite = ite->next;
        }
        return ite->data;
    }
    T rear() {
        return tail->data;
    }
    T front() {
        return head->data;
    }
    bool contains(string name) {
        node* i = this->head;
        while (i) {
            if (i->data.name == name) return true;
            i = i->next;
        }
        return false;
    }
    void copy_to_arr(T* arr) {
        int i = 0;
        node* ite = head;
        if (count == 0) return;
        for (; i < count; i++) {
            arr[i] = ite->data;
            ite = ite->next;
        }
    }
    void copy_from_arr(T* arr) {
        int i = 0;
        node* ite = head;
        if (count == 0) return;
        for (; i < count; i++) {
            ite->data = arr[i];
            ite = ite->next;
        }
    }
    void destroy() {
        if (count == 0) return;
        node* prev = head;
        node* cur = head->next;
        while (prev && cur) {
            delete prev;
            prev = cur;
            cur = cur->next;
        }
        delete prev;
        prev = NULL;
        count = 0;
    }
    bool valid_params() {
        if (count == 0) return true;
        Regex my_regex = Regex();
        node* i = head;
        while (i) {
            if (i->data == "string" || i->data == "number") return false;
            if (!my_regex.isNumber(i->data) && !my_regex.isString(i->data) && !my_regex.isIdentifier(i->data)) return false;
            i = i->next;
        }
        return true;
    }
};

class Symbol {
public:
    string name;
    int scope;
    Type type;
    LL<Type> parameters_type;
    int num_of_parameters;
    Symbol(string name = "", int scope = 0, Type type = any) {
        this->name = name;
        this->scope = scope;
        this->type = type;
        parameters_type = LL<Type>();
        num_of_parameters = 0;
    }
    void operator = (const Symbol& other) {
        this->name = other.name;
        this->scope = other.scope;
        this->type = other.type;
        this->num_of_parameters = other.num_of_parameters;
    }
    bool operator == (const Symbol& other) {
        return other.name == this->name && other.scope == this->scope;
    }
};
inline bool check_spaces_number(const string& str) {
    int cnt = 0;
    for (size_t i = 0; i < str.length() && str[i] != '\''; i++) {
        if (str[i] == ' ') cnt++;
    }
    switch (str[0]) {
    case 'I': return cnt == 1 || cnt == 2;
    case 'A': return cnt > 1;
    case 'L': return cnt == 1;
    case 'B': return cnt == 0;
    case 'E': return cnt == 0;
    case 'P': return cnt == 0;
    case 'C': return cnt >= 1;
    default: return false;
    }
}
inline bool split(string str, string* result) {
    Regex my_regex = Regex();
    if (!check_spaces_number(str)) return false;
    for (int i = 0; i < 5; i++) {
        result[i] = "";
    }
    if (str[0] == 'I') {
        size_t space1 = str.find_first_of(' ');
        size_t space2 = str.find_last_of(' ');
        if (space1 == space2) {
            result[0] = str.substr(0, space1);
            result[1] = str.substr(space2 + 1);
        }
        else {
            result[0] = str.substr(0, space1);
            result[2] = str.substr(space2 + 1);
            result[1] = str.substr(space1 + 1, space2 - space1 - 1);
            if (!my_regex.isNumber(result[2])) return false;
        }
        if (result[0] != "INSERT" || !my_regex.isIdentifier(result[1])) return false;
        if (result[1] == "number" || result[1] == "string") return false;
    }
    else if (str[0] == 'A') {
        size_t space1 = str.find_first_of(' ');
        string temp_str = str.substr(space1 + 1);
        size_t space2 = temp_str.find_first_of(' ');
        result[0] = str.substr(0, space1);
        result[1] = temp_str.substr(0, space2);
        result[2] = temp_str.substr(space2 + 1);
        if (result[0] != "ASSIGN" || !my_regex.isIdentifier(result[1])) return false;
        if (result[1] == "string" || result[1] == "number") return false;
        if (result[2] == "string" || result[2] == "number") return false;
        if (!my_regex.isIdentifier(result[2]) && !my_regex.isFunctionCall(result[2]) && !my_regex.isNumber(result[2]) && !my_regex.isString(result[2])) return false;
    }
    else if (str[0] == 'L') {
        size_t space1 = str.find_first_of(' ');
        result[0] = str.substr(0, space1);
        result[1] = str.substr(space1 + 1);
        if (result[0] != "LOOKUP" || !my_regex.isIdentifier(result[1])) return false;
        if (result[1] == "number" || result[1] == "string") return false;
    }
    else if (str[0] == 'B' || str[0] == 'E' || str[0] == 'P') {
        if (str != "END" && str != "BEGIN" && str != "PRINT") return false;
        result[0] = str;
    }
    else if (str[0] == 'C') {
        size_t space1 = str.find_first_of(' ');
        result[0] = str.substr(0, space1);
        result[1] = str.substr(space1 + 1);
        if (result[0] != "CALL" || !my_regex.isFunctionCall(result[1])) return false;
    }
    else return false;
    return true;
}
inline bool split_config(string str, string* result) {
    Regex my_regex = Regex();
    int cnt = 0;
    for (unsigned int i = 0; i < str.size(); i++) {
        if (str[i] == ' ') cnt++;
    }
    if (str[0] == 'L') {
        if (cnt != 2) return false;
        size_t space1 = str.find_first_of(' ');
        size_t space2 = str.find_last_of(' ');
        result[0] = str.substr(0, space1);
        result[2] = str.substr(space2 + 1);
        result[1] = str.substr(space1 + 1, space2 - space1 - 1);
        if (result[0] != "LINEAR" || !my_regex.isNumber(result[1]) || !my_regex.isNumber(result[2])) return false;
    }
    else if (str[0] == 'Q') {
        if (cnt != 3) return false;
        size_t space1 = str.find_first_of(' ');
        size_t space3 = str.find_last_of(' ');
        result[0] = str.substr(0, space1);
        string temp_str = str.substr(space1 + 1, space3 - space1 - 1);
        size_t space2 = temp_str.find_first_of(' ');
        result[1] = temp_str.substr(0, space2);
        result[2] = temp_str.substr(space2 + 1);
        result[3] = str.substr(space3 + 1);
        if (result[0] != "QUADRATIC" || !my_regex.isNumber(result[1]) || !my_regex.isNumber(result[2]) || !my_regex.isNumber(result[3])) return false;
    }
    else if (str[0] == 'D') {
        if (cnt != 2) return false;
        size_t space1 = str.find_first_of(' ');
        size_t space2 = str.find_last_of(' ');
        result[0] = str.substr(0, space1);
        result[2] = str.substr(space2 + 1);
        result[1] = str.substr(space1 + 1, space2 - space1 - 1);
        if (result[0] != "DOUBLE" || !my_regex.isNumber(result[1]) || !my_regex.isNumber(result[2])) return false;
    }
    else return false;
    return true;
}
inline void split(const string& str, LL<string>& params, string& functionName) {
    int opening_bracket_pos = str.find_first_of('(');
    functionName = str.substr(0, opening_bracket_pos);
    int j = opening_bracket_pos + 1;
    int i = j;
    if (str[j] == ')') return;
    for (; str[i] != ')'; i++) {
        if (str[i] == ',') {
            params.push_back(str.substr(j, i - j));
            j = i + 1;
        }
    }
    params.push_back(str.substr(j, i - j));
}
class SymbolTable
{
public:
    Symbol* table;
    int current_scope, count, capacity, hash_const1, hash_const2;
    Mode mode;
    SymbolTable() {
        table = NULL;
        current_scope = 0;
        count = 0;
        capacity = 0;
        hash_const1 = 0;
        hash_const2 = 0;
        mode = linear;
    }
    bool find_in_scope(string& name, int scope, string& res_name, int& res_scope, Type& res_type, int& res_index) {
        for (int i = 0; i < capacity; i++) {
            if (table[i].name == name && table[i].scope == scope) {
                res_name = name;
                res_scope = scope;
                res_type = table[i].type;
                res_index = i;
                return true;
            }
        }
        return false;
    }
    bool find_all(string& name, string& res_name, int& res_scope, Type& res_type, int& res_index) {
        for (int scp = this->current_scope; scp >= 0; scp--) {
            if (find_in_scope(name, scp, res_name, res_scope, res_type, res_index)) return true;
        }
        return false;
    }
    void insert_symbol(string cmd, string* tokens) {
        if (tokens[2] == "") {
            int num_probe = 0;
            string key = label_to_key(tokens[1], this->current_scope);
            for (; num_probe < this->capacity * 2; num_probe++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                if (table[index].name == "") {
                    table[index] = Symbol(tokens[1], this->current_scope);
                    cout << num_probe << endl;
                    this->count++;
                    return;
                }
                else {
                    if (table[index].name == tokens[1] && table[index].scope == this->current_scope) throw Redeclared(tokens[1]);
                }
            }
            throw Overflow(cmd);
        }
        else {
            int num_probe = 0;
            string key = label_to_key(tokens[1], current_scope);
            for (; num_probe < capacity * 2; num_probe++) {
                int index = 0;
                if (mode == linear) index = linear_probing(key, num_probe, capacity, hash_const1);
                else if (mode == quad) index = quadratic_probing(key, num_probe, capacity, hash_const1, hash_const2);
                else index = double_probing(key, num_probe, capacity, hash_const1);
                if (table[index].name == "") {
                    if (current_scope != 0) throw InvalidDeclaration(tokens[1]);
                    table[index] = Symbol(tokens[1], current_scope);
                    table[index].num_of_parameters = stoi(tokens[2]);
                    table[index].type = func_any;
                    for (int i = 0; i < table[index].num_of_parameters; i++) {
                        table[index].parameters_type.push_back(any);
                    }
                    count++;
                    cout << num_probe << endl;
                    return;
                }
                else {
                    if (table[index].name == tokens[1] && table[index].scope == current_scope) throw Redeclared(tokens[1]);
                }
            }
            throw Overflow(cmd);
        }
    }
    void assign(string cmd, string* tokens) {
        Regex my_regex = Regex();
        if (my_regex.isNumber(tokens[2])) {
            int num_probe = 0;
            string res_name = "";
            int res_scope = 0, res_index = 0;
            Type res_type;
            if (!find_all(tokens[1], res_name, res_scope, res_type, res_index)) throw Undeclared(tokens[1]);
            if (res_type != any && res_type != num) throw TypeMismatch(cmd);
            string key = label_to_key(tokens[1], res_scope);
            for (; num_probe < this->capacity * 2; num_probe++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                if (res_index == index) {
                    cout << num_probe << endl;
                    table[index].type = num;
                    return;
                }
            }
        }
        else if (my_regex.isString(tokens[2])) {
            int num_probe = 0;
            string res_name = "";
            int res_scope = 0, res_index = 0;
            Type res_type;
            if (!find_all(tokens[1], res_name, res_scope, res_type, res_index)) throw Undeclared(tokens[1]);
            if (res_type != any && res_type != str) throw TypeMismatch(cmd);
            string key = label_to_key(tokens[1], res_scope);
            for (; num_probe < this->capacity * 2; num_probe++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                if (res_index == index) {
                    cout << num_probe << endl;
                    table[index].type = str;
                    return;
                }
            }
        }
        else if (my_regex.isIdentifier(tokens[2])) {
            string res_name2 = "";
            int res_scope2 = 0, res_index2 = 0;
            Type res_type2;
            if (!find_all(tokens[2], res_name2, res_scope2, res_type2, res_index2)) throw Undeclared(tokens[2]);

            string res_name1 = "";
            int res_scope1 = 0, res_index1 = 0;
            Type res_type1;
            if (!find_all(tokens[1], res_name1, res_scope1, res_type1, res_index1)) throw Undeclared(tokens[1]);

            if (res_type1 == any) {
                if (res_type2 == any) throw TypeCannotBeInferred(cmd);
                if (res_type2 == str || res_type2 == num) table[res_index1].type = res_type2;
                else throw TypeMismatch(cmd);
            }
            else if (res_type1 == str || res_type1 == num) {
                if (res_type2 == any) table[res_index2].type = res_type1;
                else throw TypeMismatch(cmd);
            }
            else throw TypeMismatch(cmd);

            int num_probe1 = 0, num_probe2 = 0;
            string key = label_to_key(tokens[1], res_scope1);
            for (; num_probe1 < this->capacity * 2; num_probe1++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key, num_probe1, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key, num_probe1, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key, num_probe1, this->capacity, this->hash_const1);
                if (res_index1 == index) {
                    break;
                }
            }
            key = label_to_key(tokens[2], res_scope2);
            for (; num_probe2 < this->capacity * 2; num_probe2++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key, num_probe2, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key, num_probe2, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key, num_probe2, this->capacity, this->hash_const1);
                if (res_index2 == index) {
                    break;
                }
            }
            cout << num_probe1 + num_probe2 << endl;
        }
        else if (my_regex.isFunctionCall(tokens[2])) {
            string res_name2 = "";
            int res_scope2 = 0, res_index2 = 0;
            Type res_type2;
            
            string functionName = "";
            LL<string> value_params = LL<string>();
            split(tokens[2], value_params, functionName);

            if (functionName == "string" || functionName == "number") {
                value_params.destroy();
                throw InvalidInstruction(cmd);
            }
            if (!value_params.valid_params()) {
                value_params.destroy();
                throw InvalidInstruction(cmd);
            }
            if (!find_all(functionName, res_name2, res_scope2, res_type2, res_index2)) {
                value_params.destroy();
                throw Undeclared(functionName);
            }
            if (res_type2 != func_any && res_type2 != func_num && res_type2 != func_str && res_type2 != func_void) {
                value_params.destroy();
                throw TypeMismatch(cmd);
            }
            int num_probe = 0;
            string key_fn = label_to_key(functionName, res_scope2);
            for (; num_probe < this->capacity * 2; num_probe++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key_fn, num_probe, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key_fn, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key_fn, num_probe, this->capacity, this->hash_const1);
                if (res_index2 == index) break;
            }
            if (table[res_index2].num_of_parameters != value_params.count) {
                value_params.destroy();
                throw TypeMismatch(cmd);
            }
            int the_smaller_size = table[res_index2].num_of_parameters;
            
            string* value_params_arr = new string[value_params.count];
            value_params.copy_to_arr(value_params_arr);
            Type* type_params_arr = new Type[table[res_index2].num_of_parameters];
            table[res_index2].parameters_type.copy_to_arr(type_params_arr);
            
            for (int i = 0; i < the_smaller_size; i++) {
                string current_value = value_params_arr[i];
                if (my_regex.isNumber(current_value)) {
                    if (type_params_arr[i] == any) {
                        type_params_arr[i] = num;
                        continue;
                    }
                    else if (type_params_arr[i] == num) continue;
                    else {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw TypeMismatch(cmd);
                    }
                }
                else if (my_regex.isString(current_value)) {
                    if (type_params_arr[i] == any) {
                        type_params_arr[i] = str;
                        continue;
                    }
                    else if (type_params_arr[i] == str) continue;
                    else {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw TypeMismatch(cmd);
                    }
                }
                else if (my_regex.isIdentifier(current_value)) {
                    string res_name = "";
                    int res_scope = 0, res_index = 0;
                    Type res_type;
                    if (!find_all(current_value, res_name, res_scope, res_type, res_index)) {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw Undeclared(current_value);
                    }
                    if (type_params_arr[i] == any) {
                        if (res_type == any) {
                            value_params.destroy();
                            delete[] type_params_arr;
                            delete[] value_params_arr;
                            throw TypeCannotBeInferred(cmd);
                        }
                        if (res_type == str || res_type == num) {
                            type_params_arr[i] = res_type;
                            int num_probe_temp = 0;
                            string key = label_to_key(res_name, res_scope);
                            for (; num_probe_temp < 2 * this->capacity; num_probe_temp++) {
                                int index = 0;
                                if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                                else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                                else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                                if (res_index == index) {
                                    num_probe += num_probe_temp;
                                    break;
                                }
                            }
                        }
                        else {
                            value_params.destroy();
                            delete[] type_params_arr;
                            delete[] value_params_arr;
                            throw TypeMismatch(cmd);
                        }
                    }
                    else if (type_params_arr[i] == num || type_params_arr[i] == str) {
                        if (res_type == any || res_type == num || res_type == str) {
                            if (res_type == any) table[res_index].type = type_params_arr[i];
                            else if (type_params_arr[i] != res_type) {
                                value_params.destroy();
                                delete[] type_params_arr;
                                delete[] value_params_arr;
                                throw TypeMismatch(cmd);
                            }
                            int num_probe_temp = 0;
                            string key = label_to_key(res_name, res_scope);
                            for (; num_probe_temp < 2 * this->capacity; num_probe_temp++) {
                                int index = 0;
                                if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                                else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                                else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                                if (res_index == index) {
                                    num_probe += num_probe_temp;
                                    break;
                                }
                            }
                        }
                        else {
                            value_params.destroy();
                            delete[] type_params_arr;
                            delete[] value_params_arr;
                            throw TypeMismatch(cmd);
                        }
                    }
                    else {
                        throw invalid_argument("This should not happen!");
                    }
                }
                else {
                    value_params.destroy();
                    delete[] value_params_arr;
                    delete[] type_params_arr;
                    throw InvalidInstruction(cmd);
                }
            }
            
            string res_name1 = "";
            int res_scope1 = 0, res_index1 = 0;
            Type res_type1;
            if (!find_all(tokens[1], res_name1, res_scope1, res_type1, res_index1)) {
                value_params.destroy();
                delete[] type_params_arr;
                delete[] value_params_arr;
                throw Undeclared(tokens[1]);
            }
            if (res_type2 == func_any) {
                if (res_type1 == any) {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeCannotBeInferred(cmd);
                }
                else if (res_type1 == num) table[res_index2].type = func_num;
                else if (res_type1 == str) table[res_index2].type = func_str;
                else {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeMismatch(cmd);
                }
            }
            else if (res_type2 == func_str || res_type2 == func_num) {
                if (res_type1 == any) {
                    if (res_type2 == func_str) table[res_index1].type = str;
                    else if (res_type2 == func_num) table[res_index1].type = num;
                }
                else if (res_type1 == num) {
                    if (res_type2 != func_num) {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw TypeMismatch(cmd);
                    }
                }
                else if (res_type1 == str) {
                    if (res_type2 != func_str) {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw TypeMismatch(cmd);
                    }
                }
                else {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeMismatch(cmd);
                }
            }
            else if (res_type2 == func_void) {
                value_params.destroy();
                delete[] value_params_arr;
                delete[] type_params_arr;
                throw TypeMismatch(cmd);
            }
            else {
                throw invalid_argument("This should not happen!");
            }
            int num_probe_temp = 0;
            string key1 = label_to_key(res_name1, res_scope2);
            for (; num_probe_temp < 2 * this->capacity; num_probe_temp++) {
                int index = 0;
                if (this->mode == linear) index = linear_probing(key1, num_probe, this->capacity, this->hash_const1);
                else if (this->mode == quad) index = quadratic_probing(key1, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                else index = double_probing(key1, num_probe, this->capacity, this->hash_const1);
                if (index == res_index1) {
                    num_probe += num_probe_temp;
                    break;
                }
            }
            table[res_index2].parameters_type.copy_from_arr(type_params_arr);
            cout << num_probe << endl;
            value_params.destroy();
            delete[] type_params_arr;
            delete[] value_params_arr;
        }
    }
    void call(string cmd, string* tokens) {
        Regex my_regex = Regex();
        string functionName = "";
        LL<string> value_params = LL<string>();
        split(tokens[1], value_params, functionName);
        if (functionName == "string" || functionName == "number") {
            value_params.destroy();
            throw InvalidInstruction(cmd);
        }
        if (!value_params.valid_params()) {
            value_params.destroy();
            throw InvalidInstruction(cmd);
        }
        string res_name = "";
        int res_index = 0, res_scope = 0;
        Type res_type = any;
        if (!find_all(functionName, res_name, res_scope, res_type, res_index)) {
            value_params.destroy();
            throw Undeclared(functionName);
        }
        if (res_type != func_any && res_type != func_void) {
            value_params.destroy();
            throw TypeMismatch(cmd);
        }
        if (res_type == func_any) table[res_index].type = func_void;

        string key_fn = label_to_key(functionName, res_scope);
        int num_probe = 0;
        for (; num_probe < 2 * this->capacity; num_probe++) {
            int index = 0;
            if (this->mode == linear) index = linear_probing(key_fn, num_probe, this->capacity, this->hash_const1);
            else if (this->mode == quad) index = quadratic_probing(key_fn, num_probe, this->capacity, this->hash_const1, this->hash_const2);
            else index = double_probing(key_fn, num_probe, this->capacity, this->hash_const1);
            if (index == res_index) break;
        }
        if (table[res_index].num_of_parameters != value_params.count) {
            value_params.destroy();
            throw TypeMismatch(cmd);
        }
        string* value_params_arr = new string[value_params.count];
        value_params.copy_to_arr(value_params_arr);
        Type* type_params_arr = new Type[table[res_index].num_of_parameters];
        table[res_index].parameters_type.copy_to_arr(type_params_arr);
        int the_smaller_size = table[res_index].num_of_parameters;

        for (int i = 0; i < the_smaller_size; i++) {
            string current_value = value_params_arr[i];
            if (my_regex.isNumber(current_value)) {
                if (type_params_arr[i] == any || type_params_arr[i] == num) type_params_arr[i] = num;
                else {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeMismatch(cmd);
                }
            }
            else if (my_regex.isString(current_value)) {
                if (type_params_arr[i] == any || type_params_arr[i] == str) type_params_arr[i] = str;
                else {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeMismatch(cmd);
                }
            }
            else if (my_regex.isIdentifier(current_value)) {
                int num_probe_temp = 0;
                string res_name_para = "";
                int res_index_para = 0, res_scope_para = 0;
                Type res_type_para = any;
                if (!find_all(current_value, res_name_para, res_scope_para, res_type_para, res_index_para)) {
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw Undeclared(current_value);
                }
                if (res_type_para == any) {
                    if (type_params_arr[i] == any) {
                        value_params.destroy();
                        delete[] type_params_arr;
                        delete[] value_params_arr;
                        throw TypeCannotBeInferred(cmd);
                    }
                    else if (type_params_arr[i] == num || type_params_arr[i] == str) {
                        table[res_index_para].type = type_params_arr[i];
                    }
                    else throw invalid_argument("This should not happen!");
                }
                else if (res_type_para == num || res_type_para == str) {
                    if (type_params_arr[i] == any) type_params_arr[i] = res_type_para;
                    else if (type_params_arr[i]==num||type_params_arr[i]==str){
                        if (type_params_arr[i] != res_type_para) {
                            value_params.destroy();
                            delete[] type_params_arr;
                            delete[] value_params_arr;
                            throw TypeMismatch(cmd);
                        }
                    }
                    else throw invalid_argument("This should not happen!");
                }
                else { 
                    value_params.destroy();
                    delete[] type_params_arr;
                    delete[] value_params_arr;
                    throw TypeMismatch(cmd);
                }
                string key = label_to_key(res_name_para, res_scope_para);
                for (; num_probe_temp < 2 * this->capacity; num_probe_temp++) {
                    int index = 0;
                    if (this->mode == linear) index = linear_probing(key, num_probe, this->capacity, this->hash_const1);
                    else if (this->mode == quad) index = quadratic_probing(key, num_probe, this->capacity, this->hash_const1, this->hash_const2);
                    else index = double_probing(key, num_probe, this->capacity, this->hash_const1);
                    if (index == res_index_para) {
                        num_probe += num_probe_temp;
                        break;
                    }
                }
            }
            else {
                value_params.destroy();
                delete[] type_params_arr;
                delete[] value_params_arr;
                throw InvalidInstruction(cmd);
            }
        }
        table[res_index].parameters_type.copy_from_arr(type_params_arr);
        cout << num_probe << endl;
        value_params.destroy();
        delete[] value_params_arr;
        delete[] type_params_arr;
    }
    void begin() {
        this->current_scope++;
    }
    void end() {
        if (this->current_scope == 0) throw UnknownBlock();
        for (int i = 0; i < this->capacity && this->count >= 0; i++) {
            if (table[i].scope == this->current_scope) {
                table[i].parameters_type.destroy();
                table[i] = Symbol();
                this->count--;
            }
        }
        this->current_scope--;
    }
    void look_up(string cmd, string* tokens) {
        string res_name = "";
        int res_scope = 0, res_index = 0;
        Type res_type = any;
        if (!find_all(tokens[1], res_name, res_scope, res_type, res_index)) throw Undeclared(tokens[1]);
        cout << res_index << endl;
    }
    void print() {
        if (this->count == 0) return;
        int cnt = 0;
        for (int i = 0; i < this->capacity; i++) {
            if (table[i].name != "") {
                cnt++;
                if (cnt == this->count) {
                    cout << i << ' ' << table[i].name << "/" << "/" << table[i].scope;
                    break;
                }
                else cout << i << ' ' << table[i].name << "/" << "/" << table[i].scope << ';';
            }
        }
        cout << endl;
    }
    bool unclosed() {
        return this->current_scope != 0;
    }
    inline void run(string filename);
    ~SymbolTable() {
        for (int i = 0; i < capacity; i++) {
            table[i].parameters_type.destroy();
            table[i] = Symbol();
        }
        count = 0;
        capacity = 0;
        delete[] table;
    }
};
#endif