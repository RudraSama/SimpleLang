#include <lexer/lexer.h>
#include <iostream>

//Grammar for our langauge 

//Base Class
class TreeNode{
    //This is for printing tree
    public:
        virtual void print(){}
};

//Grammer for Identifier
class Identifier : public TreeNode{
    private:
        std::string value;
    public:
        Identifier(std::string value){
            this->value = value;
        }

        void print(){
            std::cout<<" ";
            std::cout<<value;
            std::cout<<" ";
        }
};

//Grammar for ASSIGN Statement
class Assign : public TreeNode{
    private:
        std::string value;
        TreeNode *left;
        TreeNode *right;
    public:
        Assign(std::string value, TreeNode *left, TreeNode *right){
            this->value = value;
            this->left = left;
            this->right = right;
        }

        void print(){
            left->print();
            std::cout<<" ";
            std::cout<<value;
            std::cout<<" ";
            right->print();
        }
};

//Grammar for NUMBERS
class Literal : public TreeNode{
    private:
        int value;
    public:
        Literal(int value){
            this->value = value;
        }

        int get_value(){
            return value;
        }

        void print(){
            std::cout<<value;
        }
};

//Binary Expression such as 'a + b', 'a - b'
class BinaryExpr : public TreeNode{
    private:
        std::string value; //value is an operator type (-, +)
        TreeNode *left;        //Left side of expression
        TreeNode *right;       //Right side of expression
    public:
        BinaryExpr(TreeNode* left, std::string value, TreeNode* right){
            this->left = left;
            this->value = value;
            this->right = right;
        }


        void print(){
            std::cout<<"( ";
            left->print();
            std::cout<<" ";
            std::cout<<value;
            std::cout<<" ";
            right->print();
            std::cout<<" )";
        }
};

class parser{
    private:
        std::vector<Token> token_vector;
        int index = 0;              //This is token index in vector
        Token peek();               //returns current Token in list
        void next_token();          //forwards token positin to next
        bool expect(Token_Kind token_kind);   //Return true if current token is expected token else false
        bool is_literal(TreeNode *node);
        int literal_evaluation(TreeNode *left, TreeNode *right, Token_Kind token_kind);
        TreeNode *parse_statement();    //This will parse statements like 'int a = 5 + 6'
        TreeNode *program();
        TreeNode *parse_expression(bool group_expr = false);   //This will parse expressoin 
        TreeNode *parse_term();         //This will parse Term (Literal, Identifier, OpenPara)

    public:
        parser(std::vector<Token> token_vector);
       //TODO This will return TreeNode 
        void parse();
        //might delete this method
        void print_tokens();
};
