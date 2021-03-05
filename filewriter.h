#ifndef SIGNATUREWRITER_H
#define SIGNATUREWRITER_H

#include <condition_variable>
#include "commons.h"


class FileWriter
{
public:
	FileWriter(const std::string& outputFile, const std::streampos& firstFrameNumber, char splitter = '\n') noexcept(false);
	virtual ~FileWriter();

	void write(const Frame& frame) noexcept(false);

private:
	std::ofstream m_output;
	std::streampos m_lastFrameNumber;
	char m_splitter;
	std::mutex m_outputMtx;
	std::condition_variable m_outputCv;
};

#endif // SIGNATUREWRITER_H
