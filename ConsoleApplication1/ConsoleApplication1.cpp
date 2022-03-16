#include <iostream>
#include <Windows.h>
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{
    HANDLE file;
    file = CreateFile(L"C:\\Users\\complito\\source\\repos\\ConsoleApplication1\\book.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        cout << "File didn't opened";
        return 1;
    }

    LARGE_INTEGER fileSize;
    BOOL isFileSizeGot = GetFileSizeEx(file, &fileSize);
    if (!isFileSizeGot)
    {
        cout << "Didn't get file size";
        return 2;
    }
    cout << "File size: " << fileSize.QuadPart;

    char* buffer = new char[fileSize.QuadPart + 1];
    DWORD readBytesNumber;
    bool isFileRead = ReadFile(file, buffer, fileSize.QuadPart, &readBytesNumber, NULL);
    if (!isFileRead)
    {
        cout << "File didn't read";
        return 3;
    }
    if (readBytesNumber != fileSize.QuadPart)
    {
        cout << "Number of read bytes doesn't equal to file size";
        return 4;
    }
    cout << "\nFile is read\n\n";

    milliseconds startTime = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
        );

    buffer[fileSize.QuadPart] = '\0';
    char* start = buffer;
    map<string, size_t> words;
    for (int i = 0; i < fileSize.QuadPart; ++i)
        if (!((buffer[i] >= 48 && buffer[i] <= 57) ||
            (buffer[i] >= 65 && buffer[i] <= 90) ||
            (buffer[i] >= 97 && buffer[i] <= 122)))
        {
            buffer[i] = '\0';
            if (start[0] != '\0')
                words[start]++;
            start = &buffer[i + 1];
        }

    vector<pair<string, size_t>> vectors;
    for (pair<string, size_t> item : words)
        vectors.push_back(item);

    sort(vectors.begin(), vectors.end(), []
    (pair<string, size_t> a, pair<string, size_t> b) {
            return a.second > b.second;
        });

    int i = 0;
    for (pair<string, size_t> item : vectors)
    {
        if (++i > 10) break;
        cout << item.first << " : " << item.second << '\n';
    }

    milliseconds endTime = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
        );

    milliseconds diff = endTime - startTime;

    cout << "\nProgram running time in milliseconds: " << diff.count() << '\n';
}