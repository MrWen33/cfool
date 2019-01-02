%{
    #include<string.h>
    #include "cppytab.h"
    #include<stdio.h>
    #define SAVE sscanf(yytext, "%s", &yylval.attr)
%}
string      \"(\\.|[^"\\])*\"
anno        \/\/[^\n]*
numbers     ([0-9])*(\.)?([0-9])+
addop       [+-]
multop      [*/]
assign      [=]
equal       "=="
not_equal   "!="
lesser      "<"
greater     ">"
leeq        "<="
greq        ">="
lbrace      "{"
rbrace      "}"
lparenthese "("
rparenthese ")"
lsb         \[
rsb         \]
for        for
comma       ,
semi        ;
while       while
int         int
if          if
void        void
return      return
else        else
increase    "++"
decrease    "--"
add_assign  "+="
sub_assign  "-="
mul_assign  "*="
div_assign  "/="
logicand    \&\&
logicor     \|\|
id ([_a-zA-Z])([_a-zA-Z0-9])*
delim [ \n\t\r]  
ws {delim}* 
%%
{logicand}   {return AND;}
{logicor}    {return OR;}
{else}      {return ELSE;}
{if}        {return IF;}
{not_equal} {return NE;}
{increase}  {return INC_OP;}
{decrease}  {return DEC_OP;}
{add_assign}    {return ADD_ASSIGN;}
{sub_assign}    {return SUB_ASSIGN;}
{mul_assign}    {return MUL_ASSIGN;}
{div_assign}    {return DIV_ASSIGN;}
{return}    {return RETURN;}
{for}       {return FOR;}
{lsb}       {return LSB;}
{rsb}       {return RSB;}
{anno}       ;
{while}     {return WHILE;}
{comma}     {return COMMA;}
{semi}      {return SEMI;}
{string}    {SAVE; return STRING;}
{numbers}   {SAVE; return NUM;}
{addop}     {SAVE;return ADDOP;}
{multop}    {SAVE;return MULTOP;}
{equal}     {return EQ;}
{lesser}    {return LESSER;}
{greater}   {return GREATER;}
{leeq}      {return LEEQ;}
{greq}      {return GREQ;}
{assign}    {return ASSIGN;}
{lbrace}    {return LB;}
{rbrace}    {return RB;}
{lparenthese}   {return LP;}
{rparenthese}   {return RP;}
{void}      {SAVE; return TYPE;}
{int}       {SAVE; return TYPE;}
{id}        {SAVE; return ID;}
{ws}        ;
.           {printf(yytext);printf("\nERROR\n"); exit(1);}
%%
