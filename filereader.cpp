#include "filereader.h"


FileReader::FileReader(const std::string& inputFile, size_t frameSize) noexcept(false)
	: m_input(inputFile, std::ios_base::in | std::ios_base::binary)
	, m_frameSize(frameSize)
{
	if(frameSize == 0)
		throw WrongFrameSizeException(frameSize);

	if(!m_input.is_open())
		throw OpenErrorException(inputFile);

	m_input.seekg(0, m_input.end);
	m_inputSize = m_input.tellg();
	m_input.seekg(0, m_input.beg);
}

FileReader::~FileReader()
{
	m_input.close();
}

Frame FileReader::readNext() noexcept(false)
{
	const auto readSize = static_cast<size_t>(std::min(std::streampos(m_inputSize - m_input.tellg()), std::streampos(m_frameSize)));
	Frame f{ std::vector<unsigned char>(readSize), m_readCount };

	if(!m_input.read(reinterpret_cast<char*>(f.data.data()), f.data.size()))
		throw ReadErrorException(m_input.tellg(), m_frameSize, m_readCount);

	m_readCount = m_input.tellg() / m_frameSize + (m_input.tellg() % m_frameSize > 0 ? 1 : 0);
	if(m_inputSize == m_input.tellg())
		m_eof = true;

	return f;
}
