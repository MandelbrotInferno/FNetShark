


#include "FNetShark/Utils/UtilityFunctions.hpp"
#include <fmt/core.h>
#include <cstdio>
#include <filesystem>
#include <array>

namespace FNetShark
{
	namespace Utils
	{
		void GenerateBinaryDataAndMetaDataOfAllServerFiles(std::string_view l_serverPath, const size_t l_maxAllowedBytesFiles, std::vector<char>& l_allFileData, std::unordered_map<std::string, FileBinaryMetaData>& l_fileNamesToMetaData)
		{
			size_t lv_finalSize{};
			size_t lv_totalNumMetaData{};

			std::filesystem::directory_entry lv_dir{ l_serverPath };

			if (true == lv_dir.exists() && true == lv_dir.is_directory()) {
				for (const auto& l_entry : std::filesystem::recursive_directory_iterator(l_serverPath)) {
					if (true == l_entry.is_regular_file()) {
						lv_finalSize += l_entry.file_size();
						++lv_totalNumMetaData;
					}
				}
			}
			else {
				throw "The server path is not a directory!\n";
			}

			if (l_maxAllowedBytesFiles < lv_finalSize) {
				fmt::print("The total number of bytes in all server directories is {} which exceeds the max limit.\n", lv_finalSize);
				throw "Cannot initialize the server due to too much data in server directory.\n";
			}

			l_allFileData.resize(lv_finalSize);
			l_fileNamesToMetaData.reserve(lv_totalNumMetaData);

			size_t lv_currentIndex{};
			for (const auto& l_entry : std::filesystem::recursive_directory_iterator(l_serverPath)) {
				if (true == l_entry.is_regular_file()) {

					const auto lv_filePath = l_entry.path().string();
					FILE* lv_file = fopen(lv_filePath.c_str(), "r");

					if (nullptr == lv_file) {
						fmt::print("Failed to open file : {}\n", lv_filePath);
						throw "Serious failure occured!\n";
					}

					const size_t lv_fileSize = l_entry.file_size();
					char* lv_currentData = &(l_allFileData.data()[lv_currentIndex]);
					const size_t lv_bytesRead = fread(lv_currentData, 1, lv_fileSize, lv_file);

					if (lv_bytesRead < lv_fileSize) {
						fmt::print("fread() failed to read the whole file {}\n", lv_filePath);
						throw "Serious failure occured!\n";
					}
					
					if (0 != fclose(lv_file)) {
						fmt::print("fclose() failed for file {}\n", lv_filePath);
					}

					const std::string lv_localServerPath = lv_filePath.substr(lv_filePath.rfind("Server"));

					FileBinaryMetaData lv_metaData{ .m_totalSizeFile = lv_fileSize, .m_data = lv_currentData};

					l_fileNamesToMetaData.insert(std::make_pair(lv_localServerPath, lv_metaData));

					lv_currentIndex += lv_fileSize;
				}
			}
		}

		std::string GeneratePathOfAllFilesInServer(const std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData)
		{
			std::string lv_pathOfAllFiles{};

			lv_pathOfAllFiles = lv_pathOfAllFiles + "Server has the following files:\n";
			for (const auto& l_meta : l_allFilesMetaData) {
				lv_pathOfAllFiles = lv_pathOfAllFiles + l_meta.first + "\n";
			}

			return lv_pathOfAllFiles;
		}

		std::string GenerateNotFoundErrorMsg(const std::string& l_pathOfAllFilesInServer)
		{
			std::string lv_notFoundMsg{};

			lv_notFoundMsg = "The requested file does not exist. Choose another file from the following files:\n\n" + l_pathOfAllFilesInServer;

			return lv_notFoundMsg;
		}


