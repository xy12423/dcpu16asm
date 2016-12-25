#include "stdafx.h"
#include "asm.h"
#include "asm_yacc.h"
#include "utils.h"

bool _skip_incpc[0x40] = {
	false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false,
	true, true, true, true, true, true, true, true,
	false, false, true, false, false, false, true, true,
	false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false
};

int8_t lexer_state_op[][27] = {
	{ 4,25,0,14,0,0,0,64,36,52,0,0,9,69,0,74,0,60,1,0,0,0,0,28,0,0,0, },
	{ 0,47,0,0,2,0,0,31,0,0,0,0,0,0,0,0,0,0,0,49,7,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1, },
	{ 0,0,0,5,0,0,0,0,0,0,0,0,0,23,0,0,0,0,33,0,0,0,0,0,0,0,0, },
	{ 0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,46,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2, },
	{ 0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-3, },
	{ 0,0,0,21,0,0,0,0,0,0,0,12,0,0,19,0,0,0,0,0,10,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-4, },
	{ 0,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-5, },
	{ 72,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,17,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6, },
	{ 0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-7, },
	{ 0,0,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-8, },
	{ 0,0,0,0,0,0,0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-9, },
	{ 0,0,0,24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-10, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-11, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,29,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,30,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-12, },
	{ 0,0,0,0,0,0,0,0,0,0,0,35,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-13, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-14, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-15, },
	{ 57,0,0,0,0,37,0,0,0,0,0,0,0,55,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 43,38,39,0,40,0,42,0,0,0,0,44,0,41,0,0,0,0,0,0,45,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-16, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-17, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-18, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-19, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-20, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-21, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-22, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-23, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-25, },
	{ 0,0,0,51,0,0,0,0,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-26, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-27, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,54,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-28, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-29, },
	{ 0,0,0,0,0,0,58,0,0,0,0,0,0,0,0,0,63,0,59,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-30, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-31, },
	{ 0,0,0,0,0,61,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-32, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-33, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,68,0,0,0,0,66,0,0,67,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-34, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-35, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-36, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,71,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-37, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,73,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-38, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,78,0,0,0,0,0,75,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,76,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,77,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-39, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,79,0,0,0,0,0,0,0,0,0,0,0, },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-40, },
};
uint8_t lexer_state_op_final[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,26,27,30,31,33,40,41,42,43,44,48,49,50,65,128,129,130, };

char read_esc_char(const char*& itr, const char* itr_end)
{
	thread_local static std::stringstream tmp;
	char replace = '\0';
	switch (*itr)
	{
		case 'a':
			replace = '\a';
			break;
		case 'b':
			replace = '\b';
			break;
		case 'f':
			replace = '\f';
			break;
		case 'n':
			replace = '\n';
			break;
		case 'r':
			replace = '\r';
			break;
		case 't':
			replace = '\t';
			break;
		case 'v':
			replace = '\v';
			break;
		case '\\':
			replace = '\\';
			break;
		case '\'':
			replace = '\'';
			break;
		case '"':
			replace = '"';
			break;
		case 'x':
		{
			tmp << std::hex;
			++itr;
			if (!isxdigit(*itr))
				throw(assembler_error("Invalid argument:Unrecognized esc char"));
			for (int i = 0; i < 2 && itr < itr_end; i++)
			{
				tmp << *itr;
				++itr;
				if (!isxdigit(*itr))
					break;
			}
			--itr;
			int tmpn = 0;
			tmp >> tmpn;
			replace = static_cast<char>(tmpn);
			break;
		}
		default:
		{
			if (*itr > '7' || *itr < '0')
				throw(assembler_error("Invalid argument:Unrecognized esc char"));
			tmp << std::oct;
			for (int i = 0; i < 3 && itr < itr_end; i++)
			{
				tmp << *itr;
				++itr;
				if (*itr > '7' || *itr < '0')
					break;
			}
			--itr;
			int tmpn = 0;
			tmp >> tmpn;
			replace = static_cast<char>(tmpn);
		}
	}

	return replace;
}

void assembler::asm_ins(const char** itr, const char* itr_end, inter_instruction& ret)
{
	constexpr int state_default = 26;
	int state = 0;
	do
	{
		int new_state;
		if (*itr == itr_end || !isalpha(**itr))
		{
			new_state = lexer_state_op[state][state_default];
			if (new_state == 0)
				throw(assembler_error("Invalid instruction"));
		}
		else
		{
			new_state = lexer_state_op[state][toupper(**itr) - 'A'];
			if (new_state == 0)
			{
				new_state = lexer_state_op[state][state_default];
				if (new_state == 0)
					throw(assembler_error("Invalid instruction"));
			}
		}
		if (new_state < 0)
		{
			uint8_t op = lexer_state_op_final[-new_state];
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
				op -= 0x80;
				ret.ins = 0;
				ret.b = op;
			}
		}
		state = new_state;
		(*itr)++;
	} while (state > 0);
}

