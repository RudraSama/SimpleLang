#include <symbol_table/symbol_table.h>

bool SymbolTable::insert_symbol(std::string symbol, std::string data_type){
    //TODO creating address for every new variable.
    //Also checking if address exceeds LIMIT or not.
    int new_addr = (int)table.size();

    if((int)new_addr > (int)MAX_MEMORY_LIMIT) return false;

    Attribute atr;
    atr.address = new_addr;
    atr.data_type = data_type;
    table.insert({symbol, atr});

    return true;
}

bool SymbolTable::exist(std::string symbol){
    if(table.find(symbol) != table.end()){
        return true;
    }

    return false;
}

unsigned char SymbolTable::get_address(std::string symbol){
    if(exist(symbol)) return table[symbol].address;
    //Cannot return 0, cause it is valid address
    return 0;
}

