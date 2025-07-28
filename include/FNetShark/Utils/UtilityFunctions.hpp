#pragma once


#include "FNetShark/FileBinaryMetaData.hpp"
#include <string_view>
#include <vector>
#include <unordered_map>

namespace FNetShark
{
	namespace Utils
	{
		void GenerateBinaryDataAndMetaDataOfAllServerFiles(std::string_view l_serverPath, const size_t l_maxAllowedBytesFiles,std::vector<char>& l_allFileData, std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData);
	}
}