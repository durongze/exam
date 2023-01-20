#include <iostream>
#include <string>
#include <map>

int GenTenNumber(std::map<int, std::string> &num_map) {
        num_map[0]="оди́н   ";
        num_map[1]="два    ";
        num_map[2]="три    ";
        num_map[3]="четы́ре ";
        num_map[4]="пять   ";
        num_map[5]="шесть  ";
        num_map[6]="семь   ";
        num_map[7]="во́семь ";
        num_map[8]="де́вять ";
        num_map[9]="де́сять ";
        return 0;
}

int GenTwiNumber(std::map<int, std::string> &num_map) {
        num_map[0]="оди́ннадцать  ";
        num_map[1]="двена́дцать   ";
        num_map[2]="трина́дцать   ";
        num_map[3]="четы́рнадцать ";
        num_map[4]="пятна́дцать   ";
        num_map[5]="шестна́дцать  ";
        num_map[6]="семна́дцать   ";
        num_map[7]="восемна́дцать ";
        num_map[8]="девятна́дцать ";
        num_map[9]="два́дцать     ";
        return 0;
}

int GenThiNumber(std::map<int, std::string> &num_map) {
        num_map[0]="два́дцать оди́н   ";
        num_map[1]="два́дцать два    ";
        num_map[2]="два́дцать три    ";
        num_map[3]="два́дцать четы́ре ";
        num_map[4]="два́дцать пять   ";
        num_map[5]="два́дцать шесть  ";
        num_map[6]="два́дцать семь   ";
        num_map[7]="два́дцать во́семь ";
        num_map[8]="два́дцать де́вять ";
        num_map[9]="три́дцать        ";
        return 0;
}

int GenFourNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="три́дцать оди́н   ";
        num_map[1]="три́дцать два    ";
        num_map[2]="три́дцать три    ";
        num_map[3]="три́дцать четы́ре ";
        num_map[4]="три́дцать пять   ";
        num_map[5]="три́дцать шесть  ";
        num_map[6]="три́дцать семь   ";
        num_map[7]="три́дцать во́семь ";
        num_map[8]="три́дцать де́вять ";
        num_map[9]="со́рок           ";
        return 0;
}

int GenFiveNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="со́рок оди́н   ";
        num_map[1]="со́рок два    ";
        num_map[2]="со́рок три    ";
        num_map[3]="со́рок четы́ре ";
        num_map[4]="со́рок пять   ";
        num_map[5]="со́рок шесть  ";
        num_map[6]="со́рок семь   ";
        num_map[7]="со́рок во́семь ";
        num_map[8]="со́рок де́вять ";
        num_map[9]="пятьдеся́т    ";
        return 0;
}

int GenSixNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="пятьдеся́т оди́н   ";
        num_map[1]="пятьдеся́т два    ";
        num_map[2]="пятьдеся́т три    ";
        num_map[3]="пятьдеся́т четы́ре ";
        num_map[4]="пятьдеся́т пять   ";
        num_map[5]="пятьдеся́т шесть  ";
        num_map[6]="пятьдеся́т семь   ";
        num_map[7]="пятьдеся́т во́семь ";
        num_map[8]="пятьдеся́т де́вять ";
        num_map[9]="шестьдеся́т       ";
        return 0;
}

int GenSevenNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="шестьдеся́т пе́рвый  ";
        num_map[1]="Шестьдеся́т два     ";
        num_map[2]="Шестьдеся́т три     ";
        num_map[3]="Шестьдеся́т четы́ре  ";
        num_map[4]="Шестьдеся́т пять    ";
        num_map[5]="шесть шестна́дцати  ";
        num_map[6]="шесть семна́дцати   ";
        num_map[7]="шесть восемна́дцати ";
        num_map[8]="Шестьдеся́т де́вять  ";
        num_map[9]="се́мьдесят          ";
        return 0;
}

int GenEightNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="се́мьдесят оди́н    ";
        num_map[1]="се́мьдесят два     ";
        num_map[2]="Се́мьдесят три     ";
        num_map[3]="се́мьдесят четы́ре  ";
        num_map[4]="се́мьдесят пять    ";
        num_map[5]="семь шестна́дцати  ";
        num_map[6]="семь семна́дцати   ";
        num_map[7]="семь восемна́дцати ";
        num_map[8]="се́мьдесят де́вять  ";
        num_map[9]="во́семьдесят       ";
        return 0;
}

int GenNineNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="во́семьдесят оди́н   ";
        num_map[1]="во́семьдесят два    ";
        num_map[2]="во́семьдесят три    ";
        num_map[3]="во́семьдесят четы́ре ";
        num_map[4]="Во́семьдесят пять   ";
        num_map[5]="Во́семьдесят шесть  ";
        num_map[6]="Во́семьдесят семь   ";
        num_map[7]="во́семьдесят во́семь ";
        num_map[8]="во́семьдесят де́вять ";
        num_map[9]="Девяно́сто          ";
        return 0;
}

int GenHanNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="Девяно́сто оди́н          ";
        num_map[1]="Девяно́сто два           ";
        num_map[2]="Девяно́сто три           ";
        num_map[3]="девяно́сто четы́ре        ";
        num_map[4]="девяно́сто пять          ";
        num_map[5]="Девяно́сто шесть         ";
        num_map[6]="Девяно́сто семь          ";
        num_map[7]="девяно́сто восемна́дцатый ";
        num_map[8]="Девяно́сто де́вять        ";
        num_map[9]="сто                     ";
        return 0;
}

int GenHanMoreNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="сто       ";
        num_map[1]="две́сти    ";
        num_map[2]="три́ста    ";
        num_map[3]="четы́реста ";
        num_map[4]="пятьсо́т   ";
        num_map[5]="шестьсо́т  ";
        num_map[6]="семьсо́т   ";
        num_map[7]="восемьсо́т ";
        num_map[8]="девятьсо́т ";
        num_map[9]="ты́сяча    ";
        return 0;
}

int GenMoreNumber(std::map<int, std::string> &num_map)
{
        num_map[0]="сто оди́н            ";
        num_map[1]="сто два             ";
    num_map[2]="две́сти два́дцать три ";
        num_map[3]="ты́сяча              ";
        num_map[4]="две ты́сячи          ";
        num_map[5]="три ты́сячи          ";
        num_map[6]="де́сять ты́сяч        ";
        return 0;
}

void DumpNumMap(std::map<int, std::string> &num_map, std::ostream &os = std::cout)
{
        for (auto &p : num_map) {
                os << p.first + 1 << " -> " << p.second << std::endl;
        }
}

int main(int argc, char** argv)
{
        std::map<int, std::string> num_map;

        GenTenNumber(num_map);
        DumpNumMap(num_map);

        GenTwiNumber(num_map);
        DumpNumMap(num_map);

        GenThiNumber(num_map);
        DumpNumMap(num_map);

        GenFourNumber(num_map);
        DumpNumMap(num_map);

        GenFiveNumber(num_map);
        DumpNumMap(num_map);

        GenSixNumber(num_map);
        DumpNumMap(num_map);

        GenSevenNumber(num_map);
        DumpNumMap(num_map);

        GenEightNumber(num_map);
        DumpNumMap(num_map);

        GenNineNumber(num_map);
        DumpNumMap(num_map);

        GenHanNumber(num_map);
        DumpNumMap(num_map);

        GenHanMoreNumber(num_map);
        DumpNumMap(num_map);

        GenMoreNumber(num_map);
        DumpNumMap(num_map);
        return 0;
}