void assembler::asm_arg(const char* itr, const char* itr_end, bool is_a, inter_instruction& ret)
{
	asm_yacc_proc proc(itr, itr_end, is_a, symbols, ret.used_symbol);
	if (yyparse(&proc) != 0)
		throw(assembler_error("Invalid operand"));
	ret.unresolved_symbol = ret.unresolved_symbol || proc.unresolved;
	asm_yacc_result &result = proc.result;
	if (!proc.unresolved)
	{
		if (is_a && result.res == 0x1F)
		{
			if (result.data == 0xFFFF)
				result.res = 0x20;
			else if (result.data < 0x1F)
				result.res = 0x21 + result.data;
		}
		else if (0x10 <= result.res && result.res <= 0x17)
		{
			if (result.data == 0)
				result.res -= 0x8;
		}
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

void assembler::asm_spec(const char* itr, const char* itr_end, inter_instruction& ret)
{
	switch (ret.b)
	{
		case 0:	//DAT
		{
			enum { STATE_START, STATE_COMMA, STATE_STR, STATE_SPACE, STATE_CONST } state = STATE_START;
			const char *begin = nullptr;
			for (; itr < itr_end; itr++)
			{
				switch (state)
				{
					case STATE_START:
						if (isspace(*itr))
							break;
						begin = itr;
						if (*itr == '[')
							state = STATE_SPACE;
						else if (*itr == '"')
							state = STATE_STR;
						else
							state = STATE_CONST;
						break;
					case STATE_COMMA:
						if (*itr != ',')
							break;
						state = STATE_START;
						break;
					case STATE_STR:
						if (*itr == '"')
							state = STATE_COMMA;
						else if (*itr == '\\')
						{
							++itr;
							if (itr >= itr_end)
								throw(assembler_error("Invalid argument"));
							ret.extra.push_back(read_esc_char(itr, itr_end));
						}
						else
							ret.extra.push_back(*itr);
						break;
					case STATE_SPACE:
						if (*itr == ']')
						{
#ifdef _DEBUG
							if (begin == nullptr)
								throw(assembler_error());
#endif
							asm_arg(begin + 1, itr, false, ret);
							if (ret.ins.b != 0x1F)
								throw(assembler_error("Invalid argument"));
							ret.ins.b = 0;
							for (int i = 0; i < ret.b; i++)
								ret.extra.push_back(0);
							state = STATE_COMMA;
						}
						break;
					case STATE_CONST:
						if (*itr == ',')
						{
#ifdef _DEBUG
							if (begin == nullptr)
								throw(assembler_error());
#endif
							asm_arg(begin, itr, false, ret);
							if (ret.ins.b != 0x1F)
								throw(assembler_error("Invalid argument"));
							ret.ins.b = 0;
							ret.extra.push_back(ret.b);
							state = STATE_START;
						}
						break;
				}
			}
			if (state == STATE_CONST)
			{
				asm_arg(begin, itr, false, ret);
				if (ret.ins.b != 0x1F)
					throw(assembler_error("Invalid argument"));
				ret.ins.b = 0;
				ret.extra.push_back(ret.b);
				state = STATE_START;
			}
			else if (state != STATE_COMMA && state != STATE_START)
				throw(assembler_error("Invalid argument"));
			ret.size = ret.extra.size();
			break;
		}
		case 1: //PUSH
		{
			ret.ins.op = 1; //SET
			ret.ins.b = 0x18; //PUSH
			asm_arg(itr, itr_end, true, ret);
			ret.size = 1;
			if (_skip_incpc[ret.ins.a])
				ret.size += 1;
			break;
		}
		case 2: //POP
		{
			ret.ins.op = 1; //SET
			ret.ins.a = 0x18; //POP
			asm_arg(itr, itr_end, false, ret);
			ret.size = 1;
			if (_skip_incpc[ret.ins.b])
				ret.size += 1;
			break;
		}
	}
}

void assembler::read(uint16_t* out, size_t size)
{
	uint16_t *out_end = out + size;
	for (auto itr = instructions.begin(), itr_end = instructions.end(); itr != itr_end && out < out_end; itr++)
	{
		inter_instruction &ins = *itr;
		if (ins.ins == 0)
		{
			size_t size_copy = std::min(static_cast<size_t>(out_end - out), ins.extra.size());
			uint16_t *data = ins.extra.data();
			for (size_t i = 0; i < size_copy; i++, out++, data++)
				*out = *data;
			continue;
		}
		*out = ins.ins;
		out++;
		if (ins.ins.op != 0)
		{
			if (_skip_incpc[ins.ins.b])
			{
				if (out >= out_end) break;
				*out = itr->b;
				out++;
			}
			if (_skip_incpc[ins.ins.a])
			{
				if (out >= out_end) break;
				*out = itr->a;
				out++;
			}
		}
		else if (ins.ins.b != 0)
		{
			if (_skip_incpc[ins.ins.a])
			{
				if (out >= out_end) break;
				*out = itr->a;
				out++;
			}
		}
	}
}

void assembler::write(std::istream& in)
{
	std::string line;
	std::getline(in, line);

	auto itr = instructions.end();
	--itr;

	while (!in.eof())
	{
		inter_instruction &ins = instructions.back();
		ins.ins_str = std::move(line);
		if (process_line(ins))
			instructions.emplace_back(ins.addr + ins.size);
		std::getline(in, line);
	}
	auto itr_end = instructions.end();
	--itr_end;
	
	std::unordered_set<std::string> changed_symbol;
	bool done = re_evaluate_unresolved(itr, itr_end);
	while (!done)
	{
		shrink(itr, itr_end, changed_symbol);
		done = re_evaluate(itr, itr_end, changed_symbol);
	}
}

bool assembler::process_line(inter_instruction &ins)
{
	std::string &line = ins.ins_str;
	trim(line);
	if (line.empty())
		return false;

	if (line.front() == ':')
	{
		if (!symbols.emplace(line.substr(1), ins.addr).second)
			throw(assembler_error("Dumplicate label"));
		ins.is_symbol.emplace(line.substr(1));
		return false;
	}

	process_ins(ins);
	return true;
}

void assembler::process_ins(inter_instruction &ins)
{
	std::string &line = ins.ins_str;

	ins.unresolved_symbol = false;
	ins.size = 1;
	const char *itr = line.data(), *itr_end = line.data() + line.size();

	asm_ins(&itr, itr_end, ins);
	if (ins.ins == 0)
	{
		asm_spec(itr, itr_end, ins);
		return;
	}
	while (itr != itr_end)
	{
		if (!isspace(*itr))
			break;
		itr++;
	}
	if (itr == itr_end)
	{
		if (ins.ins.op != 0 || ins.ins.b != 0)
			throw(assembler_error("Need operand"));
		return;
	}

	const char *itr_mid = itr;
	for (; itr_mid != itr_end; itr_mid++)
		if (*itr_mid == ',')
			break;
	if (itr_mid == itr_end)
	{
		if (ins.ins.op != 0)
			throw(assembler_error("Need operand"));
		asm_arg(itr, itr_end, true, ins);
		if (_skip_incpc[ins.ins.a])
			ins.size += 1;
		return;
	}
	asm_arg(itr, itr_mid, false, ins);
	asm_arg(itr_mid + 1, itr_end, true, ins);
	if (_skip_incpc[ins.ins.b])
		ins.size += 1;
	if (_skip_incpc[ins.ins.a])
		ins.size += 1;
}

bool assembler::re_evaluate_unresolved(const std::list<inter_instruction>::iterator& _itr, const std::list<inter_instruction>::iterator& itr_end)
{
	uint16_t addr = _itr->addr;
	std::list<inter_instruction>::iterator itr = _itr;
	for (; itr != itr_end; itr++)
	{
		inter_instruction &ins = *itr;
		if (ins.addr != addr)
			break;
		if (ins.unresolved_symbol)
			process_ins(ins);
		addr += ins.size;
	}
	if (itr == itr_end)
		return true;
	for (; itr != itr_end; itr++)
	{
		inter_instruction &ins = *itr;
		if (ins.unresolved_symbol)
			process_ins(ins);
		addr += ins.size;
	}
	return false;
}

bool assembler::re_evaluate(const std::list<inter_instruction>::iterator& _itr, const std::list<inter_instruction>::iterator& itr_end,
	const std::unordered_set<std::string>& changed_symbol)
{
	uint16_t addr = _itr->addr;
	std::list<inter_instruction>::iterator itr = _itr;
	for (; itr != itr_end; itr++)
	{
		inter_instruction &ins = *itr;
		if (ins.addr != addr)
			break;
		bool need_asm = false;
		for (const std::string& symbol : changed_symbol)
		{
			if (ins.used_symbol.count(symbol) > 0)
			{
				need_asm = true;
				break;
			}
		}
		if (need_asm)
			process_ins(ins);
		addr += ins.size;
	}
	if (itr == itr_end)
		return true;
	for (; itr != itr_end; itr++)
	{
		inter_instruction &ins = *itr;
		bool need_asm = false;
		for (const std::string& symbol : changed_symbol)
		{
			if (ins.used_symbol.count(symbol) > 0)
			{
				need_asm = true;
				break;
			}
		}
		if (need_asm)
			process_ins(ins);
		addr += ins.size;
	}
	return false;
}

void assembler::shrink(const std::list<inter_instruction>::iterator& _itr, const std::list<inter_instruction>::iterator& itr_end,
	std::unordered_set<std::string>& changed_symbol)
{
	changed_symbol.clear();
	uint16_t addr = _itr->addr;
	std::list<inter_instruction>::iterator itr = _itr;
	for (; itr != itr_end; itr++)
	{
		inter_instruction &ins = *itr;
		if (ins.addr != addr)
		{
			ins.addr = addr;
			if (!ins.is_symbol.empty())
			{
				for (const std::string& symbol : ins.is_symbol)
				{
					symbols.at(symbol) = addr;
					changed_symbol.emplace(symbol);
				}
			}
		}
		addr += ins.size;
	}
}
