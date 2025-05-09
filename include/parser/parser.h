#pragma once
#include <iostream>
#include <file_writer/file_writer.h>
#include <lexer/lexer.h>
#include <symbol_table/symbol_table.h>

/**
 * Grammar for our langauge 
 */ 

/** 
 * @brief Base class for all grammar.
 */
class TreeNode{
    public:
        /** 
         * @brief prints tree.
         * @return void
        */
        virtual ~TreeNode(){}

        /** 
         * @brief prints tree.
         * @return void
        */
        virtual void print(){}


        /** Only Statements Class will Implement these */

        /** 
         * @brief Add statement to Current Node of Statements.
         * @param Node of statement. Any instance of TreeNode.
         * @return void
        */
        virtual void add_node(TreeNode *node){ (void)node; /*To prevent compiler from giving warning*/}

        /** 
         * @brief Add Next Node to current node of Statements.
         * @param Nullptr of TreeNode.
         * @return void
        */
        virtual void add_next(TreeNode *node){ (void)node; }

        /** 
         * @brief Get Next Node of Statements.
         * @return void
        */
        virtual TreeNode *get_next(){ return nullptr; }

        /** 
         * @brief Get Current Node of Statements.
         * @return void
        */
        virtual TreeNode *get_node(){ return nullptr; }
};


/**
 * @brief Grammar rule for Identifier.
 */
class Identifier : public TreeNode{
    private:
        std::string value; //!< value of the identifier, i.e. name of variable
    public:

        /**
         * @brief Constructor for Identifier.
         * @param value. Name of the identifier.
        */
        Identifier(std::string value){
            this->value = value;
        }

        /**
         * @brief Returning value
         * @return string
         */
        std::string get_value(){
            return value;
        }

        void print(){
            std::cout<<" ";
            std::cout<<value;
            std::cout<<" ";
        }
};

/**
 * @brief Grammar rule for Identifier.
 */
class Assign : public TreeNode{
    private:
        std::string value;  //!< '=' 
        TreeNode *left;     //!< Left side of the assignment statement
        TreeNode *right;    //!< Right side of the assignment statement
    public:
        /**
         * @brief Constructor of Assign
         * @param left, instance of TreeNode (Identifier).
         * @param value, '='.
         * @param right, instace of TreeNode (Identifer, Literal, BinaryExpres).
         */
        Assign(TreeNode *left, std::string value, TreeNode *right){
            this->left = left;
            this->value = value;
            this->right = right;
        }

        ~Assign(){
            delete left;
            delete right;
        }

        /**
         * @brief Returning value
         * @return string
         */
        std::string get_value(){
            return value;
        }

        /**
         * @brief Return left side of Assign
         * @return TreeNode 
         */
        TreeNode *get_left(){
            return left;
        }

        /**
         * @brief Return right side of Assign 
         * @return string
         */
        TreeNode *get_right(){
            return right;
        }

        void print(){
            left->print();
            std::cout<<" ";
            std::cout<<value;
            std::cout<<" ";
            right->print();
        }
};

/**
 * @brief Grammar rule for Literal.
 */
class Literal : public TreeNode{
    private:
        unsigned char value; //!< Storing value from 1 to 255 only because of our CPU's memory constraint.
    public:

        /**
         * @brief Constructor of Assign
         * @param value, Any valid int from 1 to 255.
         */
        Literal(unsigned char value){
            this->value = value;
        }

        /**
         * @brief returns current value of Literal.  
         * @param unsigned char.
         */
        unsigned char get_value(){
            return value;
        }

        void print(){
            std::cout<<(int)value;
        }
};

/**
 * @brief Grammar rule for Binary Expression.
 */
class BinaryExpr : public TreeNode{
    private:
        Token_Kind op;    //!<  op is an operator type (-, +) 
        TreeNode *left;       //!<  Left side of expression 
        TreeNode *right;      //!<  Right side of expression 

    public:
        /**
         * @brief Constructor of BinaryExpr 
         * @param left, instance of TreeNode (BinaryExpr, Identifier, Literal).
         * @param value. (+, -).
         * @param right, instace of TreeNode (BinaryExpr, Identifier, Literal).
         */
        BinaryExpr(TreeNode *left, Token_Kind op, TreeNode *right){
            this->left = left;
            this->op = op;
            this->right = right;
        }

