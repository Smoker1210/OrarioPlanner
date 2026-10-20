#include "utility.h"
#include <sstream>
#include <tuple>

namespace shiftplanner{
    namespace utility{
        std::string cleanString(const std::string& input){
            std::string result;
            result.reserve(input.size());
            bool spazioPrec = false;
            for (char c : input){
                unsigned char uc = static_cast<unsigned char> (c);
                char maiuscolo = std::toupper(uc);
                if (std::isspace(uc)){
                    if(!spazioPrec){
                        result += ' ';
                        spazioPrec = true;
                    }
                }
                else{
                        result += maiuscolo;
                        spazioPrec = false;
                }
            }
            if(!result.empty() && result.front() == ' '){
                result.erase(result.begin());
            }
            if(!result.empty() && result.back() == ' '){
                result.pop_back();
            }
            return result;
        }
        unsigned int stringOraToMinuti(const std::string& str){
            int ore = 0;
            int minuti = 0;
            size_t sep = str.find_first_of(".:");
            try{
            if (sep != std::string::npos){
                std::string parteOre = str.substr(0,sep);
                std::string parteMinuti = str.substr(sep+1);
                size_t idxOre = 0;
                size_t idxMin = 0;
                ore = parteOre.empty()? 0 : std::stoi(parteOre, &idxOre);
                minuti = parteMinuti.empty() ? 0 : std::stoi(parteMinuti, &idxMin);
                //questa parte di controllo viene aggiunta dopo che il parser accettava come stringa una cosa come 2 23R23R23R
                //risolvo controllando che effettivamente tutta la stringa sia stata controllata visto che stoi nel caso sopra prima della modifica si fermava ad R e riteneva valido 2 23
                if(!parteOre.empty() && idxOre != parteOre.length()) return 0;
                if(!parteMinuti.empty() && idxMin != parteMinuti.length()) return 0;
            } else{
                size_t idx = 0;
                ore = std::stoi(str, &idx);
                if(idx !=str.length()){
                    return 0;
                }
            }
            }
            catch (const std::invalid_argument&){
                return 0;
            }
            catch (const std::out_of_range&){
                return 0;
            }
            if(ore<0 || ore >24) ore = 0;
            if(minuti<0 || minuti > 59) minuti = 0;
            return (ore*60) + minuti;
        }
        std::tuple<unsigned int, unsigned int, bool> estraiOrari(const std::string& bloccoStr){
            std::stringstream ss(bloccoStr);
            std::string inizioStr, fineStr;
            if(ss >> inizioStr >> fineStr){
                //controlliamo che non ci siano caratteri garbage in più nella stringa
                std::string tokenExtra;
                if(ss>>tokenExtra){
                    return {0,0, false};
                }
                unsigned int inizioMin = stringOraToMinuti(inizioStr);
                unsigned int fineMin = stringOraToMinuti(fineStr);
                bool inizioOK = (inizioMin != 0 || inizioStr == "0" || inizioStr == "00" || inizioStr == "0:00" || inizioStr == "00:00");
                bool fineOK   = (fineMin != 0   || fineStr == "0"   || fineStr == "00"   || fineStr == "0:00"   || fineStr == "00:00");
                if(inizioOK && fineOK && (fineMin>inizioMin)){
                    return {inizioMin, fineMin, true};
                }
            }
            return {0,0, false};
        }
        void cleanBloccoOrari(std::string& orario){
            std::stringstream ss(orario);
            std::string blocco, risultato;

            while (ss >> orario) {
                cleanBlocco(blocco);
                if (!risultato.empty()) risultato += " ";
                risultato += orario;
            }

            blocco = risultato;
        }
        void cleanBlocco(std::string& blocco){
            try{
                size_t sep = blocco.find_first_of(".:");
                if (sep != std::string::npos) {
                    std::string parteMinuti = blocco.substr(sep + 1);
                    if (parteMinuti.empty() || parteMinuti == "0" || parteMinuti == "00") {
                        blocco = blocco.substr(0, sep); // Tiene solo la parte delle ore
                    }
                }
            }
            catch (const std::invalid_argument&){
                return;
            }
            catch (const std::out_of_range&){
                return;
            }
        }
        std::string minutiToOraString(unsigned int minutiTotali){
            int ore = minutiTotali/60;
            int minuti = minutiTotali%60;
            char buffer[16];
            snprintf(buffer, sizeof(buffer), "%02d:%02d", ore, minuti);
            return std::string(buffer);
        }
        void lowerFirstLetter(std::string& stringa){
            char b = stringa.front();
            b = std::tolower(b);
            stringa[0] = b;
        }
        void upperFirstLetter(std::string& stringa){
            char b = stringa.front();
            b = std::toupper(b);
            stringa[0] = b;
        }
        std::string extractNameFromKey(const std::string& key){
            size_t underscore = key.find("_");
            std::string result = "";
            if(underscore == std::string::npos){
                return result;
            }
            else{
                result = key.substr(0,underscore);
                upperFirstLetter(result);
                return result;
            }
            return result;
        }
        std::string extractSurnameFromKey(const std::string& key){
            size_t underscore = key.find("_");
            std::string result = "";
            if(underscore == std::string::npos){
                return result;
            }
            else{
                result = key.substr(underscore+1);
                upperFirstLetter(result);
                return result;
            }
            return result;
        }
        std::string intToGiorno(int giorno){
            switch (giorno){
            case 0:
                return "Lunedì";
            case 1:
                return "Martedì";
            case 2:
                return "Mercoledì";
            case 3:
                return "Giovedì";
            case 4:
                return "Venerdì";
            case 5:
                return "Sabato";
            case 6:
                return "Domenica";
            default:
                return "";
            }
        }
    }
}