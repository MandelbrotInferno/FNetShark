


#include "FNetShark/Server.hpp"
#include "FNetShark/Utils/UtilityFunctions.hpp"
#include <iostream>
#include <fmt/core.h>
#include <thread>

int main()
{
	FNetShark::Server lv_server("2805");
	constexpr size_t lv_maxTotalAllowedBytesOfAllFileInServer{128000000};

	std::vector<char> lv_allFilesData{};
	std::unordered_map<std::string, FNetShark::FileBinaryMetaData> lv_fileNamesToMetaData{};

	FNetShark::Utils::GenerateBinaryDataAndMetaDataOfAllServerFiles("C:/Users/farhan/Desktop/Server", lv_maxTotalAllowedBytesOfAllFileInServer,lv_allFilesData, lv_fileNamesToMetaData);

	const std::string lv_pathOfAllFiles = FNetShark::Utils::GeneratePathOfAllFilesInServer(lv_fileNamesToMetaData);
	const std::string lv_notFoundMsg = FNetShark::Utils::GenerateNotFoundErrorMsg(lv_pathOfAllFiles);

	std::vector<std::thread> lv_threads{};
	lv_threads.reserve(32000);

	try {

		lv_server.Init();
		lv_server.Bind();
		lv_server.Listen();

		while (true) {
			
			SOCKET lv_clientSocket = lv_server.Accept();
			
			lv_threads.emplace_back(FNetShark::Utils::SendingFileToClientThread, lv_clientSocket, std::ref(lv_pathOfAllFiles), std::ref(lv_notFoundMsg), std::ref(lv_fileNamesToMetaData));
			
		}

		for (auto& l_thread : lv_threads) {
			l_thread.join();
		}
	}
	catch (const char* l_error) {
		fmt::print("{}", l_error);
	}


	try {
		lv_server.CleanUp();
	}
	catch (const char* l_error) {
		fmt::print("{}", l_error);
	}
	return 0;
}