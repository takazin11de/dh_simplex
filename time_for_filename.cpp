#include "time_for_filename.hpp"

void TimeForFileName(std::string &str){
	std::locale default_locale;
	std::locale::global(std::locale("ja_JP.utf8"));
    std::time_t t = std::time(NULL);
    char mbstr[256];
    if (std::strftime(mbstr, sizeof(mbstr), "%Y_%m_%d_%H_%M", std::localtime(&t))) {
        str=mbstr;
    }
    std::locale::global(default_locale);//必ずデフォルトロケールにする．
    std::cout << default_locale.name() << std::endl;
}