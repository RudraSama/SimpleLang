#include <code_generator/code_generator.h>


Code_Generator::Code_Generator(std::string &output_file_url, SymbolTable *ST){
    this->ST = ST;
    fw.open_file(output_file_url);
}

void Code_Generator::generate(TreeNode *head){

    fw.append("start:");
    traverse(head);
    fw.append("\thlt");

    fw.close_file();
}

void Code_Generator::traverse(TreeNode *head){
    if(head == nullptr) return;

    while(head->get_next() != nullptr){
        sub_traverse(head->get_node());
        head = head->get_next();
    }
}

TreeNode *Code_Generator::sub_traverse(TreeNode *node){
    if(Parser::is_assign(node))
        traverse_assign(node);
    else if(Parser::is_if_statement(node)){
        int position = fw.get_position();

        std::string if_label = "if_" + std::to_string(position);
        fw.append("\tcall %" + if_label);
        fw.append("\t");
        fw.append(if_label+":");
        traverse_if_statement(node);

        fw.set_position(position+1);
    }
    else if(Parser::is_binary_expression(node))
        return traverse_binary_expression(node);
    else if(Parser::is_identifier(node))
        return node;
    else if(Parser::is_literal(node))
        return node;

    return nullptr;
}

void Code_Generator::traverse_assign(TreeNode *node){
    Assign *assign = dynamic_cast<Assign*>(node);

    Identifier *ident = dynamic_cast<Identifier*>(assign->get_left());
    unsigned char address = ST->get_address(ident->get_value());

    TreeNode *expr = sub_traverse(assign->get_right());

    if(Parser::is_binary_expression(expr)){
        std::string instruction = "\tmov M A" + std::to_string((int)address);
        fw.append(instruction);
    }
    else if(Parser::is_literal(expr)){
        int value = (int)dynamic_cast<Literal*>(expr)->get_value();

        std::string instruction = "\tldi A " + std::to_string(value);
        fw.append(instruction);

        instruction = "\tmov M A " + std::to_string((int)address);
        fw.append(instruction);
    }
    else if(Parser::is_identifier(expr)){

        Identifier *ident_right = dynamic_cast<Identifier*>(expr);
        unsigned char address_right = ST->get_address(ident_right->get_value());

        std::string instruction = "\tlda " + std::to_string((int)address_right);
        fw.append(instruction);

        instruction = "\tsta " + std::to_string((int)address);
        fw.append(instruction);
    }
}

TreeNode *Code_Generator::traverse_binary_expression(TreeNode *node){
    BinaryExpr *binary_expr = dynamic_cast<BinaryExpr*>(node);
    
    TreeNode *left = sub_traverse(binary_expr->get_left());

    TreeNode *right = sub_traverse(binary_expr->get_right());

    //LEFT SIDE
    if(Parser::is_literal(left)){
        int value = (int)dynamic_cast<Literal*>(left)->get_value();
        std::string instruction = "\tldi A " + std::to_string(value);
        fw.append(instruction);
    }

    else if(Parser::is_identifier(left)){
        Identifier *ident = dynamic_cast<Identifier*>(left);
        unsigned char address = ST->get_address(ident->get_value());
        std::string instruction = "\tmov A M " + std::to_string((int)address);
        fw.append(instruction);
    }

    else if(Parser::is_binary_expression(left)){
        fw.append("\tpop A");
    }


    //RIGHT SIDE
    if(Parser::is_literal(right)){
        int value = (int)dynamic_cast<Literal*>(right)->get_value();
        std::string instruction = "\tldi B " + std::to_string(value);
        fw.append(instruction);
    }

    else if(Parser::is_identifier(right)){
        Identifier *ident = dynamic_cast<Identifier*>(right);
        unsigned char address = ST->get_address(ident->get_value());
        std::string instruction = "\tmov B M " + std::to_string((int)address);
        fw.append(instruction);
    }


    else if(Parser::is_binary_expression(right)){
        fw.append("\tpop B");
    }

    if(binary_expr->get_operator() == Token_Kind::PLUS_TOKEN) fw.append("\tadc");
    else fw.append("\tsub");

    fw.append("\tpush A");

    return binary_expr;
}

void Code_Generator::traverse_if_statement(TreeNode *node){
    IFStatement *if_statement = dynamic_cast<IFStatement*>(node);


    TreeNode *left = sub_traverse(if_statement->get_left());
    TreeNode *right = sub_traverse(if_statement->get_right());

    if(Parser::is_literal(left)){
        int value = dynamic_cast<Literal*>(left)->get_value();

        std::string instruction = "\tldi A " + std::to_string(value);
        fw.append(instruction);
    }
    else if(Parser::is_identifier(left)){
        Identifier *ident = dynamic_cast<Identifier*>(left);
        unsigned char address = ST->get_address(ident->get_value());

        std::string instruction = "\tmov A M " + std::to_string((int)address);
        fw.append(instruction);
    }
    else if(Parser::is_binary_expression(left)){
        fw.append("\tpop A");
    }

    if(Parser::is_literal(right)){
        int value = dynamic_cast<Literal*>(right)->get_value();

        std::string instruction = "\tldi B " + std::to_string(value);
        fw.append(instruction);
    }
    else if(Parser::is_identifier(right)){
        Identifier *ident = dynamic_cast<Identifier*>(right);
        unsigned char address = ST->get_address(ident->get_value());

        std::string instruction = "\tmov B M " + std::to_string((int)address);
        fw.append(instruction);
    }

    else if(Parser::is_binary_expression(right)){
        fw.append("\tpop B");
    }

    int position = fw.get_position();

    std::string if_body = "if_body_" + std::to_string(position);
    fw.append("\tcmp");

    if(if_statement->get_condition() == Token_Kind::EQUALS_TOKEN){
        fw.append("\tjz %" + if_body);
    }
    else if(if_statement->get_condition() == Token_Kind::NOT_EQUALS_TOKEN){
        fw.append("\tjnz %" + if_body);
    }

    fw.append("\tret");
    fw.append("\t");
    fw.append(if_body + ":");

    traverse(if_statement->get_block());

    fw.append("\tret");

    fw.set_position(position+1);

}

