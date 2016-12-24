#ifndef _H_ASM_YACC
#define _H_ASM_YACC

struct asm_yacc_result
{
	uint8_t res;
	uint16_t data;
};

struct asm_yacc_proc
{
	asm_yacc_proc(const char* _itr, const char* _itr_end, bool _is_a,
		std::unordered_map<std::string, uint16_t>& _symbols, std::unordered_set<std::string>& _used_symbols)
		:itr(_itr), itr_end(_itr_end), is_a(_is_a),
		symbols(_symbols), used_symbols(_used_symbols)
	{}

	const char *itr, *itr_end;
	bool is_a;

	std::unordered_map<std::string, uint16_t>& symbols;
	std::unordered_set<std::string>& used_symbols;
	bool unresolved = false;

	asm_yacc_result result;
};

int yyparse(asm_yacc_proc* proc);

#endif
