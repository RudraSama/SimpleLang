#include <symbol_table/symbol_table.h>
#include <iostream>

void SymbolTable::insert_symbol(std::string symbol, std::string data_type){
    //TODO creating address for every new variable.
    //Also checking if address exceeds LIMIT or not.
    Attribute atr;
    atr.address = "20";
    atr.data_type = data_type;
    table.insert({symbol, atr});
}

bool SymbolTable::exist(std::string symbol){
    if(table.find(symbol) != table.end()){
        return true;
    }

    return false;
}
