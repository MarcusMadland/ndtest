/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/process.h>

#include <stdio.h>

#ifndef BASE_CONFIG_CRT_PROCESS
#	define BASE_CONFIG_CRT_PROCESS !(0  \
			|| BASE_CRT_NONE            \
			|| BASE_PLATFORM_EMSCRIPTEN \
			|| BASE_PLATFORM_PS4        \
			|| BASE_PLATFORM_WINRT      \
			|| BASE_PLATFORM_XBOXONE    \
			)
#endif // BASE_CONFIG_CRT_PROCESS

namespace base
{
#if BASE_CONFIG_CRT_PROCESS

#if BASE_CRT_MSVC
#	define popen  _popen
#	define pclose _pclose
#endif // BASE_CRT_MSVC

	ProcessReader::ProcessReader()
		: m_file(NULL)
	{
	}

	ProcessReader::~ProcessReader()
	{
		BASE_ASSERT(NULL == m_file, "Process not closed!");
	}

	bool ProcessReader::open(const FilePath& _filePath, const StringView& _args, Error* _err)
	{
		BASE_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

		if (NULL != m_file)
		{
			BASE_ERROR_SET(_err, kErrorReaderWriterAlreadyOpen, "ProcessReader: File is already open.");
			return false;
		}

		char tmp[kMaxFilePath*2] = "\"";
		strCat(tmp, BASE_COUNTOF(tmp), _filePath);
		strCat(tmp, BASE_COUNTOF(tmp), "\" ");
		strCat(tmp, BASE_COUNTOF(tmp), _args);

		m_file = popen(tmp, "r");
		if (NULL == m_file)
		{
			BASE_ERROR_SET(_err, kErrorReaderWriterOpen, "ProcessReader: Failed to open process.");
			return false;
		}

		return true;
	}

	void ProcessReader::close()
	{
		BASE_ASSERT(NULL != m_file, "Process not open!");
		FILE* file = (FILE*)m_file;
		m_exitCode = pclose(file);
		m_file = NULL;
	}

	int32_t ProcessReader::read(void* _data, int32_t _size, Error* _err)
	{
		BASE_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors."); BASE_UNUSED(_err);

		FILE* file = (FILE*)m_file;
		int32_t size = (int32_t)fread(_data, 1, _size, file);
		if (size != _size)
		{
			if (0 != feof(file) )
			{
				BASE_ERROR_SET(_err, kErrorReaderWriterEof, "ProcessReader: EOF.");
			}
			else if (0 != ferror(file) )
			{
				BASE_ERROR_SET(_err, kErrorReaderWriterRead, "ProcessReader: read error.");
			}

			return size >= 0 ? size : 0;
		}

		return size;
	}

	int32_t ProcessReader::getExitCode() const
	{
		return m_exitCode;
	}

	ProcessWriter::ProcessWriter()
		: m_file(NULL)
	{
	}

	ProcessWriter::~ProcessWriter()
	{
		BASE_ASSERT(NULL == m_file, "Process not closed!");
	}

	bool ProcessWriter::open(const FilePath& _filePath, const StringView& _args, Error* _err)
	{
		BASE_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

		if (NULL != m_file)
		{
			BASE_ERROR_SET(_err, kErrorReaderWriterAlreadyOpen, "ProcessWriter: File is already open.");
			return false;
		}

		char tmp[kMaxFilePath*2] = "\"";
		strCat(tmp, BASE_COUNTOF(tmp), _filePath);
		strCat(tmp, BASE_COUNTOF(tmp), "\" ");
		strCat(tmp, BASE_COUNTOF(tmp), _args);

		m_file = popen(tmp, "w");
		if (NULL == m_file)
		{
			BASE_ERROR_SET(_err, kErrorReaderWriterOpen, "ProcessWriter: Failed to open process.");
			return false;
		}

		return true;
	}

	void ProcessWriter::close()
	{
		BASE_ASSERT(NULL != m_file, "Process not open!");
		FILE* file = (FILE*)m_file;
		m_exitCode = pclose(file);
		m_file = NULL;
	}

	int32_t ProcessWriter::write(const void* _data, int32_t _size, Error* _err)
	{
		BASE_ASSERT(NULL != _err, "Reader/Writer interface calling functions must handle errors."); BASE_UNUSED(_err);

		FILE* file = (FILE*)m_file;
		int32_t size = (int32_t)fwrite(_data, 1, _size, file);
		if (size != _size)
		{
			if (0 != ferror(file) )
			{
				BASE_ERROR_SET(_err, kErrorReaderWriterWrite, "ProcessWriter: write error.");
			}

			return size >= 0 ? size : 0;
		}

		return size;
	}

	int32_t ProcessWriter::getExitCode() const
	{
		return m_exitCode;
	}
#endif // BASE_CONFIG_CRT_PROCESS

} // namespace base
