#include "util.hpp"
#include <string>
#include <filesystem>
#include <qstandardpaths.h>
#include <iostream>

#if defined(__WIN32) || defined(__WIN64)
#define SLASH '\\'
#else
#define SLASH '/'
#endif


const std::string DATA_DIR = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toStdString() + SLASH + "tasktree";

std::string getDbPath() {
    return DATA_DIR + SLASH + "tasktree.db";
}

std::string getDataDir() {
	std::string data_dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toStdString() + SLASH + "tasktree";
	std::filesystem::create_directories(data_dir);
	return data_dir;
}
