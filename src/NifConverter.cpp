#include "lib/nif/nif_converter.h"






int main(int argc, char* const argv[], char* const envp[]) {

	
	if (argc != 3) {
		std::cout << "Please provide input and output folders." << std::endl;
		return 1;
	}

	char* inFolder = argv[1];
	char* outFolder = argv[2];
	
	/*
	char inFolder[] = "C:/Users/User/YandexDisk/dev/NifConverter/Debug/_in";
	char outFolder[] = "C:/Users/User/YandexDisk/dev/NifConverter/Debug/_out";
	*/

	if (strcmp(inFolder, outFolder) == 0) {
		std::cout << "Input and output folders can not be same." << std::endl;
		return 1;
	}

	if (!std::experimental::filesystem::exists(inFolder) || !std::experimental::filesystem::exists(outFolder)) {
		std::cout << "Can not find input/output folder(s)." << std::endl;
		return 1;
	}

	std::fstream fTxt("missing_objects.txt", std::ios::out);
	std::unordered_map<std::string, std::string> names;

	nif::processFolders(inFolder, outFolder, names);

	for (auto& nm : names) {
		fTxt << nm.first << " : " << nm.second << std::endl;
	}
	fTxt.close();

    return 0; 
}