#include "AST.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include <map>

using std::cout;
using std::endl;
using std::map;
using std::setw;
using std::string;
using std::to_string;
using std::vector;

namespace AST
{
int count = 0;
int block = 0;
vector<int> entry;
map<string, int> function_entry;

void PrintInterval(int dep)
{
    for (int i = 0; i < dep; ++i)
    {
        cout << "\t";
    }
}
void IntNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Integer:" << val << endl;
}
void FloatNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Float:" << val << endl;
}
void StringNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "String:" << str << endl;
}
void BinOpNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "BinOp:" << binop << endl;
    l_child->Print(depth + 1);
    r_child->Print(depth + 1);
}
void ExpressionStatementNode::Print(int depth)
{
    if (exprs.empty())
    {
        PrintInterval(depth);
        cout << "Empty stmt" << endl;
    }
    else if (exprs.size() == 1)
    {
        exprs[0]->Print(depth);
    }
    else
    {
        PrintInterval(depth);
        cout << "Mult expr statement:" << endl;
        for (auto &expr : exprs)
        {
            expr->Print(depth + 1);
        }
    }
}
void IdentifierNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "ID:" << name << endl;
}
void ProgramNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Program:" << endl;
    for (auto &decl_ptr : decls)
    {
        decl_ptr->Print(depth + 1);
    }
}

void FunCallNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Function call:" << name << ":" << endl;
    PrintInterval(depth);
    cout << "Params" << endl;
    for (auto &param : params)
    {
        param->Print(depth + 1);
    }
}
void ArrayOffsetNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "ArrayIndexExpr:" << endl;
    PrintInterval(depth);
    cout << "ArrayExpr:" << endl;
    array->Print(depth + 1);
    PrintInterval(depth);
    cout << "IndexExpr:" << endl;
    offset->Print(depth + 1);
}
void UnaryOpNode::Print(int depth)
{
    PrintInterval(depth);
    if (isPrefix)
    {
        cout << "Prefix UnaryOp" << unaryop << endl;
    }
    else
    {
        cout << "Postfix UnaryOp" << unaryop << endl;
    }
    child->Print(depth + 1);
}

void InitListNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Init list:" << endl;
    for (auto &expr : initList)
    {
        expr->Print(depth + 1);
    }
}

void StatementNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Empty Statement." << endl;
}

void ReturnNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Return stmt:" << endl;
    expr->Print(depth + 1);
}
void FuncParamDeclNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "FuncParamDecl:" << endl;
    PrintInterval(depth);
    cout << "Type:" << endl;
    type->Print(depth + 1);
    if (id != NULL)
    {
        PrintInterval(depth);
        cout << "ID:" << endl;
        type->Print(depth + 1);
    }
    if (initializer)
    {
        PrintInterval(depth);
        cout << "Initializer:" << endl;
        initializer->Print(depth + 1);
    }
}
void FuncDeclNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "FuncDecl:" << name << ":" << endl;
    PrintInterval(depth);
    cout << "ReturnType:" << endl;
    returnType->Print(depth + 1);
    PrintInterval(depth);
    cout << "ParamDecls" << endl;
    if (funcParamDecls.empty())
    {
        PrintInterval(depth + 1);
        cout << "void" << endl;
    }
    else
    {
        for (auto &decl : funcParamDecls)
        {
            decl->Print(depth + 1);
        }
    }
    stmt->Print(depth + 1);
}
void BlockNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Statement block:" << endl;
    for (auto &stmt : stmtList)
    {
        stmt->Print(depth + 1);
    }
}
void IfNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "If statement:" << endl;
    PrintInterval(depth);
    cout << "condition:" << endl;
    for (auto &cond : conditions)
    {
        cond->Print(depth + 1);
    }
    PrintInterval(depth);
    cout << "stmt:" << endl;
    stmt->Print(depth + 1);
    if (else_stmt != NULL)
    {
        PrintInterval(depth);
        cout << "else stmt:" << endl;
        else_stmt->Print(depth + 1);
    }
}

void VarDeclNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "Var Decl:" << endl;
    PrintInterval(depth);
    cout << "Type:" << endl;
    type->Print(depth + 1);
    PrintInterval(depth);
    cout << "DeclList:" << endl;
    for (auto &decl : declList)
    {
        decl->Print(depth + 1);
    }
}

void WhileNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "While statement:" << endl;
    PrintInterval(depth);
    cout << "condition:" << endl;
    for (auto &condition : conditions)
    {
        condition->Print(depth + 1);
    }
    PrintInterval(depth);
    cout << "stmt:" << endl;
    stmt->Print(depth + 1);
}

void ForNode::Print(int depth)
{
    PrintInterval(depth);
    cout << "For statement:" << endl;
    PrintInterval(depth);
    cout << "init stmt:" << endl;
    init_stmt->Print(depth + 1);
    PrintInterval(depth);
    cout << "cond stmt:" << endl;
    cond_stmt->Print(depth + 1);
    if (!range_exprs.empty())
    {
        PrintInterval(depth);
        cout << "range stmt:" << endl;
        for (auto &range_expr : range_exprs)
        {
            range_expr->Print(depth + 1);
        }
    }
    PrintInterval(depth);
    cout << "stmt:" << endl;
    stmt->Print(depth + 1);
}

NumNode *NumNode::GetNumNode(std::string strval)
{
    if (strval.find('.') == string::npos)
    {
        return new IntNode(std::stoi(strval));
    }
    else
    {
        return new FloatNode(std::stof(strval));
    }
}

void PrintTab()
{
    PrintInterval(1);
}

string ExpressionNode::codeGen()
{
    cout << "S" << ++count << endl;
    return "";
}

string IntNode::codeGen()
{
    return to_string(val);
}

string FloatNode::codeGen()
{
    return to_string(val);
}

string StringNode::codeGen()
{
    return str;
}

string BinOpNode::codeGen()
{
    string param1 = l_child->codeGen();
    string param2 = r_child->codeGen();
    if (binop == "=")
    {
        cout << setw(10) << " " << setw(10) << binop << setw(10) << param2 << setw(10) << " " << setw(10) << param1 << endl;
        return param1;
    }
    else if (binop == "-" || binop == "*" || binop == "+" || binop == "/")
    {
        string param3 = "t" + to_string(++count);
        cout << setw(10) << " " << setw(10) << binop << setw(10) << param1 << setw(10) << param2 << setw(10) << param3 << endl;
        return param3;
    }
    else if (binop == ">" || binop == "<" || binop == ">=" || binop == "<=" || binop == "!=" || binop == "==")
    {
        string op = "if" + binop;
        cout << setw(10) << " " << setw(10) << op << setw(10) << param1 << setw(10) << param2 << setw(10) << "goto";
        return "OP";
    }
    return "BinOpNode";
}

string UnaryOpNode::codeGen()
{
    string param1 = child->codeGen();
    string param2 = "t" + to_string(++count);
    cout << setw(10) << " " << setw(10) << unaryop << setw(10) << param1 << setw(10) << " " << setw(10) << param2 << endl;
    return param2;
}

string IdentifierNode::codeGen()
{
    return name;
}

string FunCallNode::codeGen()
{
    vector<string> params_name;
    for (auto &i : params)
    {
        params_name.push_back(i->codeGen());
    }
    for (auto &i : params_name)
    {
        cout << setw(10) << " " << setw(10) << "param" << setw(10) << i << endl;
    }
    cout << setw(10) << " " << setw(10) << "call" << setw(10) << name << endl;
    return name;
}

string ArrayOffsetNode::codeGen()
{
    string param = "t" + to_string(++count);
    string name = array->codeGen();
    string offset_num = offset->codeGen();
    cout << setw(10) << " " << setw(10) << "[]" << setw(10) << name << setw(10) << offset_num << setw(10) << param << endl;
    return param;
}

string InitListNode::codeGen()
{
    for (auto &i : initList)
    {
        i->codeGen();
    }
    return "InitListNode";
}

string StatementNode::codeGen()
{
    string stmt = "S" + to_string(++count) + ": ";
    cout << std::setw(5) << stmt << endl;
    return "StatementNode";
}

string ExpressionStatementNode::codeGen()
{
    for (auto &i : exprs)
    {
        i->codeGen();
    }
    return "ExpressionStatementNode";
}

string ReturnNode::codeGen()
{
    string param = expr->codeGen();
    cout << setw(10) << " " << setw(10) << "ret" << setw(10) << param << endl;
    return param;
}

