#pragma once

class assembler
{
private:
	struct instruction
	{
		instruction(uint16_t code)
			:op(code & 0x1F), b((code >> 5) & 0x1F), a(code >> 10)
		{}
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
		uint16_t addr;

		std::unordered_set<std::string> used_symbol, is_symbol;
	};
public:
	assembler() { instructions.emplace_back(0); }

	void read(uint16_t* out, size_t size);
	void write(std::istream& in);

	uint16_t get_symbol(const std::string& str) { return symbols.at(str); }
private:
	void process_line(std::string& line);

	void asm_ins(const char** itr, const char* itr_end, inter_instruction& ret);
	void asm_arg(const char* itr, const char* itr_end, bool is_a, inter_instruction& ret);

	std::deque<uint16_t> buf;
	std::deque<inter_instruction> instructions;
	std::unordered_map<std::string, uint16_t> symbols;
};

class assembler_error :public std::runtime_error
{
public:
	assembler_error() :std::runtime_error("Internal assembler error") {}
};
