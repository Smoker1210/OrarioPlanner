#ifndef UTILITY_H
#define UTILITY_H
#include <string>

namespace shiftplanner{
    namespace utility{
        std::string cleanString(const std::string&);
        unsigned int stringOraToMinuti(const std::string&);
        std::tuple<unsigned int, unsigned int, bool> estraiOrari(const std::string& bloccoStr);
        std::string minutiToOraString(unsigned int);
        void lowerFirstLetter(std::string& stringa);
        void upperFirstLetter(std::string& stringa);
        std::string extractNameFromKey(const std::string& key);
        std::string extractSurnameFromKey(const std::string& key);
        std::string intToGiorno(int giorno);
    }
}

#endif