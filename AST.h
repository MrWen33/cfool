#ifndef _AST_H_
#define _AST_H_
#include <string>
#include <vector>
#include <memory>
namespace AST
{
class ExpressionNode;
class StatementNode;
class ExpressionNode;
class IdentifierNode;
class ExpressionStatementNode;
class FuncParamDeclNode;
class DeclNode;

//typedef ExpressionNode> ExpressionNodePtr;
//typedef StatementNode> StatementNodePtr;
//typedef DeclNode> DeclNodePtr;
//typedef IdentifierNode> IDPtr;
//typedef ExpressionStatementNode> ExprStmtPtr;
//typedef FuncParamDeclNode> FuncParamDeclPtr;

typedef ExpressionNode *ExpressionNodePtr;
typedef StatementNode *StatementNodePtr;
typedef DeclNode *DeclNodePtr;
typedef IdentifierNode *IDPtr;
typedef ExpressionStatementNode *ExprStmtPtr;
typedef FuncParamDeclNode *FuncParamDeclPtr;

typedef std::vector<FuncParamDeclPtr> FuncParamDeclList;
typedef std::vector<StatementNodePtr> StatementList;
typedef std::vector<ExpressionNodePtr> ExprList;
typedef std::vector<DeclNodePtr> DeclList;

class Node
{
  public:
	virtual ~Node() {}
	virtual void Print(int depth) = 0;
	virtual std::string codeGen() = 0;
};
class ExpressionNode : public Node
{
  public:
	std::string codeGen() override;
};
class NumNode : public ExpressionNode
{
  public:
	static NumNode *GetNumNode(std::string strval);
};
class IntNode : public NumNode
{
  public:
	long long val;
	IntNode(long long IntVal) : val(IntVal) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class FloatNode : public NumNode
{
  public:
	float val;
	FloatNode(float FloatVal) : val(FloatVal) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class StringNode : public ExpressionNode
{
  public:
	std::string str;
	StringNode(std::string StrVal) : str(StrVal) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class BinOpNode : public ExpressionNode
{
  public:
	std::string binop;
	ExpressionNodePtr l_child;
	ExpressionNodePtr r_child;
	BinOpNode(std::string op, ExpressionNodePtr LChild, ExpressionNodePtr RChild)
		: binop(op), l_child(LChild), r_child(RChild) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class UnaryOpNode : public ExpressionNode
{
  public:
	std::string unaryop;
	ExpressionNodePtr child;
	bool isPrefix;
	UnaryOpNode(std::string op, ExpressionNodePtr child, bool isPrefix)
		: unaryop(op), child(child), isPrefix(isPrefix) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class IdentifierNode : public ExpressionNode
{
  public:
	std::string name;
	IdentifierNode(std::string name) : name(name) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class FunCallNode : public ExpressionNode
{
  public:
	std::string name;
	ExprList params;
	FunCallNode(std::string name, ExprList params) : name(name), params(params) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class ArrayOffsetNode : public ExpressionNode
{
  public:
	ExpressionNodePtr array;
	ExpressionNodePtr offset;
	ArrayOffsetNode(ExpressionNodePtr ArrayNode, ExpressionNodePtr Offset) : array(ArrayNode), offset(Offset) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class InitListNode : public ExpressionNode
{
  public:
	ExprList initList;
	InitListNode(ExprList InitList) : initList(InitList) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class StatementNode : public Node
{
  public:
	void Print(int depth) override;
	std::string codeGen() override;
};
class ExpressionStatementNode : public StatementNode
{
  public:
	ExprList exprs;
	ExpressionStatementNode(ExpressionNodePtr Expr)
	{
		if (Expr != NULL)
		{
			exprs.push_back(Expr);
		}
	}
	ExpressionStatementNode(ExprList exprs) : exprs(exprs) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class DeclNode : public StatementNode
{
};

class ReturnNode : public StatementNode
{
  public:
	ExpressionNodePtr expr;
	ReturnNode(ExpressionNodePtr Expr) : expr(Expr) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class FuncParamDeclNode : public ExpressionNode
{
  public:
	IDPtr type;
	IDPtr id;
	ExpressionNodePtr initializer;
	FuncParamDeclNode(IDPtr Type, IDPtr ID, ExpressionNodePtr Init)
		: type(Type), id(ID), initializer(Init) {}
	void Print(int depth) override;
	std::string codeGen() override;
};

class FuncDeclNode : public DeclNode
{
  public:
	IDPtr returnType;
	std::string name;
	FuncParamDeclList funcParamDecls;
	StatementNodePtr stmt;
	FuncDeclNode(IDPtr ReturnType, std::string name, FuncParamDeclList FuncParamDecls, StatementNodePtr Stmt)
		: returnType(ReturnType), name(name), funcParamDecls(FuncParamDecls), stmt(Stmt) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class VarDeclNode : public DeclNode
{
  public:
	IDPtr type;
	ExprList declList;
	VarDeclNode(IDPtr Type, ExprList DeclList) : type(Type), declList(DeclList) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class BlockNode : public StatementNode
{
  public:
	StatementList stmtList;
	BlockNode(StatementList StmtList) : stmtList(StmtList) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class IfNode : public StatementNode
{
  public:
	ExprList conditions;
	StatementNodePtr stmt;
	StatementNodePtr else_stmt;
	IfNode(ExprList Condition, StatementNodePtr Stmt, StatementNodePtr ElseStmt = NULL)
		: conditions(Condition), stmt(Stmt), else_stmt(ElseStmt) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class WhileNode : public StatementNode
{
  public:
	ExprList conditions;
	StatementNodePtr stmt;
	WhileNode(ExprList Condition, StatementNodePtr Stmt) : conditions(Condition), stmt(Stmt) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class ForNode : public StatementNode
{
  public:
	ExprStmtPtr init_stmt;
	ExprStmtPtr cond_stmt;
	ExprList range_exprs;
	StatementNodePtr stmt;
	ForNode(ExprStmtPtr InitStmt, ExprStmtPtr CondStmt, ExprList RangeStmt, StatementNodePtr Stmt)
		: init_stmt(InitStmt), cond_stmt(CondStmt), range_exprs(RangeStmt), stmt(Stmt) {}
	void Print(int depth) override;
	std::string codeGen() override;
};
class ProgramNode : public Node
{
  public:
	DeclList decls;
	ProgramNode() {}
	void Print(int depth) override;
	std::string codeGen() override;
};
} // namespace AST

#endif
