#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <map>


struct Dolgozat {
    std::string filenev;
    std::vector<std::string> parok;

    Dolgozat(std::string filenev) : filenev(filenev) {
        std::ifstream is(filenev);
        std::string uj, elozo;
        is >> elozo;
        std::set<std::string> halmaz;
        while (is >> uj) {
            if (isspace(uj.front()))
                uj.erase(uj.begin());
            halmaz.insert(elozo + " " + uj);
            elozo = uj;
        }
        parok = std::vector<std::string>(halmaz.begin(), halmaz.end());
    }
};

std::vector<Dolgozat> dolgozatok_beolvas(std::string input_filenev) {
    std::vector<Dolgozat> dolgozatok;
    std::ifstream is(input_filenev);
    std::string filenev;
    while (is >> filenev)
        dolgozatok.push_back(Dolgozat(filenev));
    return dolgozatok;
}

std::map<std::string, int> elofordulast_szamol(std::vector<Dolgozat> const& dolgozatok) {
    std::map<std::string, int> stringek;
    for (size_t i = 0; i < dolgozatok.size(); ++i) {
        for (size_t j = 0; j < dolgozatok[i].parok.size(); ++j) {
            auto kereses = stringek.find(dolgozatok[i].parok[j]);
            if (kereses != stringek.end())
                kereses->second++;
            else;
                stringek.insert(std::pair<std::string, int>(dolgozatok[i].parok[j], 1));
        }
    }
    return stringek;
};

bool osszeilleszt(std::vector<std::pair<std::string, int>>& stringek, std::string& mondat, int elofordulas) {
    std::string vege = mondat.substr(mondat.find_last_of(' ') + 1); //mondat utolso szava space nelkul
    int hanyadszor = 1;
    while ((vege.back()!= '.' || vege.back() != '!' || vege.back() != '?' || vege.back() != ',') && hanyadszor <= 5) {
        for (size_t i = 0; i < stringek.size(); ++i)
        {
            std::string keres = stringek[i].first.substr(0, stringek[i].first.find(' ')); //szoparok vectorjaban az i-edik szopar elso szava
            if (stringek[i].second != 0 && vege == keres && abs(stringek[i].second - elofordulas) < hanyadszor * 5) {   //ha megegyeznek es az elofordulaok kulonbsege egy bizonos szam alatt van, ami minden ciklusban no
          //      std::cout << "--------------" << mondat << "____" << vege << "____" << stringek[i].first.substr(stringek[i].first.find(' ')) << std::endl;
                mondat += stringek[i].first.substr(stringek[i].first.find(' '));
                stringek[i].second--;
                vege = mondat.substr(mondat.find_last_of(' ') + 1);
            }
        }
        hanyadszor++;
    }
    return mondat.back() == '.' || mondat.back() == '!' || mondat.back() == '?';
}


int main() {
    std::vector<Dolgozat> dolgozatok = dolgozatok_beolvas("inputm.ctr");
    std::cout << dolgozatok.size() << std::endl;

//kitorli a tul kis elofordulasu szoparokat
    std::map<std::string, int> stringek = elofordulast_szamol(dolgozatok);
    for (auto iter = stringek.begin(); iter != stringek.end(); ++iter)
        if (iter->second <= 5)
            stringek.erase(iter);

//atmasolom vektorba, mert tul sokszor iteralok rajta vegig az elkovetkezokben
    std::vector<std::pair<std::string, int>> szoparok(stringek.begin(), stringek.end());
//    std::cout << szoparok.size() << std::endl;
//    for (auto i = szoparok.begin(); i != szoparok.end(); ++i)
//        std::cout << i->first << std::endl;

    std::string mondat;
    int elofordulas;
    for (size_t i = 0; i < szoparok.size(); ++i) {
        if (isupper(szoparok[i].first.front())) {
            mondat = szoparok[i].first;
            elofordulas = szoparok[i].second;
            if (osszeilleszt(szoparok, mondat, elofordulas) && std::count(mondat.begin(), mondat.end(), ' ') >= 3)
                std::cout << mondat << std::endl;
//            if (mondat.length() >= 20)
//                std::cout << "____________________" << mondat << std::endl;
        }

    }
    for (size_t i = 0; i < szoparok.size(); ++i) {
        std::cout << szoparok[i].second << " ";
    }

    std::cout << mondat;
}
