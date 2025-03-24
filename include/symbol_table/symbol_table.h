#pragma once
#include <unordered_map>
#include <string>

/**
 * @brief Struct for storing Symbol attributes.
 */
typedef struct {
    std::string data_type;
    unsigned char address; //!< Address will never go above 255
} Attribute; 

/**
 * @brief Symbol table class.
 * Contains all methods to manage Symbol table.
 */
class SymbolTable{
    private:
        unsigned char MAX_MEMORY_LIMIT = 128; //!<< This is the maximum memory size of our 8 bit computer. (Lower 128 bytes are allocated for Variable memory and upper 127 bytes are for Stack memory)
        std::unordered_map<std::string, Attribute> table;  //!< Unordered Hashmap for storing Symbols and its attributes.
    
    public:
        /**
         * @brief This method is to insert symbol in symbol table.
         * @param string, symbol value
         * @param string, data_type.
         * @return bool 
         */
        bool insert_symbol(std::string symbol, std::string data_type);

        /**
         * @brief This method is to check if symbol already exist in symbol table or not.
         * @param symbol value
         * @return bool
         */
        bool exist(std::string symbol);

        /**
         * @brief Returns address of variable.
         * @param symbol value
         * @return unsigened char 
         */
        unsigned char get_address(std::string symbol);
}; 