		void SendingFileToClientThread(SOCKET l_clientSocket, const std::string& l_pathOfAllFiles, const std::string& l_notFoundMsg, const std::unordered_map<std::string, FileBinaryMetaData>& l_allFilesMetaData)
		{
			int lv_result = send(l_clientSocket, l_pathOfAllFiles.data(), (int)l_pathOfAllFiles.size(), 0);

			
			if (0 == lv_result) {
				fmt::print("Closing connection. send() sent 0 byte.\n");
				if (0 != shutdown(l_clientSocket, SD_BOTH)) {
					fmt::print("shutdown() failed : {}\n", WSAGetLastError());
				}
				if (0 != closesocket(l_clientSocket)) {
					fmt::print("closesocket() failed: {}\n", WSAGetLastError());
				}
				return;
			}
			else if (lv_result < 0) {
				fmt::print("send() failed : {}", WSAGetLastError());
				if (0 != shutdown(l_clientSocket, SD_BOTH)) {
					fmt::print("shutdown() failed : {}\n", WSAGetLastError());
				}
				if (0 != closesocket(l_clientSocket)) {
					fmt::print("closesocket() failed: {}\n", WSAGetLastError());
				}
				return;
			}


			while (0 < lv_result) {

				constexpr size_t lv_sizeRecvBuffer = 2048;
				std::array<char, lv_sizeRecvBuffer> lv_recvBuffer{};

				lv_result = recv(l_clientSocket, lv_recvBuffer.data(), (int)lv_recvBuffer.size(), 0);
				if (0 == lv_result) {
					fmt::print("Closing connection. Received 0 bytes....\n");
					if (0 != shutdown(l_clientSocket, SD_BOTH)) {
						fmt::print("shutdown() failed : {}\n", WSAGetLastError());
					}
					if (0 != closesocket(l_clientSocket)) {
						fmt::print("closesocket() failed: {}\n", WSAGetLastError());
					}
					return;
				}
				else if (lv_result < 0) {
					fmt::print("recv() failed : {}", WSAGetLastError());
					if (0 != shutdown(l_clientSocket, SD_BOTH)) {
						fmt::print("shutdown() failed : {}\n", WSAGetLastError());
					}
					if (0 != closesocket(l_clientSocket)) {
						fmt::print("closesocket() failed: {}\n", WSAGetLastError());
					}
					return;
				}

				const std::string lv_requestedFileName = lv_recvBuffer.data();

				auto lv_fileIter = l_allFilesMetaData.find(lv_requestedFileName);

				if (l_allFilesMetaData.cend() == lv_fileIter) {
					lv_result = send(l_clientSocket, l_notFoundMsg.data(), (int)l_notFoundMsg.size(), 0);

					if (0 == lv_result) {
						fmt::print("Closing connection. send() sent 0 bytes.\n");
						if (0 != shutdown(l_clientSocket, SD_BOTH)) {
							fmt::print("shutdown() failed : {}\n", WSAGetLastError());
						}
						if (0 != closesocket(l_clientSocket)) {
							fmt::print("closesocket() failed: {}\n", WSAGetLastError());
						}
						return;
					}
					else if (lv_result < 0) {
						fmt::print("send() failed : {}", WSAGetLastError());
						if (0 != shutdown(l_clientSocket, SD_BOTH)) {
							fmt::print("shutdown() failed : {}\n", WSAGetLastError());
						}
						if (0 != closesocket(l_clientSocket)) {
							fmt::print("closesocket() failed: {}\n", WSAGetLastError());
						}
						return;
					}

				}
				else {
					int lv_totalBytesSent{};
					while (lv_totalBytesSent < (int)lv_fileIter->second.m_totalSizeFile) {
						
						lv_result = send(l_clientSocket, &lv_fileIter->second.m_data[lv_totalBytesSent], (int)lv_fileIter->second.m_totalSizeFile - lv_totalBytesSent, 0);

						if (lv_result < 0) {
							fmt::print("send() failed : {}", WSAGetLastError());
							if (0 != shutdown(l_clientSocket, SD_BOTH)) {
								fmt::print("shutdown() failed : {}\n", WSAGetLastError());
							}
							if (0 != closesocket(l_clientSocket)) {
								fmt::print("closesocket() failed: {}\n", WSAGetLastError());
							}
							return;
						}

						lv_totalBytesSent += lv_result;
					}
					lv_result = 0;
				}
			}

			if (0 != shutdown(l_clientSocket, SD_BOTH)) {
				fmt::print("shutdown() failed : {}\n", WSAGetLastError());
			}
			if (0 != closesocket(l_clientSocket)) {
				fmt::print("closesocket() failed: {}\n", WSAGetLastError());
			}
		}

	}
}