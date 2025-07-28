


#include "FNetShark/Server.hpp"
#include "FNetShark/Utils/UtilityFunctions.hpp"
#include <iostream>
#include <fmt/core.h>

int main()
{
	//FNetShark::Server lv_server;
	constexpr size_t lv_maxTotalAllowedBytesOfAllFileInServer{128000000};
	std::vector<char> lv_allFilesData{};
	std::unordered_map<std::string, FNetShark::FileBinaryMetaData> lv_fileNamesToMetaData{};

	FNetShark::Utils::GenerateBinaryDataAndMetaDataOfAllServerFiles("C:/Users/farhan/Desktop/Server", lv_maxTotalAllowedBytesOfAllFileInServer,lv_allFilesData, lv_fileNamesToMetaData);

	for (const auto& l_metaData : lv_fileNamesToMetaData) {
		std::cout << l_metaData.first << std::endl;
	}

	try {
		//lv_server.Init();
	}
	catch (const char* l_error) {
		fmt::print("{}", l_error);
	}


	try {
		//lv_server.InitCleanUp();
	}
	catch (const char* l_error) {
		fmt::print("{}", l_error);
	}
	return 0;
}