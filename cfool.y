%{
    #include<stdio.h>
    #include<string.h>
    #include "lex.yy.c"
    #include "AST.h"
    #include <memory>
    #include "symbolTab.h"
    //#define PTR(OBJ) std::shared_ptr<OBJ>
    void yyerror(char* s);
    int yyparse(void);
    extern FILE * yyin;
    extern char* yytext;
    //int code = 0;
    struct SymbolTab* symtab;
    void print_info(int code, char* type_str,char* sym_info, int* children_info);
    using namespace AST;
%}
%token TYPE ID LP RP LB RB EQ NE COMMA LSB RSB
%token OP FUNC WHILE FOR IF ELSE INTEGER SEMI RETURN
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST STRING VOID
%token NUM ADDOP MULTOP GREATER LESSER GREQ LEEQ AND OR
%token ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token INC_OP DEC_OP
%union
{
    BinOpNode* binop;
    DeclNode* decl;
    ProgramNode* program;
    IdentifierNode* id;
    ExpressionNode* expr;
    StatementNode* stmt;
    ExpressionStatementNode* exprStmt;
    FuncParamDeclNode* funcParamDecl;
    
    ExprList* exprListPtr;
    StatementList* stmtListPtr;
    FuncParamDeclList* funcParamDeclListPtr;
    
    char attr[16];
};
%start finish
%%
finish: program {
        printf("analyze fin\n");
        $<program>1->Print(0);
        $<program>1->codeGen();
    };

program:program global_decl{
            $<program>$ = $<program>1;
            $<program>$->decls.push_back($<decl>2);
        }
        | global_decl{
            $<program>$ = new ProgramNode();
            $<program>$->decls.push_back($<decl>1);
        }
        ;
global_decl: var_decl{
                $<decl>$ = $<decl>1;
            }
            | func_decl {
                $<decl>$ = $<decl>1;
            }
            ;

assign_op:  ASSIGN {
                //$<code>$=code++;
                //print_info($<code>$, "ASSIGN", "=", NULL);
                strcpy($<attr>$, "=");
            }
            | ADD_ASSIGN{
                strcpy($<attr>$, "+=");
                //$<code>$=code++;
                //print_info($<code>$, "ADD_ASSIGN", "+=", NULL);
            }
            | SUB_ASSIGN{
                strcpy($<attr>$, "-=");
                //$<code>$=code++;
                //print_info($<code>$, "SUB_ASSIGN", "-=", NULL);
            }
            | DIV_ASSIGN{
                strcpy($<attr>$, "/=");
                //$<code>$=code++;
                //print_info($<code>$, "DIV_ASSIGN", "/=", NULL);
            }
            | MUL_ASSIGN{
                strcpy($<attr>$, "*=");
                //$<code>$=code++;
                //print_info($<code>$, "MUL_ASSIGN", "*=", NULL);
            }
            ;

unary_op:    ADDOP{
                strcpy($<attr>$, $<attr>1);
            }
            ;
bool_op:    EQ {
                strcpy($<attr>$, "==");
            }
            | GREATER{
                strcpy($<attr>$, ">");
            }
            | LESSER{
                strcpy($<attr>$, "<");
            }
            | GREQ{
                strcpy($<attr>$, ">=");
            }
            LEEQ{
                strcpy($<attr>$, "<=");
            }
            ;
logic_op:   AND{
                strcpy($<attr>$, "&&");
            } 
            | OR{
                strcpy($<attr>$, "||");
            }
            ;
id: ID {
    $<id>$ = new IdentifierNode($<attr>1);
}
;
factor: id {
            $<expr>$ = $<id>1;
        }
    | NUM {
        $<expr>$ = NumNode::GetNumNode($<attr>1);
    }
    | STRING {
        $<expr>$ = new StringNode($<attr>1);
    }
    | func_call_expr {
        $<expr>$ = $<expr>1;
    }
    | LP assign_expr RP {
        $<expr>$ = $<expr>2;
    }
    ;
postfix_expr:    factor {
                    $<expr>$=$<expr>1;
                }
                |postfix_expr LSB factor RSB{
                    $<expr>$ = new ArrayOffsetNode($<expr>1, $<expr>3);
                }
                |postfix_expr INC_OP{
                    $<expr>$ = new UnaryOpNode("++", $<expr>1, false);
                }
                |postfix_expr DEC_OP{
                    $<expr>$ = new UnaryOpNode("--", $<expr>1, false);
                }
                ;
