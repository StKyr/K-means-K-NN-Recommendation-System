#include "../include/Lexicon.hpp"

CryptoLexicon::CryptoLexicon(std::string fileName, char del){
    std::ifstream fin;
    std::string line, abbr, crypto, cryptoName;

    int i=0;
    fin.open(fileName);
    if (fin.is_open()){

        while(getline(fin, line)){
            std::stringstream ss(line);

            int column=0;

            while(getline(ss, abbr, del)){
                column++;

                abbr.erase(std::remove(abbr.begin(), abbr.end(),'\r'), abbr.end());
                abbr.erase(std::remove(abbr.begin(), abbr.end(),'\n'), abbr.end());
                abbr.erase(std::remove(abbr.begin(), abbr.end(),'\t'), abbr.end());
                abbr.erase(std::remove(abbr.begin(), abbr.end(),' '), abbr.end());
                this->abbreviations[abbr] = i;

                if (column == 1 || column == 5) cryptoName = abbr;

            }
            this->cryptos.push_back(cryptoName);
            i++;
        }
    }else{
        throw std::runtime_error(fileName+" not found.");
    }
    fin.close();
}

std::vector<double> CryptoLexicon::createCryptoVector(std::vector<std::string>& tweet){
    std::vector<double> cryptoVector(this->cryptos.size(), 0);
    for (auto& token: tweet){
        if (this->abbreviations.find(token) != this->abbreviations.end()){
            //std::cerr<<"Found crypto abbreviation: "<<token<<" ("<<this->abbreviations[token]<<"-"<<this->cryptos[abbreviations[token]]<<std::endl;
            int cryptoIndex = this->abbreviations[token];
            cryptoVector.at(cryptoIndex) = 1;
        }
    }
    return cryptoVector;
}

std::vector<bool> CryptoLexicon::createBoolVector(std::vector<std::string>& tweet){
    std::vector<bool> cryptoVector(this->cryptos.size(), false);
    for (auto& token: tweet){
        if (this->abbreviations.find(token) != this->abbreviations.end()){
            //std::cerr<<"Found crypto abbreviation: "<<token<<" ("<<this->abbreviations[token]<<"-"<<this->cryptos[abbreviations[token]]<<std::endl;
            int cryptoIndex = this->abbreviations[token];
            cryptoVector.at(cryptoIndex) = true;
        }
    }
    return cryptoVector;
}

bool CryptoLexicon::exists(const std::string word){return abbreviations.find(word)!=abbreviations.end();}
bool CryptoLexicon::exists(const char * word){return this->exists(std::string(word));}
std::string CryptoLexicon::getCrypto(int index){
    return this->cryptos[index];
}

void CryptoLexicon::print(){
    std::cerr<<"~~~Crypto Lexicon~~~"<<std::endl;
    int cnt=1;
    for (auto it: this->abbreviations) std::cerr<<cnt++<<" :"<<it.first<<std::endl;
}