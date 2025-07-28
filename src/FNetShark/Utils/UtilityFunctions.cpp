


#include "FNetShark/Utils/UtilityFunctions.hpp"
#include <fmt/core.h>
#include <cstdio>
#include <filesystem>

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
	}
}