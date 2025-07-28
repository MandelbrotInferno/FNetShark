#pragma once


#include <string>


namespace FNetShark
{
	struct FileBinaryMetaData final
	{
		size_t m_totalSizeFile{};
		char* m_data{};
	};
}