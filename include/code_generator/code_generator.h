#include <parser/parser.h>
#include <file_writer/file_writer.h>
#include <symbol_table/symbol_table.h>


/**
 * @brief This class is responsible for converting AST into Assembly code.
 * We traverse AST to generate code.
 */
class Code_Generator{
    private:
        SymbolTable *ST; //!< Symbol_Table pointer 
        File_Writer fw;  //!< File_Writer Object 

         /**
         * @brief Traversing AST Tree.
         * This is entry point of Traversing Tree.
         * @param TreeNode head, head of AST
         * @return void
         */
        void traverse(TreeNode *head);

         /**
         * @brief Traversig based on type of Grammar.
         * This method calls other traversing methdos based on condition.
         * @param TreeNode node 
         * @return TreeNode 
         */
        TreeNode *sub_traverse(TreeNode *node);


        /**
         * @brief Travering Assign TreeNode
         * @return void
         */
        void traverse_assign(TreeNode *node);

        /**
         * @brief Travering Binary Expression TreeNode  
         * @return TreeNode 
         */
        TreeNode *traverse_binary_expression(TreeNode *node);

        /**
         * @brief Travering If Statement TreeNode  
         * @return void
         */
        void traverse_if_statement(TreeNode *node);

    public:

        /**
         * @brief Constructor for Code_Generator class. 
         * @param pointer to SymbolTable ojbect
         * @return void
         */
        Code_Generator(std::string &output_file_url, SymbolTable *ST);

        /**
         * @brief Generating Code in file. 
         * @param TreeNode, head of AST 
         * @return void
         */
        void generate(TreeNode *node);
};
