#include <iostream>
#include <string.h>
#include <chrono>
//#include <openssl/md5.h>
#include "filereader.h"
#include "workpool.h"
#include "filewriter.h"


class Md5Writer
{
public:
	Md5Writer(Frame&& frame, FileWriter& fw)
		: m_frame(std::move(frame))
		, m_fw(fw)
	{}

	void operator()()
	{
//		Frame hash{ std::vector<unsigned char>(MD5_DIGEST_LENGTH, 0), m_frame.number };
//		MD5(m_frame.data.data(), m_frame.data.size(), hash.data.data());
//		m_fw.write(hash);
	}

private:
	Frame m_frame;
	FileWriter& m_fw;
};


int main(int argc, char* argv[])
{
	std::string input;
	std::string output;
	size_t frameSize = 1024 * 1024;

	try
	{
		const auto assertIndexValid = [argc](int index)
		{
			if(index >= argc)
				throw std::logic_error("Wrong parameters");
			return index;
		};

		for(int i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "-i") == 0)
				input = argv[assertIndexValid(++i)];
			else if(strcmp(argv[i], "-o") == 0)
				output = argv[assertIndexValid(++i)];
			else if(strcmp(argv[i], "-s") == 0)
				frameSize = std::stoi(argv[assertIndexValid(++i)]);
			else
				throw std::logic_error(std::string("Unknown parameter \"") + argv[i] + "\"");
		}

		if(input.empty())
			throw std::logic_error("Input file not set");

		if(output.empty())
			throw std::logic_error("Output file not set");

		std::cout << "Parameters set: input file = " << input << ", output file = " << output << ", frame size = " << frameSize << std::endl;

		FileReader fr(input, frameSize);
		FileWriter fw(output, 0);

		WorkPool ws(std::thread::hardware_concurrency() - 1);

		auto start = std::chrono::system_clock::now();
		while(!fr.eof())
			ws.put(Md5Writer{ fr.readNext(), fw });
		ws.wait();
		auto end = std::chrono::system_clock::now();

		std::cout << "Read " << fr.readCount() << " frames in "
				  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds.";

		return 0;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}
}
