#pragma once
#include <unordered_map>
#include <string>

/**
 * @brief Struct for storing Symbol attributes.
 */
typedef struct {
    std::string data_type;
    std::string address;
} Attribute; 

/**
 * @brief Symbol table class.
 * Contains all methods to manage Symbol table.
 */
class SymbolTable{
    private:
        int MAX_MEMORY_LIMIT = 255; //!<< This is the maximum memory size of our 8 bit computer. (after 256 bytes, it roll over to 0)
        std::unordered_map<std::string, Attribute> table;  //!< Unordered Hashmap for storing Symbols and its attributes.
    
    public:
        /**
         * @brief This method is to insert symbol in symbol table.
         * @param string, symbol value
         * @param string, data_type.
         * @return void 
         */
        void insert_symbol(std::string symbol, std::string data_type);

        /**
         * @brief This method is to check if symbol already exist in symbol table or not.
         * @param symbol value
         * @return bool
         */
        bool exist(std::string symbol);
}; 
