#pragma once

class assembler
{
private:
	struct instruction
	{
		instruction(uint16_t code)
			:op(code & 0x1F), b((code >> 5) & 0x1F), a(code >> 10)
		{}
		const instruction& operator=(uint16_t code) { op = code & 0x1F; b = (code >> 5) & 0x1F; a = code >> 10; return *this; }
		operator uint16_t()
		{
			return ((a << 10) | (b << 5)) | op;
		}

		uint8_t op, b, a;
	};

	struct inter_instruction
	{
		inter_instruction() :ins(0), a(0), b(0), addr(0) {}
		inter_instruction(uint16_t _addr) :ins(0), a(0), b(0), addr(_addr) {}

		instruction ins;
		uint16_t a, b;
		std::vector<uint16_t> extra;
		uint16_t addr;
		int size;

		std::string ins_str;
		std::unordered_set<std::string> is_symbol, used_symbol;
		bool unresolved_symbol;
	};
public:
	assembler() { instructions.emplace_back(0); }

	void read(uint16_t* out, size_t size);
	void write(std::istream& in);

	uint16_t get_symbol(const std::string& str) { return symbols.at(str); }
private:
	bool process_line(inter_instruction &ins);
	void process_ins(inter_instruction &ins);

	bool re_evaluate_unresolved(const std::list<inter_instruction>::iterator& itr, const std::list<inter_instruction>::iterator& itr_end);
	bool re_evaluate(const std::list<inter_instruction>::iterator& itr, const std::list<inter_instruction>::iterator& itr_end, const std::unordered_set<std::string>& changed_symbol);
	void shrink(const std::list<inter_instruction>::iterator& itr, const std::list<inter_instruction>::iterator& itr_end, std::unordered_set<std::string>& changed_symbol);

	void asm_ins(const char** itr, const char* itr_end, inter_instruction& ret);
	void asm_arg(const char* itr, const char* itr_end, bool is_a, inter_instruction& ret);
	void asm_spec(const char* itr, const char* itr_end, inter_instruction& ret);

	std::list<inter_instruction> instructions;
	std::unordered_map<std::string, uint16_t> symbols;
};

class assembler_error :public std::runtime_error
{
public:
	assembler_error() :std::runtime_error("Internal assembler error") {}
	assembler_error(const char* err) :std::runtime_error(err) {}
};
