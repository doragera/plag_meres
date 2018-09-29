#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <iterator>
#include <functional>

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

class KamuIter {
private:
    int & cnt;
public:
    KamuIter(int & cnt) : cnt(cnt) {}
    KamuIter & operator++() {
        ++cnt;
        return *this;
    }
    auto operator*() const {
        return std::ignore;
    }
};

struct Hasonlosag {
    Dolgozat *d1, *d2;
    double mertek;
    Hasonlosag(Dolgozat * d1, Dolgozat * d2) : d1(d1), d2(d2) {
        int cnt = 0;
        std::set_intersection(d1->parok.begin(), d1->parok.end(),
                              d2->parok.begin(), d2->parok.end(),
                              KamuIter(cnt));
        mertek = std::max(
                (double)cnt / (double)d1->parok.size(),
                (double)cnt / (double)d2->parok.size());
    }
    bool operator< (Hasonlosag const & masik) const {
        return this->mertek < masik.mertek;
    }
};

std::vector<Hasonlosag> hasonlit(std::vector<Dolgozat> & dolgozatok) {
    std::vector<Hasonlosag> hasonlosagok;
    hasonlosagok.reserve(dolgozatok.size()*(dolgozatok.size()-1)/2);
    for (size_t i = 0; i != dolgozatok.size(); ++i)
        for (size_t j = i+1; j != dolgozatok.size(); ++j)
            hasonlosagok.push_back(Hasonlosag(&dolgozatok[i], &dolgozatok[j]));
    return hasonlosagok;
}


std::map<std::string, int> elofordulast_szamol(std::vector<Dolgozat*> const& masolok) {
    std::map<std::string, int> stringek;
    for (size_t i = 0; i < masolok.size(); ++i) {
        for (size_t j = 0; j < masolok[i]->parok.size(); ++j) {
            auto kereses = stringek.find(masolok[i]->parok[j]);
            if (kereses != stringek.end())
                kereses->second++;
            else
                stringek.insert(std::pair<std::string, int>(masolok[i]->parok[j], 1));
        }
    }
    return stringek;
};

bool osszeilleszt(std::vector<std::pair<std::string, int>>& stringek, std::string& mondat, int elofordulas) {
    std::string vege = mondat.substr(mondat.find_last_of(' ') + 1);
    int hanyadszor = 1;
    while ((vege.back()!= '.' || vege.back() != '!' || vege.back() != '?' || vege.back() != ',') && hanyadszor <= 10) {

        for (size_t i = 0; i < stringek.size(); ++i)
        {
            std::string keres = stringek[i].first.substr(0, stringek[i].first.find(' '));
            if (stringek[i].second != 0 && vege == keres && abs(stringek[i].second - elofordulas) < hanyadszor * 5) {
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
    std::vector<Hasonlosag> hasonlosagok = hasonlit(dolgozatok);
    std::sort(hasonlosagok.rbegin(), hasonlosagok.rend());  /* reverse iterator! */

    std::vector<Hasonlosag>::iterator iter, eleje, vege;
    bool megvan = false;
    for (iter = hasonlosagok.begin(); iter != hasonlosagok.end(); ++iter)
    {
        if (!megvan && (*iter).mertek < 0.30 )  {
            eleje = iter;
            megvan = true;
        }

        if ((*iter).mertek < 0.13) {
            vege = iter;
            break;
        }

    }

    std::vector<Hasonlosag> masolok(hasonlosagok.begin(), hasonlosagok.end());

    std::set<Dolgozat*> masolok_dolgozatai_halmaz;
    for (size_t i = 0; i < masolok.size(); ++i) {
        masolok_dolgozatai_halmaz.insert(masolok[i].d1);
        masolok_dolgozatai_halmaz.insert(masolok[i].d2);
    }
    std::vector<Dolgozat*> masolok_dolgozatai(masolok_dolgozatai_halmaz.begin(), masolok_dolgozatai_halmaz.end());
    std::cout << masolok_dolgozatai.size() << std::endl;

    std::map<std::string, int> stringek = elofordulast_szamol(masolok_dolgozatai);
    for (auto iter = stringek.begin(); iter != stringek.end(); ++iter) {
        if (iter->second <= 10)
            stringek.erase(iter);
    }

    std::vector<std::pair<std::string, int>> szoparok(stringek.begin(), stringek.end());
    std::cout << szoparok.size() << std::endl;
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
    std::ofstream output;
    output.open("output.txt");
    for (size_t i = 0; i < hasonlosagok.size(); ++i) {
  //      std::cout << masolok[i].mertek*100 << std::endl;
        output << hasonlosagok[i].mertek*100
               << " " << hasonlosagok[i].d1->filenev
               << " " << hasonlosagok[i].d2->filenev << std::endl;
    }

}
