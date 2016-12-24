%{
#include "stdafx.h"
#include "asm.h"
#include "asm_yacc.h"
#define YYSTYPE asm_yacc_result
int yylex(YYSTYPE* lvalp, asm_yacc_proc* proc);
void yyerror(asm_yacc_proc* proc, const char* err);
%}

%define api.pure full
%param {asm_yacc_proc* proc}
%token ERR_TOKEN BRACKET_LEFT BRACKET_RIGHT NUMBER ADD SUB MUL DIV MOD SHL SHR AND XOR BOR SUB_LEFT SUB_RIGHT REG SPEC_REG REG_SHIFT_ALLOWED PUSH POP PEEK PICK
%start term

%%

expr_1 : NUMBER { $$.data = $1.data; }
 | BRACKET_LEFT expr_8 BRACKET_RIGHT { $$.data = $2.data; }
;
expr_2 : expr_1 { $$.data = $1.data; }
 | ADD expr_1 { $$.data = $2.data; }
 | SUB expr_1 { $$.data = -$2.data; }
expr_3 : expr_2 { $$.data = $1.data; }
 | expr_3 MUL expr_2 { $$.data = $1.data * $3.data; }
 | expr_3 DIV expr_2 { $$.data = $1.data / $3.data; }
 | expr_3 MOD expr_2 { $$.data = $1.data % $3.data; }
;
expr_4 : expr_3 { $$.data = $1.data; }
 | expr_4 ADD expr_3 { $$.data = $1.data + $3.data; }
 | expr_4 SUB expr_3 { $$.data = $1.data - $3.data; }
;
expr_5 : expr_4 { $$.data = $1.data; }
 | expr_5 SHL expr_4 { $$.data = $1.data << $3.data; }
 | expr_5 SHR expr_4 { $$.data = $1.data >> $3.data; }
;
expr_6 : expr_5 { $$.data = $1.data; }
 | expr_6 AND expr_5 { $$.data = $1.data & $3.data; }
;
expr_7 : expr_6 { $$.data = $1.data; }
 | expr_7 XOR expr_6 { $$.data = $1.data ^ $3.data; }
;
expr_8 : expr_7 { $$.data = $1.data; }
 | expr_8 BOR expr_7 { $$.data = $1.data | $3.data; }
;
const : expr_8 { $$.data = $1.data; }
;
reg : REG { $$.res = $1.res; }
 | REG_SHIFT_ALLOWED { $$.res = $1.res; }
;
spec_reg : SPEC_REG { $$.res = $1.res; }
;
addr : SUB_LEFT const SUB_RIGHT { $$.res = 0x1E; $$.data = $2.data; }
;
addr_reg : SUB_LEFT reg SUB_RIGHT { $$.res = $2.res + 0x08; }
;
addr_reg_shift : SUB_LEFT REG_SHIFT_ALLOWED const SUB_RIGHT { $$.res = $2.res + 0x10; $$.data = $3.data; }
 | reg SUB_LEFT const SUB_RIGHT { $$.res = $1.res + 0x10; $$.data = $3.data; }
;
stack_op : PUSH { if (proc->is_a) YYABORT; $$.res = 0x18; }
 | POP { if (!proc->is_a) YYABORT; $$.res = 0x18; }
 | PEEK { $$.res = 0x19; }
 | PICK const { $$.res = 0x1A; $$.data = $2.data; }
;
arg : const { $$.res = 0x1F; $$.data = $1.data; }
 | reg { $$.res = $1.res; }
 | spec_reg { $$.res = $1.res; }
 | addr { $$ = $1; }
 | addr_reg { $$.res = $1.res; }
 | addr_reg_shift { $$ = $1; }
 | stack_op { $$ = $1; }
term : arg { proc->result = $1; }

%%

int8_t lexer_state_op[][27] = {
	{1,2,3,0,13,0,0,0,7,8,0,0,0,0,0,9,0,0,11,0,0,0,0,4,5,6,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-3,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-4,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-5,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-7,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-8,},
	{0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11,},
};
uint8_t lexer_state_op_final[] = { 0,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x1C,0x1B,0x1D, };

