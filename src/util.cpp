#include "util.hpp"
#include <qdebug.h>
#include <string>
#include <filesystem>
#include <qstandardpaths.h>

#if defined(__WIN32) || defined(__WIN64)
#define SLASH '\\'
#else
#define SLASH '/'
#endif


std::string getDbPath() {
    return getDataDir() + SLASH + "tasktree.db";
}

std::string getDataDir() {
	return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString();
}
