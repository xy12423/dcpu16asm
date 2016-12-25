#include "stdafx.h"
#include "asm.h"

void print_usage()
{

}

int main(int argc, char* argv[])
{
	std::string out, in;
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
			out = argv[++i];
		else
			in = argv[i];
	}

	std::ifstream fin(in);
	if (!fin || !fin.is_open())
	{
		std::cerr << "Unable to open input file" << std::endl;
		return EXIT_FAILURE;
	}
	assembler asem;
	try
	{
		asem.write(fin);
	}
	catch (assembler_error& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}

	constexpr size_t mem_size = 0x10000;
	std::unique_ptr<uint16_t[]> mem = std::make_unique<uint16_t[]>(mem_size);
	asem.read(mem.get(), mem_size);

	std::ofstream fout(out, std::ios::out | std::ios::binary);
	if (!fout || !fout.is_open())
	{
		std::cerr << "Unable to open output file" << std::endl;
		return EXIT_FAILURE;
	}
	fout.write(reinterpret_cast<char*>(mem.get()), mem_size * sizeof(uint16_t));
	fout.close();

	return EXIT_SUCCESS;
}
