#include <iostream>
#include <fstream>
#include <vector>
#include <map>

int const num_bit = 8;
int const num_bytes = 32;
bool flag_is_memory = false;
std::map<std::string, int> met_table;
std::string* info;
int count_istructions = 0;


class int1 {
    private:
        unsigned int m_value;
    public:
        int1();
        int1(unsigned int);
        void operator= (unsigned int);
        void set_value(unsigned int);
        friend int1 operator+ (const int1&, const int1&);
        unsigned int get_value () const;
};

std::vector<int1*> dynamic_mem;

class ins_par{
    protected:
        std::string str_info_about_instruction;
    public:
        virtual bool getVal() const = 0;
        virtual void run() = 0;
        virtual std::string getInfo() const = 0;
        virtual void setInfo(const std::string&) = 0;
};

class ins: public ins_par{
    private:
        int1 *operand1;
        int1 *operand2;
        std::string name;
        bool val;
        void (*function)(int1*, int1*);
    public:
        ins (const std::string&, const std::string&, const std::string&);
        bool getVal() const override;
        void run() override;
        std::string getInfo() const override;
        void setInfo(const std::string&) override;
};

class unar_ins_JUMP: public ins_par {
    private:
        std::string operand;
        std::string name;
        bool val;
        void (*function)(const std::string&);
    public:
        unar_ins_JUMP (const std::string&, const std::string&); 
        bool getVal() const override;
        void run() override;
        std::string getInfo() const override;
        void setInfo(const std::string&) override;
};

class unar_ins_not : public ins_par {
    private:
        int1 *operand;
        bool val;
        void (*function)(int1*);
    public:
        unar_ins_not (const std::string&);    
        bool getVal() const override;
        void run() override;
        std::string getInfo() const override;
        void setInfo(const std::string&) override;
};

class dumpMemory : public ins_par {
    private:
        void (*function)();
    public:
        dumpMemory ();
        bool getVal() const override{ return true; };
        void run() override;
        std::string getInfo() const override;
        void setInfo(const std::string&) override;
};

bool check_is_num_or_not(const std::string& str){
    for (int i = 0;i < str.length();++i){
        if (!(str[i] >= '0' && str[i] <= '9')){
            return false;
        }
    }
    return true;
}

int char_to_int (char ch) {
    return ((int)ch - (int)'0');
}

unsigned int string_to_number(const std::string& str){
    unsigned int result = 0;
    unsigned int log10 = 1;
    for (int i = str.length() - 1;i >= 0;--i){
        result += log10 * char_to_int(str[i]);
        log10 *= 10;
    }
    return result;
}

bool check_is_address_or_not(const std::string& str) {
    if (str[str.length() - 1] == ']' && str[0] == '[') {
        std::string new_without_brackets = "";
        for (int i = 1;i < str.length() - 1;++i) {
            new_without_brackets += std::string(1, str[i]);
        }
        if (check_is_num_or_not(new_without_brackets)) {
            unsigned int num = string_to_number(new_without_brackets);
            if (num >= count_istructions && num < num_bytes) {        
                return true;
            }
            else {
                std::cout << "The problem is that it is not possible to change the area where the program code is installed" << std::endl;
            }
        }
    }
    return false;
}

int string_to_address (const std::string& address) {
    std::string new_without_brackets = "";
    for (int i = 1;i < address.length() - 1;++i) {
        new_without_brackets += std::string(1, address[i]);
    }
    return string_to_number(new_without_brackets); 
}

int* comma_detection_form_word(const std::string& word) {
    int count_comma = 0;
    for (int i = 0;i < word.size();++i) {
        if (word[i] == ',') {
            ++count_comma;
        }
    }
    int* result = new int[count_comma + 1];
    int j = 0;
    for (int i = 0;i < word.size();++i) {
        if (word[i] == ','){
            result[j++] = i;
        }
    }
    result[j] = word.size() * word.size();
    return result;
}

int pow (int n){
    int result = 1;
    for (int i = 0;i < n;++i){
        result *= 2;
    }
    return result;
}

