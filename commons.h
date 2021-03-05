#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <fstream>
#include <vector>

struct Frame
{
	std::vector<unsigned char> data;
	std::streampos number;
};


class OpenErrorException : public std::runtime_error
{
public:
	OpenErrorException(const std::string& fileName)
		: std::runtime_error(std::string("Failed to open \"") + fileName + "\"")
		, m_fileName(fileName)
	{}

	const std::string& filename() const { return m_fileName; }

private:
	std::string m_fileName;
};


class WrongFrameSizeException : public std::logic_error
{
public:
	WrongFrameSizeException(size_t frameSize)
		: std::logic_error(std::string("Wrong frame size ") + std::to_string(frameSize))
		, m_frameSize(frameSize)
	{}

	size_t frameSize() const { return m_frameSize; }

private:
	size_t m_frameSize;
};


class ReadErrorException : public std::runtime_error
{
public:
	ReadErrorException(std::streampos pos, size_t frameSize, std::streampos frameNumber)
		: std::runtime_error(std::string("Failed to read frame ") + std::to_string(frameNumber)
							 + " of size " + std::to_string(frameSize) + " bytes from position " + std::to_string(pos))
		, m_pos(pos)
		, m_frameSize(frameSize)
		, m_frameNumber(frameNumber)
	{}

	const std::streampos& pos() const { return m_pos; }
	size_t frameSize() const { return m_frameSize; }

private:
	std::streampos m_pos;
	size_t m_frameSize;
	std::streampos m_frameNumber;
};

#endif // COMMONS_H
