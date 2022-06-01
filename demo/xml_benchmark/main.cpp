#include <iostream>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "tinyxml2.h"
// #include "tinyxml2.cpp"
#ifdef _WIN32
#include <time.h>
#include <windows.h>
#include <tchar.h>
#else
#include <sys/time.h>
#endif

#define TEST_TIMES 500


#ifdef _WIN32
int gettimeofday(struct timeval *tp, void *tzp) {
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return 0;
}
#endif

using namespace std;

char* readFile(char* filename) {
    auto fp = std::fopen(filename, "r");
    std::fseek(fp, 0, SEEK_END);
    auto len = std::ftell(fp);
    std::fseek(fp, 0, SEEK_SET);
    char* buffer = new char[len+1]{0};
    std::fread(buffer, 1, len, fp);
    std::fclose(fp);
    return buffer;
}

void pugixml(char *xml_data) {
    timeval starttime, endtime;
    gettimeofday(&starttime, 0);
    cout << " \n** The pugi start *******************" << starttime.tv_sec << "," << starttime.tv_usec << "\n" << endl;
    pugi::xml_document doc;
    double timeuse = 0.;
    double timeAverage = 0.;
    for (int i = 0; i < TEST_TIMES; ++i)
    {
        gettimeofday(&starttime, 0);
        if (doc.load_string(xml_data) == 0) {
            cout << "failed in load xml file! _ " << i << endl;
            break;
        }
        gettimeofday(&endtime, 0);
        timeuse = 1000000. * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
        timeuse *= 0.001;
        // cout << " reading files costs : " << timeuse << "ms root name:" << doc.first_child().name() << endl;
        timeAverage += timeuse;
    }
    timeAverage /= TEST_TIMES;
    gettimeofday(&starttime, 0);
    cout << " \n** The pugi end *******************\n    the average costs = " << timeAverage << "ms" << starttime.tv_sec << "," << starttime.tv_usec << "\n" << endl;
}

void tinyxml(char* xml_data) {
    tinyxml2::XMLDocument doc;
    timeval starttime, endtime;
    gettimeofday(&starttime, 0);
    cout << " \n** The tiny start ******************* " << starttime.tv_sec << "," << starttime.tv_usec << "\n" << endl;
    double timeuse = 0.;
    double timeAverage = 0.;
    for (int i = 0; i < TEST_TIMES; ++i)
    {
        gettimeofday(&starttime, 0);
        tinyxml2::XMLError err = doc.Parse(xml_data);
        if (err != 0) {
            cout << "failed in load xml! " << i << err << doc.ErrorStr() << endl;
            break;
        }
        gettimeofday(&endtime, 0);
        timeuse = 1000000. * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
        timeuse *= 0.001;
        // cout << " reading files costs : " << timeuse << "ms root name: " << doc.RootElement()->Name() << endl;
        timeAverage += timeuse;
        doc.Clear();
    }
    timeAverage /= TEST_TIMES;
    gettimeofday(&starttime, 0);
    cout << " \n** The tiny end *******************\n    the average costs = " << timeAverage << "ms " << starttime.tv_sec << "," << starttime.tv_usec << "\n" << endl;
}

using namespace std;
#ifdef UNICODE

int wmain(int argc, TCHAR* argv[]) {
    _wsetlocale(0, L".65001");
#else
int main(int argc, TCHAR* argv[]) {
#endif
    auto xml_data = readFile((char*)argv[1]);
    pugixml(xml_data);
    tinyxml(xml_data);
    return 0;
}