int value_give_memory(unsigned int value) {
    int result = 0;
    //CF 4
    if ((value & pow(8)) == pow(8)) {
        ++result; 
    }
    result <<= 1;
    //ZF 3
    if (value == 0) {
        ++result;
    }
    result <<= 1;
    //SF 2
    if ((value & pow(7)) == pow(7)) {
        ++result;
    }
    result <<= 1;
    //OF 1
    if ((~(pow(8) - 1) & value) > 0) {
        ++result;
    }
    result <<= 1;
    //PF 0
    bool PF = true;
    int count_1 = 0;
    int i = 0;
    while (i < 8) {
        if ((value & 1) == 1) {
            ++count_1;
        }
        value >>= 1;
        ++i;
    }
    PF = !(count_1 % 2);// zuyg True, kent False
    if (PF) {
        ++result;
    }
    return result;
}

int1 AYB;
int1 BEN;
int1 GIM;
int1 DA;
int1 ECH;
int1 ZA;
int1 GH;

int1 MEMORY[num_bytes];

//instractions

void MOV (int1* oper1, int1* oper2) {
    *oper1 = (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void ADD (int1* oper1, int1* oper2) {
    *oper1 = (*oper1).get_value() + (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void SUB (int1* oper1, int1* oper2) {
    *oper1 = (*oper1).get_value() - (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void MUL (int1* oper1, int1* oper2) {
    *oper1 = (*oper1).get_value() * (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void DIV (int1* oper1, int1* oper2) {
    if ((*oper2).get_value() == 0) std::cerr << "Zero division error!!" << std::endl;
    *oper1 = (*oper1).get_value() / (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void AND (int1* oper1, int1* oper2) {
    *oper1 = (*oper1).get_value() & (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void OR (int1* oper1, int1* oper2) {
    *oper1 = (*oper1).get_value() | (*oper2).get_value();
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = *oper1;
}

void NOT (int1* oper) {
    *oper = ~((*oper).get_value());
    ZA.set_value(value_give_memory((*oper).get_value()));
    AYB = *oper;
}

void CMP (int1* oper1, int1* oper2) {
    if ((*oper1).get_value() < (*oper2).get_value()) {
        DA = 0;//ays jamanak ashxatum e JL instrution-y
    } 
    else if ((*oper1).get_value() > (*oper2).get_value()) {
        DA = 1;//ays jamanak ashxatum e JG instruction-y 
    } 
    else {
        DA = 2;//hamapat. ays jamanak JE 
    } 
    ZA.set_value(value_give_memory((*oper1).get_value()));
    AYB = DA;
}

void JMP (const std::string& name) {   
    GH = met_table[name];
}

void JG (const std::string& name) { 
    if (DA.get_value() == 1){
        JMP(name);
    }
}

void JE (const std::string& name) { 
    if (DA.get_value() == 2){
        JMP(name);
    }
}

void JL (const std::string& name) { 
    if (DA.get_value() == 0){
        JMP(name);
    }
}

bool check_unar_operand_valid_or_not (const std::string& str){
    if (check_is_num_or_not(str)) {
        return false;
    }
    else {
        if (check_is_address_or_not(str)) {
            return false;
        }
        else {
            if (
                str == "AYB" ||
                str == "BEN" ||
                str == "GIM" ||
                str == "DA" ||
                str == "ECH"
            )
            {
                return true;
            }
            else{
                return false;
            }
        }
    }
}

bool check_operand2_valid_or_not (const std::string& str){
    if (check_is_num_or_not(str)) {
        return true;
    }
    else {
        if (check_is_address_or_not(str)) {
            return true;
        }
        else {
            if (
                str == "AYB" ||
                str == "BEN" ||
                str == "GIM" ||
                str == "DA" ||
                str == "ECH"
            )
            {
                return true;
            }
            else{
                return false;
            }
        }
    }
}

bool check_operand1_valid_or_not (const std::string& str){
    if (check_is_num_or_not(str)) {
        return false;
    }
    else {
        if (check_is_address_or_not(str)) {
            return true;
        }
        else {
            if (
                str == "AYB" ||
                str == "BEN" ||
                str == "GIM" ||
                str == "DA" ||
                str == "ECH"
            )
            {
                return true;
            }
            else{
                return false;
            }
        }
    }
}


std::string operand_value_type (const std::string& str){
    if (check_is_num_or_not(str)) {
        return "literal";
    }
    else {
        if (check_is_address_or_not(str)) {
            return "address";
        }
        else {
            if (
                str == "AYB" ||
                str == "BEN" ||
                str == "GIM" ||
                str == "DA" ||
                str == "ECH"
            )
            {
                return "registor";
            }
        }
    }
    return "undefined";
}

int1* operand_registor_name (const std::string& str){
    if (str == "AYB") {
        return &AYB;
    }
    else if (str == "BEN") {
        return &BEN;
    }
    else if (str == "GIM") {
        return &GIM;
    }
    else if (str == "DA") {
        return &DA;
    }
    else {
        return &ECH;
    }
}

bool check_unar (int1** operand, const std::string& operand_text) {
    if (check_unar_operand_valid_or_not(operand_text)) {
        if (operand_value_type(operand_text) == "registor") {
            *operand = operand_registor_name(operand_text);
        }
        else {
            std::cout << "Error: Cannot have two memory type operands at the same time!!" << std::endl;
            return false;
        }
    }
    else {
        return false;
    }
    return true;
}

std::vector<std::string> split (const std::string& str){ 
    std::string current = "";
    std::vector<std::string> result;
    for (int i = 0;i < str.length();++i){
        if (str[i] != ':'){
            current = current + std::string(1, str[i]);
        }
        else {
            if (current != "")
                result.push_back(current);
            result.push_back(":");
            current = "";
        }
    }
    if (current != ""){
        result.push_back(current);
    }
    return result;
}

bool check_met_valid_or_not(const std::string& str, const std::map<std::string, int>& table){
    for (const auto& i: table) {
        if (i.first == str){
            return false;
        }
    }
    return true;
}

bool check_met (const std::string& met_name) {
    for (const auto& i: met_table) {
        if (met_name == i.first) {
            return true;
        }
    }
    return false;
}

void print_dump () {
    std::cout << "---------MEMORY---------" << std::endl;
    for (int i = 0;i < num_bytes;++i) {
        std::cout << "------------------------" << std::endl;
        if (i < count_istructions)
            std::cout << "|" << i << "\t | " << info[i] << std::endl;
        else
            std::cout << "|" << i << "\t | " << MEMORY[i].get_value() << std::endl;
        std::cout << "------------------------" << std::endl;
    }
}

int main(int argc, char** args) {
    if (argc == 1) {
        std::cout << "Error: file not specified" << std::endl;
        return 0;
    }
    std::string file_name = args[1];
    std::ifstream file;
    file.open(file_name);
    std::vector<std::string> tokenization_identifiers = {};//this is where the final tokenization results will be stored
    if (!file.is_open()){
        std::cerr << "Error: .. file" << std::endl;
        return 0;
    }
    else {
        
        //break by spaces and line breaks

        std::vector<std::string> str = {""};
        char ch = 0;
        while (file.read(&ch, sizeof(char))) {
            if ((ch != ' ') && (ch != '\n')) {
                str.back() = str.back() + std::string(1, ch);
            }
            else {
                if (str.back() != ""){
                    str.push_back("");
                }
            }
        }

        str.pop_back();

        //division by commas because they determine the main operands, etc
        
        std::vector<std::string> comman_tok_str = {}; 
        
        for (int i = 0;i < str.size();++i) {
            std::vector<std::string> str_separated = {};
            for (int j = 0;j < str[i].length();++j) {
                if (str[i][j] != ',') {
                    if (str_separated.size() == 0){
                        str_separated.push_back("");
                    }
                    str_separated.back() = str_separated.back() + std::string(1, str[i][j]);
                }
                else {
                    if (j != str[i].length() - 1) {
                        str_separated.push_back(",");
                        str_separated.push_back("");
                    }
                    else {
                        str_separated.push_back(",");
                    }
                }
            }
            for (int j = 0;j < str_separated.size();++j){
                comman_tok_str.push_back(str_separated[j]);
            }
        }
        


        //forbidden symbols in MASM assembly !@#$%^&*()-+={}[]\|;'"<>./?

        std::string symbols = "!@#$%^&*()-+={}\\|;'\"<>./?";
        
        for (int i = 0;i < comman_tok_str.size();++i) {
            for (int j = 0;j < comman_tok_str[i].size();++j) {
                for (int k = 0;k < symbols.length();++k) {
                    if (comman_tok_str[i][j] == symbols[k]){
                        std::cout << "Syntax Error!!The code will contain extra characters" << std::endl;
                        return 0;
                    }
                }
            }
        }

        //separation of parts to access memory by address
        
        std::vector<std::string> address_tok_str = {};

        for (int i = 0;i < comman_tok_str.size();++i) {
            std::vector<std::string> new_identifier_separate = {""};  
            for (int j = 0;j < comman_tok_str[i].length();++j) {
                if (comman_tok_str[i][j] != '[') {
                    new_identifier_separate.back() = new_identifier_separate.back() + std::string(1, comman_tok_str[i][j]);
                }
                else {
                    if (new_identifier_separate.back() != ""){
                        new_identifier_separate.push_back("[");
                    }
                    else {
                        new_identifier_separate.back() = new_identifier_separate.back() + std::string(1, comman_tok_str[i][j]);
                    }
                }
                if (comman_tok_str[i][j] == ']'){
                    if (j != comman_tok_str[i].length() - 1){
                        new_identifier_separate.push_back("");
                    }
                }
            }
            for (int j = 0;j < new_identifier_separate.size();++j) {
                address_tok_str.push_back(new_identifier_separate[j]);
            }
        }
        tokenization_identifiers = address_tok_str;
    }

    //detection of met
    std::vector<std::string> tokenization_identifiers_new = {};  

    for (int i = 0;i < tokenization_identifiers.size();++i) {
        std::vector<std::string> current = split(tokenization_identifiers[i]);
        for (auto i: current){
            tokenization_identifiers_new.push_back(i);
        }
    }
    
    tokenization_identifiers = tokenization_identifiers_new;

    //semantic analysis

    //<mov/add/sub/mul/div/and/or> register , register(literal)
    //<mov/add/sub/mul/div/and/or> register , address(literal)
    //<mov/add/sub/mul/div/and/or> address , register(literal)
    //not register
    //cmp <address/register> , <address/register> 
    //<jpm/jg/jl/je> met
    
    //creting token table 

    std::map<int, std::vector<std::string>> token_table;

    int id = 0;
    for (int i = 0;i < tokenization_identifiers.size();++i) {
        std::vector<std::string> current_instruction = {};
        std::string tok_current_name = tokenization_identifiers[i];
        if (
            tok_current_name == "MOV" ||
            tok_current_name == "ADD" ||
            tok_current_name == "SUB" ||
            tok_current_name == "MUL" ||
            tok_current_name == "DIV" ||
            tok_current_name == "AND" ||
            tok_current_name == "OR"  ||
            tok_current_name == "CMP"  
            )
        {
            current_instruction.push_back(tok_current_name);
            if (!(i + 3 < tokenization_identifiers.size())) {
                std::cout << "Error: Operands are missing";
                return 0;
            }
            current_instruction.push_back(tokenization_identifiers[i + 1]);
            current_instruction.push_back(tokenization_identifiers[i + 2]);
            current_instruction.push_back(tokenization_identifiers[i + 3]);
            i += 3;
        }
        else if (
                tokenization_identifiers[i] == "NOT"||
                tokenization_identifiers[i] == "JMP"||
                tokenization_identifiers[i] == "JG" ||
                tokenization_identifiers[i] == "JE" ||
                tokenization_identifiers[i] == "JL" 
                ) {  
            if (!(i + 1 < tokenization_identifiers.size())) {
                std::cout << "Error: Operands are missing";
                return 0;
            }
            current_instruction.push_back(tok_current_name);
            current_instruction.push_back(tokenization_identifiers[i + 1]);
            i += 1; 
        }
        else if (tokenization_identifiers[i] == "dumpMemory") {
            current_instruction.push_back(tok_current_name);
        }
        else if (((i + 1) < tokenization_identifiers.size()) && tokenization_identifiers[i + 1] == ":") {
            if (check_met_valid_or_not(tokenization_identifiers[i], met_table)){
                met_table[tokenization_identifiers[i]] = token_table.size();
            }
            else {
                std::cout << tokenization_identifiers[i] << ": Such deviations have already been defined before (Error!!)" << std::endl;
                return 0;
            }
            i += 1;
            continue;
        }
        else {
            std::cout << "Erroooooor!!! Sorry, but no such commands exist" << "(" << tokenization_identifiers[i] << ")" << std::endl;
            return 0;
        }
        token_table[id++] = current_instruction;
    }
   
    //instruction generation
    
    count_istructions = token_table.size(); 

    std::map<int, ins_par*> instructions;
    info = new std::string[count_istructions];
    int t = 0;
    for (int i = 0;i < token_table.size();++i) {
        std::string tok_current_name = token_table[i][0];
        if (
            tok_current_name == "MOV" ||
            tok_current_name == "ADD" ||
            tok_current_name == "SUB" ||
            tok_current_name == "MUL" ||
            tok_current_name == "DIV" ||
            tok_current_name == "AND" ||
            tok_current_name == "OR"  ||
            tok_current_name == "CMP"  
            ) {
            ins* current_ins = new ins(token_table[i][1], token_table[i][3], token_table[i][0]);
            if (!(*current_ins).getVal()){
                return 0;
            }
            current_ins->setInfo (token_table[i][0] + " " + token_table[i][1] + token_table[i][2] + " " + token_table[i][3]);
            info[t++] = current_ins->getInfo(); 
            instructions[i] = current_ins;
        }
        else if (
                tok_current_name == "JMP"||
                tok_current_name == "JG" ||
                tok_current_name == "JE" ||
                tok_current_name == "JL" 
                ) { 
            unar_ins_JUMP* current_ins = new unar_ins_JUMP(token_table[i][1], token_table[i][0]);
            if (!(*current_ins).getVal()){
                return 0;
            }
            current_ins->setInfo (token_table[i][0] + " " + token_table[i][1]);
            info[t++] = current_ins->getInfo(); 
            instructions[i] = current_ins;
        }
        else if (tok_current_name == "NOT") { 
            unar_ins_not* current_ins = new unar_ins_not(token_table[i][1]);
            if (!(*current_ins).getVal()){
                return 0;
            }
            current_ins->setInfo (token_table[i][0] + " " + token_table[i][1]);
            info[t++] = current_ins->getInfo(); 
            instructions[i] = current_ins;
        }
        else if (tok_current_name == "dumpMemory") {
            dumpMemory* current_ins = new dumpMemory;
            current_ins->setInfo (token_table[i][0]);
            info[t++] = current_ins->getInfo(); 
            instructions[i] = current_ins;
        }
        else {
            std::cout << "instruction: " << tok_current_name << std::endl;
            std::cout << "Error: No such instructions exist!" << std::endl;
            return 0;
        }
    }
        
    while (GH.get_value() < instructions.size()){
        instructions[GH.get_value()]->run(); 
        GH = GH + 1;
        std::cout << "---------INSTRUCTION("<< GH.get_value() <<")-----" << std::endl;
        std::cout << "---------REGISTORS-------" << std::endl;
        std::cout << "AYB: " << AYB.get_value() << std::endl;
        std::cout << "BEN: " << BEN.get_value() << std::endl;
        std::cout << "GIM: " << GIM.get_value() << std::endl;
        std::cout << "DA: " << DA.get_value() << std::endl;
        std::cout << "ECH: " << ECH.get_value() << std::endl;
        std::cout << "ZA: " << ZA.get_value() << std::endl;
        std::cout << "GH: " << GH.get_value() << std::endl;
        std::cout << "----------------------------" << std::endl;
    }
    
    for (int i = 0;i < instructions.size();++i) {
        delete instructions[i];
    }
    
    for (int i = 0;i < dynamic_mem.size();++i){
        delete dynamic_mem[i];
    }

    return 0;
}


//int1 

void int1::set_value(unsigned int value){
    m_value = value;
}


int1::int1 () 
    :m_value(0)
{}

int1::int1 (unsigned int value) {
    m_value = value & (pow(8) - 1);
}

void int1::operator= (unsigned int value) {
    m_value = value & (pow(8) - 1);
}

int1 operator+ (const int1& obj1, const int1& obj2) {
    int1 result; 
    result = obj1.get_value() + obj2.get_value();
    return result;
}

unsigned int int1::get_value() const {
    return m_value;
}

//instraction 

ins::ins (const std::string& operand_text1, const std::string& operand_text2, const std::string& instruction_name)
    :name(instruction_name)
{
    bool flag_cmp = false;
    if (instruction_name == "MOV") {
        function = MOV;
    }
    else if (instruction_name == "ADD") {
        function = ADD;
    }
    else if (instruction_name == "SUB") {
        function = SUB;
    }
    else if (instruction_name == "DIV") {
        function = DIV;
    }
    else if (instruction_name == "MUL") {
        function = MUL;
    }
    else if (instruction_name == "AND") {
        function = AND;
    }
    else if (instruction_name == "OR") {
        function = OR;
    }
    else if (instruction_name == "CMP") {
        flag_cmp = true;
        function = CMP;
    }
    else {
        val = false;
        std::cout << "Erroor: No such command exists!!" << std::endl;
        return;
    }
    if (check_operand2_valid_or_not(operand_text2)) {
        if (operand_value_type(operand_text2) == "literal") {
            operand2 = new int1;
            dynamic_mem.push_back(operand2);
            *(operand2) = string_to_number(operand_text2);
        }
        else if (operand_value_type(operand_text2) == "registor") {
            operand2 = operand_registor_name(operand_text2);
        }
        else {
            operand2 = &MEMORY[string_to_address(operand_text2)];
            flag_is_memory = true;
        }
    }
    else {
        val = false;
        std::cout << "Errooor: The second operand, respectively, can be an l or '[]' from memory or any register or literal!!!" << std::endl;
        return;
    }
    if (check_operand1_valid_or_not(operand_text1)) {
        if (flag_cmp && operand_value_type(operand_text1) == "literal") {
            operand1 = new int1;
            dynamic_mem.push_back(operand1);
            *(operand1) = string_to_number(operand_text1);
        }
        else if (operand_value_type(operand_text1) == "registor") {
            operand1 = operand_registor_name(operand_text1);
        }
        else {
            operand1 = &MEMORY[string_to_address(operand_text1)];
            if (flag_is_memory){
                std::cout << "Error: Cannot have two memory type operands at the same time!!" << std::endl;
                val = false;
                return;
            }
        }
    }
    else {
        val = false;
        std::cout << "Error: The first operand can only be a variable memory area (memory address or register)" << std::endl;
        return;
    }
    val = true;
    flag_is_memory = false;
}

bool ins::getVal() const{
    return val;
}

void ins::run(){
    function(operand1, operand2);
}

std::string ins::getInfo() const {
    return str_info_about_instruction;
}

void ins::setInfo(const std::string& str) {
    str_info_about_instruction = str;
}

//unar_ins_JUMP


unar_ins_JUMP::unar_ins_JUMP (const std::string& operand_text, const std::string& instruction_name)
    :name(instruction_name)
{
    if (instruction_name == "JMP"){
        function = JMP;
    }
    else if (instruction_name == "JG"){
        function = JG;
    }
    else if (instruction_name == "JE"){
        function = JE;
    }
    else if (instruction_name == "JL"){
        function = JL;
    }
    else {
        val = false;
        std::cout << "ERROR!!:You want to import a JUMP, but there is no such command" << std::endl;
        return;
    }
    if (check_met(operand_text)) {
        operand = operand_text;
    }
    else {
        val = false;
        std::cout << "ERROOOR: A little attention you are almost close to the solution but have an error in the deviations" << std::endl;
        return;
    }
    val = true;
}

bool unar_ins_JUMP::getVal() const{
    return val;
}

void unar_ins_JUMP::run(){
    function(operand);
}

std::string unar_ins_JUMP::getInfo() const {
    return str_info_about_instruction;
}

void unar_ins_JUMP::setInfo(const std::string& str) {
    str_info_about_instruction = str;
}

//unar_ins_not 

unar_ins_not::unar_ins_not (const std::string& operand_text) {
    function = NOT;
    if (check_unar_operand_valid_or_not(operand_text)) {
        if (operand_value_type(operand_text) == "registor") {
            operand = operand_registor_name(operand_text);
        }
        else {
            operand = &MEMORY[string_to_address(operand_text)];
            flag_is_memory = true;
        }
    }
    else {
        val = false;
        std::cout << "Erroor!!:Only a mutable memory range literal or register can be negated. No other objects can be negated in this language." << std::endl;
        return;
    }
    val = true;
}

bool unar_ins_not::getVal() const{
    return val;
}

void unar_ins_not::run(){
    function(operand);
}

std::string unar_ins_not::getInfo() const {
    return str_info_about_instruction;
}

void unar_ins_not::setInfo(const std::string& str) {
    str_info_about_instruction = str;
}

//dumpMemory


dumpMemory::dumpMemory ()
    :function(print_dump)
{}

void dumpMemory::run () {
    function();
}

std::string dumpMemory::getInfo() const {
    return str_info_about_instruction;
}

void dumpMemory::setInfo(const std::string& str) {
    str_info_about_instruction = str;
}
