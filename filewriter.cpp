#include "filewriter.h"


FileWriter::FileWriter(const std::string& outputFile, const std::streampos& firstFrameNumber, char splitter)
	: m_output(outputFile)
	, m_lastFrameNumber(firstFrameNumber - std::streampos(1))
	, m_splitter(splitter)
{
	if(!m_output.is_open())
		throw OpenErrorException(outputFile);
}

FileWriter::~FileWriter()
{
	m_output.close();
}

void FileWriter::write(const Frame& frame) noexcept(false)
{
	std::unique_lock<std::mutex> lock(m_outputMtx);

	while(m_lastFrameNumber + std::streampos(1) != frame.number)
		m_outputCv.wait(lock);

	for(int16_t c: frame.data)
		m_output << std::hex << c;
	m_output << m_splitter;

	m_lastFrameNumber = frame.number;
	m_outputCv.notify_all();
}