        ~BinaryExpr(){
            delete left;
            delete right;
        }

        /**
         * @brief Returning value
         * @return string
         */
        Token_Kind get_operator(){
            return op;
        }

        /**
         * @brief Return Left side of Binary Expresion
         * @return TreeNode
         */
        TreeNode *get_left(){
            return left;
        }

        /**
         * @brief Return Right side of Binary Expresion
         * @return TreeNode
         */
        TreeNode *get_right(){
            return right;
        }

        void print(){
            std::cout<<"( ";
            left->print();
            std::cout<<" ";
            std::cout<<Lexer::token_to_string(op).value;
            std::cout<<" ";
            right->print();
            std::cout<<" )";
        }
};

/**
 * @brief Grammar rule for Statements.
 */
class Statements : public TreeNode{
    private:
        TreeNode *node;     //!< Current Statement 
        TreeNode *next;     //!< Pointer to Next Node of Statement 

    public:

        ~Statements(){
            delete node;
            delete next;
        }
        /** 
         * @brief Add statement to Current Node of Statements.
         * @param Node of statement. Any instance of TreeNode.
         * @return void
        */
        void add_node(TreeNode *node){
            this->node = node;
        }

        /** 
         * @brief Add Next Node to current node of Statements.
         * @param Nullptr of TreeNode.
         * @return void
        */
        void add_next(TreeNode *node){
            this->next = node;
        }

        /** 
         * @brief Get Next Node of Statements.
         * @return void
        */
        TreeNode *get_next(){
            return next;
        }

        /** 
         * @brief Get Current Node of Statements.
         * @return void
        */
        TreeNode *get_node(){
            return node;
        }
};

/**
 * @brief Grammar rule for IF Statement.
 */
class IFStatement : public TreeNode{
    private:
        Token_Kind condition;      //!< value. (<, >, ==, <=, >=, !=) 
        TreeNode *left;         //!< Left side of Condition 
        TreeNode *right;        //!< Right side of Condition  
        TreeNode *block;        //!< Block of Statements inside If (condition) {}
    public:
        /**
         * @brief Constructor of IFStatement. 
         * @param left, instance of TreeNode (BinaryExpr, Identifier, Literal).
         * @param value. (<, >, ==, <=, >=, !=).
         * @param right, instace of TreeNode (BinaryExpr, Identifier, Literal).
         * @param block, Statements.
         */
        IFStatement(TreeNode *left, Token_Kind condition, TreeNode *right, TreeNode *block){
            this->left = left;
            this->condition = condition;
            this->right = right;
            this->block = block;
        }

        ~IFStatement(){
            delete left;
            delete right;
            delete block;
        }

        /**
         * @brief Returning value
         * @return string
         */
        Token_Kind get_condition(){
            return condition;
        }

        /**
         * @brief Returning left side of condition
         * @return string
         */
        TreeNode *get_left(){
            return left;
        }

        /**
         * @brief Returning right side of condition 
         * @return string
         */
        TreeNode *get_right(){
            return right;
        }

        /**
         * @brief Returning block of IF statement 
         * @return string
         */
        TreeNode *get_block(){
            return block;
        }

        void print(){
            left->print();
            std::cout<<" ";
            std::cout<<Lexer::token_to_string(condition).value;
            std::cout<<" ";
            right->print();
            std::cout<<" { ";
            std::cout<<std::endl; 
            TreeNode *temp = this->block;
            while(temp->get_node() != nullptr){
                std::cout<<"\t";
                temp->get_node()->print();
                std::cout<<std::endl;
                temp = temp->get_next();               
            }
            std::cout<<std::endl; 
            std::cout<<" } ";
        }
};

/**
 * @class parser
 * @brief Simple Recursive Descent Parser
 *
 * This class contains all the methods we need to parse our tokens and build AST Tree 
 */
class Parser{
    private:
        std::vector<Token> token_vector;        //!< Vector for storing all tokens generated using Lexer 
        std::string file_name;                  //!< Source code file name 
        SymbolTable *ST;                         //!< Symbol_Table object pointer


        int index = 0;                          //!< Stores current token index in Vector 
        /**
         * @brief Peek method to get current token in vector.
         * @return Token Struct
         */
        Token peek();               

        /**
         * @brief consume the token and go to next token in vector.
         * @return void 
         */
        void next_token();          

