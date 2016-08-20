#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <string>
#include <QString>
#include <unordered_set>

void replaceAll(std::string& s, const std::string& search, const std::string& replace)
{
    for(size_t pos = 0; ; pos += replace.length())
    {
        pos = s.find(search, pos);
        if(pos == std::string::npos) break;
        s.erase(pos, search.length());
        s.insert(pos, replace);
    }
}

bool isValidFileName(const std::string& s)
{
    std::unordered_set<char> invalidSymbols;
    std::string invalidChars = R"q(<>:"/\|?*)q";
    for(char c : invalidChars)
    {
        invalidSymbols.insert(c);
    }
    for(char c : s)
    {
        if(invalidSymbols.find(c) != invalidSymbols.end())
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    if(argc == 2 && isValidFileName(argv[1]))
    {
        std::string usacoTemplate = R"q(/*
ID: paulpau2
PROG: stringToReplace
LANG: C++11
*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <complex>
#include <queue>
#include <stack>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

int main() {
    freopen("stringToReplace.in", "r", stdin);
    freopen("stringToReplace.out", "w", stdout);

})q";

        QString fullname = QDir::currentPath() + "\\" + argv[1];
        QFile cpp(fullname + ".cpp");
        QFile in(fullname + ".in");
        QFile out(fullname + ".out");
        if(!cpp.exists() && !in.exists() && !out.exists() && cpp.open(QIODevice::ReadWrite) && in.open(QIODevice::ReadWrite) && out.open(QIODevice::ReadWrite))
        {
            QTextStream fout(&cpp);
            replaceAll(usacoTemplate, "stringToReplace", argv[1]);
            fout << QString::fromStdString(usacoTemplate) << flush;
            qDebug() << "Successfully created the files.";
            return 0;
        }
    }
    qDebug() << "Something went wrong, the files weren't' created.";
}