uint16_t read_num(asm_yacc_proc& proc)
{
	const char *itr = proc.itr, *itr_end = proc.itr_end;
	char *num_end;
	uint16_t result = static_cast<uint16_t>(std::strtol(itr, &num_end, 0));
	if (num_end == itr || num_end > itr_end)
		throw(assembler_error());
	if (errno == ERANGE)
	{
		errno = 0;
		throw(assembler_error());
	}
	proc.itr = num_end;
	return result;
}

int read_reg(asm_yacc_proc& proc, uint16_t& ret)
{
	constexpr int state_default = 26;
	const char *itr = proc.itr, *itr_end = proc.itr_end;
	int state = 0;
	do
	{
		if (itr == itr_end)
			return 0;
		if (!isalpha(*itr))
			return 0;
		state = lexer_state_op[state][toupper(*itr) - 'A'];
		if (state == 0)
		{
			state = lexer_state_op[state][state_default];
			if (state == 0)
				return 0;
			else if (state < 0)
			{
				ret = lexer_state_op_final[-state];
				proc.itr = itr;
				if (ret > 0x7)
					return SPEC_REG;
				else
					return REG;
			}
		}
		itr++;
	} while (state > 0);
}

int read_letters(asm_yacc_proc& proc, uint16_t& ret)
{
	int token = read_reg(proc, ret);
	if (token != 0)
		return token;
	
	const char *itr = proc.itr, *itr_end = proc.itr_end;
	for (; itr != itr_end; itr++)
		if (!isalnum(*itr))
			break;
	
	std::string symbol(proc.itr, itr);
	proc.itr = itr;
	proc.used_symbols.emplace(symbol);
	auto iter = proc.symbols.find(symbol);
	if (iter == proc.symbols.end())
	{
		proc.unresolved = true;
		ret = 0;
	}
	else
	{
		ret = iter->second;
	}
	return NUMBER;
}

int yylex(YYSTYPE* lvalp, asm_yacc_proc* proc)
{
	const char *itr = proc->itr, *itr_end = proc->itr_end;
	while (itr != itr_end)
	{
		if (!isspace(*itr))
			break;
		itr++;
	}
	if (itr == itr_end)
		return 0;

	int result;
	switch (*itr)
	{
		case '(':
			result = BRACKET_LEFT;
			break;
		case ')':
			result = BRACKET_RIGHT;
			break;
		case '[':
			result = SUB_LEFT;
			break;
		case ']':
			result = SUB_RIGHT;
			break;
		case '+':
			result = ADD;
			break;
		case '-':
			result = SUB;
			break;
		case '*':
			result = MUL;
			break;
		case '/':
			result = DIV;
			break;
		case '%':
			result = MOD;
			break;
		case '<':
			itr++;
			if (*itr == '<')
				result = SHL;
			else
				return ERR_TOKEN;
			break;
		case '>':
			itr++;
			if (*itr == '>')
				result = SHL;
			else
				return ERR_TOKEN;
			break;
		case '&':
			result = AND;
			break;
		case '^':
			result = XOR;
			break;
		case '|':
			result = BOR;
			break;
		default:
			if (isdigit(*itr))
			{
				lvalp->data = read_num(*proc);
				return NUMBER;
			}
			if (isalpha(*itr))
			{
				uint16_t ret;
				result = read_letters(*proc, ret);
				if (result == NUMBER)
					lvalp->data = ret;
				else if (result == REG)
				{
					itr = proc->itr;
					itr_end = proc->itr_end;
					if (itr < itr_end && (*itr == '+' || *itr == '-'))
						result = REG_SHIFT_ALLOWED;
					lvalp->res = ret;
				}
				return result;
			}
			return ERR_TOKEN;
	}
	itr++;
	proc->itr = itr;
	return result;
}

void yyerror(asm_yacc_proc* proc, const char* err)
{
	throw(assembler_error());
}