        /**
         * @brief Matches the token we are expecting with the current token.
         * @param Field from Token_Kind enum.
         * @return bool 
         */
        bool expect(Token_Kind token_kind);

        /**
         * @brief Reporting any syntax error when unexpected token is scanned parsing of tokens.
         * Shows error with std::cerr
         * @param Token_Kind expected token,
         * @param Token_Kind token got
         * @return void 
         */
        void report_unexpected_syntax_error(Token_Kind expected, Token_Kind got);
        /**
         * @brief Reporting error when undefined varaible is used.
         * Shows error with std::cerr
         * @param string value of identifier (variable).
         * @return void 
         */
        void report_undefined_variable_error(std::string s);

        /**
         * @brief Reporting error when variable is already defined.
         * Shows error with std::cerr
         * @param string value of identifier (variable).
         * @return void 
         */
        void report_already_defined_variable_error(std::string s);

        /**
         * @brief Reporting error when too many variables are declared, causing memory limit exceed on 8 bit machine.
         * Shows error with std::cerr
         * @param string value of identifer (Variable).
         * @return void 
         */
        void report_memory_limit_exceed_error(std::string s);


        /**
         * @brief Evalute the expression when both sides of expression are of Literal type.
         * This method simplifies the expression while parsing. 
         * @param left TreeNode  
         * @param right TreeNode
         * @param Field of Token_Kind enum. Possible values:
         *  - Token_Kind::PLUS_TOKEN
         *  - Token_Kind::MINUS_TOKEN
         * @return int. Evaluated field.
         */
        int literal_evaluation(TreeNode *left, TreeNode *right, Token_Kind token_kind);

        /**
         * @brief Starts parsing tokens.
         * @return Statements, derived from TreeNode 
         */
        TreeNode *program();

         /**
         * @brief Prase If Statements.
         * @return IFStatement , derived from TreeNode.
         */       
        TreeNode *parse_ifstatement();

         /**
         * @brief Call parse_statement() and combine all parsed statement into single linked list like structure.
         * @return Statements, derived from TreeNode.
         */
        TreeNode *parse_statements();

         /**
         * @brief Parse statements.
         *  - int a = 5 + 6;
         *  - a = a + 9;
         * Right now it can only parse assignment statements.
         * @return Assign, derived from TreeNode.
         */
        TreeNode *parse_statement();    //This will parse statements like 'int a = 5 + 6'

         /**
         * @brief Parse Expressions like 'a + b - c, (a + b) - c'.
         * @return BinaryExpression, derived from TreeNode.
         */
        TreeNode *parse_expression();   //This will parse expressoin 
                                        //
         /**
         * @brief Parse Single Term like an Identifier, Number or Open parentheses (for grouping expression).
         * @return Literal - If term is a number.
         * Identifier - If term is a variable.
         * BinaryExpre - If term is Open parentheses.
         */
        TreeNode *parse_term();         //This will parse Term (Literal, Identifier, OpenPara)

    public:
        /**
         * @brief Constructor for Parse class
         * @param token_vector. Output of tokenizer method of Lexer.
         * @param string file_name. Source code file name.
         * @param pointer to SymbolTable object
         * @return void
         */
        Parser(std::vector<Token> token_vector, std::string file_name, SymbolTable *ST);

        /**
         * @brief Checks if the given Object is of type Literal or not.
         * @param TreeNode 
         * @return bool 
         */
        static bool is_literal(TreeNode *node);

        /**
         * @brief Checks if the given Object is of type IFStatement or not.
         * @param TreeNode 
         * @return bool 
         */
        static bool is_if_statement(TreeNode *node);

        /**
         * @brief Checks if the given Object is of type Assign or not.
         * @param TreeNode 
         * @return bool 
         */
        static bool is_assign(TreeNode *node);

        /**
         * @brief Checks if the given Object is of type Identifier or not.
         * @param TreeNode 
         * @return bool 
         */
        static bool is_identifier(TreeNode *node);

        /**
         * @brief Checks if the given Object is of type BinaryExpr or not.
         * @param TreeNode 
         * @return bool 
         */
        static bool is_binary_expression(TreeNode *node);

         /**
         * @brief Starts parsing tokens.
         * @return void
         */       
        TreeNode *parse();
};
