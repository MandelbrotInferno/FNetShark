#pragma once


#include "FNetShark/FileBinaryMetaData.hpp"
#include <string_view>
#include <winsock2.h>
#include <vector>
#include <unordered_map>

namespace FNetShark
{
	namespace Utils
	{
		void GenerateBinaryDataAndMetaDataOfAllServerFiles(std::string_view l_serverPath, const size_t l_maxAllowedBytesFiles,std::vector<char>& l_allFileData, std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData);

		std::string GeneratePathOfAllFilesInServer(const std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData);

		std::string GenerateNotFoundErrorMsg(const std::string& l_pathOfAllFilesInServer);

		void SendingFileToClientThread(SOCKET l_clientSocket, const std::string& l_pathOfAllFiles, const std::string& l_notFoundMsg, const std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData);
	}
}