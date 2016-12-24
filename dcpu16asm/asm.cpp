#include "stdafx.h"
#include "asm.h"
#include "asm_yacc.h"
#include "utils.h"

int8_t lexer_state_op[][26] = {
	{ 3,15,0,9,0,0,0,31,21,25,0,0,6,33,0,0,0,29,1,0,0,0,0,17,0,0, },
	{ 0,23,0,0,2,0,0,19,0,0,0,0,0,0,0,0,0,0,0,24,5,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0, },
	{ 0,0,0,4,0,0,0,0,0,0,0,0,0,14,0,0,0,0,20,0,0,0,0,0,0,0, },
	{ 0,0,0,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,0,0, },
	{ 0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,13,0,0,0,0,0,0,0,8,0,0,12,0,0,0,0,0,7,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,-7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,-9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,-10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-12,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,-15,0,0,0,0,0,-13,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-14,0,0,0,0,0,0,0,0, },
	{ 28,0,0,0,0,22,0,0,0,0,0,0,0,27,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ -21,-16,-17,0,-18,0,-20,0,0,0,0,-22,0,-19,0,0,0,0,0,0,-23,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25,0,0, },
	{ 0,0,0,-27,0,0,0,0,-26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,-30,0,0,0,0,0,0,0,0,0,-33,0,-31,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,-32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0, },
	{ 0,0,0,0,0,0,0,0,-36,0,0,0,0,-34,0,0,-35,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-37,0,0,0,0,0,0,0,0,0,0, },
};
uint8_t lexer_state_op_final[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,26,27,30,31,33,40,41,42,43,44,48,49,50,65, };

void assembler::asm_ins(const char** itr, const char* itr_end, inter_instruction& ret)
{
	int state = 0;
	do
	{
		if (*itr == itr_end)
			throw(assembler_error());
		if (!isalpha(**itr))
			throw(assembler_error());
		state = lexer_state_op[state][toupper(**itr) - 'A'];
		if (state == 0)
			throw(assembler_error());
		else if (state < 0)
		{
			uint8_t op = lexer_state_op_final[-state];
			if (op < 0x20)
			{
				ret.ins.op = op;
			}
			else if (op < 0x40)
			{
				ret.ins.op = 0;
				ret.ins.b = op & 0x1F;
			}
			else if (op < 0x80)
			{
				ret.ins.op = 0;
				ret.ins.b = 0;
				ret.ins.a = op & 0x3F;
			}
			else
			{
				throw(assembler_error());
			}
		}
		(*itr)++;
	} while (state > 0);
}

void assembler::asm_arg(const char* itr, const char* itr_end, bool is_a, inter_instruction& ret)
{
	asm_yacc_proc proc(itr, itr_end, is_a, symbols, ret.used_symbol);
	if (yyparse(&proc) != 0)
		throw(assembler_error());
	asm_yacc_result &result = proc.result;
	if (result.res == 0x1F)
	{
		if (!proc.unresolved)
		{
			if (result.data == 0xFFFF)
				result.res = 0x20;
			else if (result.data < 0x1F)
				result.res = 0x21 + result.data;
		}
	}
	else if (0x10 <= result.res && result.res <= 0x17)
	{
		if (result.data == 0)
			result.res -= 0x8;
	}
	if (is_a)
	{
		ret.ins.a = result.res;
		ret.a = result.data;
	}
	else
	{
		ret.ins.b = result.res;
		ret.b = result.data;
	}
}

void assembler::write(std::istream& in)
{
	std::string line;
	std::getline(in, line);

	while (!in.eof())
	{
		process_line(line);
		std::getline(in, line);
	}
}

void assembler::process_line(std::string& line)
{
	trim(line);
	if (line.empty())
		return;
	inter_instruction &ins = instructions.back();

	if (line.front() == ':')
	{
		symbols.emplace(line.substr(1), ins.addr);
		ins.is_symbol.emplace(line.substr(1));
		return;
	}

	const char *itr = line.data(), *itr_end = line.data() + line.size();

	asm_ins(&itr, itr_end, ins);
	while (itr != itr_end)
	{
		if (!isspace(*itr))
			break;
		itr++;
	}
	if (itr == itr_end)
	{
		if (ins.ins.op != 0 || ins.ins.b != 0)
			throw(assembler_error());
		return;
	}

	const char *itr_mid = itr;
	for (; itr_mid != itr_end; itr_mid++)
		if (*itr_mid == ',')
			break;
	if (itr_mid == itr_end)
	{
		if (ins.ins.op != 0)
			throw(assembler_error());
		asm_arg(itr, itr_end, true, ins);
		return;
	}
	asm_arg(itr, itr_mid, false, ins);
	asm_arg(itr_mid + 1, itr_end, true, ins);
}