string FuncParamDeclNode::codeGen()
{
    string param = initializer->codeGen();
    string name = id->codeGen();
    cout << setw(5) << "=" << setw(5) << param << setw(5) << " " << setw(5) << name << endl;
    if (type->name == "double")
    {
        //cout << setw(5) << "malloc" << setw(5) << "64" << setw(5) << " " << setw(5) << name << endl;
    }
    else
    {
        //cout << setw(5) << "malloc" << setw(5) << "32" << setw(5) << " " << setw(5) << name << endl;
    }
    return name;
}

string FuncDeclNode::codeGen()
{
    vector<string> nameList;
    for (auto &i : funcParamDecls)
    {
        nameList.push_back(i->codeGen());
    }
    string retName = returnType->name;
    cout << setw(10) << ++block << endl
         << setw(10) << " " << setw(10) << retName << setw(10) << name << endl;
    function_entry["name"] = block;
    string result = stmt->codeGen();
    return result;
}

string VarDeclNode::codeGen()
{
    vector<string> nameList;
    for (auto &i : declList)
    {
        nameList.push_back(i->codeGen());
    }
    int width;
    if (type->name == "double")
    {
        width = 64;
    }
    else
    {
        width = 32;
    }
    for (auto &i : nameList)
    {
        //cout << setw(10) << " " << setw(10) << "malloc" << setw(10) << width << setw(10) << " " << setw(10) << i << endl;
    }
    return "VarDeclNode";
}

string BlockNode::codeGen()
{
    for (auto &i : stmtList)
    {
        i->codeGen();
    }
    return "BlockNode";
}

string IfNode::codeGen()
{
    int cond_num = ++block;
    int stmt_num = ++block;
    int else_num = ++block;
    int final_num = ++block;
    cout << setw(10) << cond_num << endl;
    for (auto &i : conditions)
    {
        i->codeGen();
        cout << setw(10) << stmt_num << endl;
    }
    if (else_stmt == NULL)
    {
        cout << setw(10) << " " << setw(10) << "goto" << setw(10) << final_num << endl;
    }
    else
    {
        cout << setw(10) << " " << setw(10) << "goto" << setw(10) << else_num << endl;
    }

    cout << setw(10) << stmt_num << endl;
    stmt->codeGen();

    if (else_stmt == NULL)
    {
        cout << setw(10) << final_num << endl;
    }
    else
    {
        cout << setw(10) << else_num << endl;
        else_stmt->codeGen();
        cout << setw(10) << final_num << endl;
    }
    return "IfNode";
}

string WhileNode::codeGen()
{
    string param;
    int cond_num = ++block;
    int stmt_num = ++block;
    int final_num = ++block;
    cout << setw(10) << cond_num << endl;
    for (auto &i : conditions)
    {
        param = i->codeGen();
        cout << setw(10) << stmt_num << endl;
    }
    cout << setw(10) << " " << setw(10) << "goto" << setw(10) << final_num << endl;
    cout << setw(10) << stmt_num << endl;
    stmt->codeGen();
    cout << setw(10) << " " << setw(10) << "goto" << setw(10) << cond_num << endl;
    cout << setw(10) << final_num << endl;
    return "WhileNode";
}

string ForNode::codeGen()
{
    int cond_num = ++block;
    int stmt_num = ++block;
    int final_num = ++block;
    string param1 = init_stmt->codeGen();
    cout << setw(10) << cond_num << endl;

    string param2 = cond_stmt->codeGen();
    cout << setw(10) << stmt_num << endl;
    cout << setw(10) << " " << setw(10) << "goto" << setw(10) << final_num << endl;

    cout << setw(10) << stmt_num << endl;
    stmt->codeGen();
    for (auto &i : range_exprs)
    {
        i->codeGen();
    }
    cout << setw(10) << " " << setw(10) << "goto" << setw(10) << cond_num << endl;

    cout << setw(10) << final_num << endl;
    return "ForNode";
}

string ProgramNode::codeGen()
{
    cout << "code generation begin" << endl;
    for (auto &decl_ptr : decls)
    {
        decl_ptr->codeGen();
    }
    cout << "code generation end" << endl;
    return "ProgramNode";
}
} // namespace AST