unary_expr:  postfix_expr{
                $<expr>$ = $<expr>1;
            }
            |INC_OP postfix_expr{
                $<expr>$ = new UnaryOpNode("++", $<expr>2, true);
            }
            |DEC_OP postfix_expr{
                $<expr>$ = new UnaryOpNode("--", $<expr>2, false);
            }
            |unary_op unary_expr{
                $<expr>$ = new UnaryOpNode($<attr>1, $<expr>2, false);
            }
            ;
term: term MULTOP unary_expr {
        $<expr>$ = new BinOpNode($<attr>2, $<expr>1, $<expr>3);
    }
    | unary_expr{
        $<expr>$ = $<expr>1;
    }
    ;
calculate_expr:     calculate_expr ADDOP term {
                        $<expr>$ = new BinOpNode($<attr>2, $<expr>1, $<expr>3);
                    }
                    | term{
                        $<expr>$ = $<expr>1;
                    }
                    ;
bool_expr:   bool_expr bool_op calculate_expr {
            $<expr>$ = new BinOpNode($<attr>2, $<expr>1, $<expr>3);
        }
        | calculate_expr{
            $<expr>$ = $<expr>1;
        }
        ;
logic_expr:   logic_expr logic_op bool_expr {
            $<expr>$ = new BinOpNode($<attr>2, $<expr>1, $<expr>3);
        }
        | bool_expr{
            $<expr>$ = $<expr>1;
        }
        ;
expr_list:  expr_list COMMA logic_expr{
                //DONE:Add children code to attr
                $<exprListPtr>$ = $<exprListPtr>1;
                $<exprListPtr>$->push_back($<expr>3);
            }
            | logic_expr{
                $<exprListPtr>$ = new ExprList();
                $<exprListPtr>$->push_back($<expr>1);
            }
            ;
assign_expr: unary_expr assign_op assign_expr {
                        $<expr>$ = new BinOpNode($<attr>2, $<expr>1, $<expr>3);
                    }
                    | logic_expr {
                        $<expr>$ = $<expr>1;
                    }
                    ;
expr:           expr COMMA assign_expr{
                    //DONE:Add children code to attr
                    $<exprListPtr>$ = $<exprListPtr>1;
                    $<exprListPtr>$->push_back($<expr>3);
                }
                | assign_expr{
                    $<exprListPtr>$ = new ExprList();
                    $<exprListPtr>$->push_back($<expr>1);
                }
                ;
expr_statement: SEMI{
                    $<exprStmt>$ = new ExpressionStatementNode(NULL);
                }
                | expr SEMI{
                    $<exprStmt>$ = new ExpressionStatementNode(*$<exprListPtr>1);
                }
                ;
type:   TYPE {
            $<id>$ = new IdentifierNode($<attr>1);
        }
        ;

declarator:      id{
                    $<expr>$=$<id>1;
                }
                |declarator LSB factor RSB{
                    $<expr>$ = new ArrayOffsetNode($<expr>1, $<expr>3);
                }
                |LP declarator RP{
                    $<expr>$=$<expr>2;
                }
                ;
initializer:     assign_expr{
                    $<expr>$ = $<expr>1;
                }       
                |LB initializer_list RB{
                    $<expr>$ = new InitListNode(*$<exprListPtr>2);
                }
                ;
initializer_list:    initializer{
                        $<exprListPtr>$ = new ExprList();
                        $<exprListPtr>$->push_back($<expr>1);
                    }
                    |initializer_list COMMA initializer{
                        $<exprListPtr>$ = $<exprListPtr>1;
                        $<exprListPtr>$->push_back($<expr>3);
                    }
                    ;

init_declarator: declarator{
                    $<expr>$ = $<expr>1;
                }
                |declarator ASSIGN initializer{
                    $<expr>$ = new BinOpNode("=", $<expr>1, $<expr>3);
                }
                ;
init_declarator_list: init_declarator{
                        $<exprListPtr>$ = new ExprList();
                        $<exprListPtr>$->push_back($<expr>1);
                    }
                    | init_declarator_list COMMA init_declarator{
                        $<exprListPtr>$ = $<exprListPtr>1;
                        $<exprListPtr>$->push_back($<expr>3);
                    }
                    ;
var_decl:   type init_declarator_list SEMI{
                $<decl>$ = new VarDeclNode($<id>1, *$<exprListPtr>2);
            }
            ;
