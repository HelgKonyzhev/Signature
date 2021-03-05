#ifndef FILEREADER_H
#define FILEREADER_H

#include "commons.h"


class FileReader
{
public:
	FileReader(const std::string& inputFile, size_t frameSize) noexcept(false);
	virtual ~FileReader();

	Frame readNext() noexcept(false);
	const std::streampos& readCount() const { return m_readCount; }
	bool eof() const { return m_eof; }

private:
	std::ifstream m_input;
	size_t m_frameSize;
	std::streampos m_inputSize;
	std::streampos m_readCount = 0;
	bool m_eof = false;
};

#endif // FILEREADER_H