statement:  var_decl{$<stmt>$=$<decl>1;}
            | expr_statement{$<stmt>$=$<exprStmt>1;}
            | for_statement{$<stmt>$=$<stmt>1;}
            | if_statement{$<stmt>$=$<stmt>1;}
            | while_statement{$<stmt>$=$<stmt>1;}
            | statement_block{$<stmt>$=$<stmt>1;}
            | RETURN assign_expr SEMI{
                $<stmt>$ = new ReturnNode($<expr>2);
            }
            ;
statement_list: statement_list statement{
                    $<stmtListPtr>$ = $<stmtListPtr>1;
                    $<stmtListPtr>$->push_back($<stmt>2);
                }
                | statement{
                    $<stmtListPtr>$ = new StatementList();
                    $<stmtListPtr>$->push_back($<stmt>1);
                }
                ;
statement_block:    LB statement_list RB{
                        $<stmt>$ = new BlockNode(*$<stmtListPtr>2);
                    }
                    ;
if_statement:   IF LP expr RP statement {
                    $<stmt>$ = new IfNode(*$<exprListPtr>3, $<stmt>5);
                }
                | IF LP expr RP statement ELSE statement{
                    
                    $<stmt>$ = new IfNode(*$<exprListPtr>3, $<stmt>5, $<stmt>7);
                }
                ;
while_statement:WHILE LP expr RP statement{
                    $<stmt>$ = new WhileNode(*$<exprListPtr>3, $<stmt>5);
                }
                ;
for_statement:  FOR LP expr_statement expr_statement expr RP statement{
                $<stmt>$ = new ForNode($<exprStmt>3,$<exprStmt>4,*$<exprListPtr>5, $<stmt>7);
            }
            | FOR LP expr_statement expr_statement RP statement{
                $<stmt>$ = new ForNode($<exprStmt>3,$<exprStmt>4,ExprList(), $<stmt>6);
            }
            ;
func_param: type {
                $<funcParamDecl>$ = new FuncParamDeclNode($<id>1, NULL, NULL);
            }
            | type id{
                $<funcParamDecl>$ = new FuncParamDeclNode($<id>1, $<id>2, NULL);
            }
            | type id ASSIGN factor{
                $<funcParamDecl>$ = new FuncParamDeclNode($<id>1, $<id>2, $<expr>4);
            }
            ;
func_param_list:    func_param_list COMMA func_param{
                        //DONE:Add children code to attr
                        $<funcParamDeclListPtr>$ = $<funcParamDeclListPtr>1;
                        $<funcParamDeclListPtr>$->push_back($<funcParamDecl>1);
                    }
                    | func_param{
                        $<funcParamDeclListPtr>$ = new FuncParamDeclList();
                        $<funcParamDeclListPtr>$->push_back($<funcParamDecl>1);
                    }
                    ;
func_param_block:   LP func_param_list RP {
                        $<funcParamDeclListPtr>$ = $<funcParamDeclListPtr>2;
                    }
                    | LP RP{
                        $<funcParamDeclListPtr>$ = new FuncParamDeclList();
                    }
                ;
func_decl:  type ID func_param_block statement_block{
                $<decl>$ = new FuncDeclNode($<id>1, $<attr>2, *$<funcParamDeclListPtr>3, $<stmt>4);
            }
            ;
func_call_param_block:  LP expr_list RP{
                            $<exprListPtr>$ = $<exprListPtr>2;
                        }
                        | LP RP {
                            $<exprListPtr>$ = new ExprList();
                        }
                        ;
func_call_expr: ID func_call_param_block {
                    $<expr>$ = new FunCallNode($<attr>1, *$<exprListPtr>2);
                }
                ;
%%
int main(int argc, char ** argv){
    if(argc<2){
        printf("Need filename");
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    
    if(file==NULL){
        printf("File:%s not exists!", argv[1]);
        return 1;
    }
    yyin = file;
    symtab = NewTab();
    yyparse();
}
void yyerror(char* s)
{
    fprintf(stderr,"%s:%s",s,yytext);
}
int yywrap()
{
    return 1;
}

void print_info(int code, char* type_str,char* sym_info, int* children_info)
{
    char info_str[] = "%d : %s,\t\t\t%s,\t\tChildren:%s \n";
    char children_str[30]=" ";
    if(children_info!=NULL){
        for(int i=1;i<=children_info[0];++i){
            char num_str[10];
            sprintf(num_str, "%d", children_info[i]);
            strcat(children_str, num_str);
            strcat(children_str, " ");
        }
    }
    printf(info_str, code, type_str, sym_info, children_str);
}
