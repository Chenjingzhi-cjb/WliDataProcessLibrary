#ifndef WLI_DATA_READER_HPP
#define WLI_DATA_READER_HPP

#include <iostream>
#include <fstream>
#include <windows.h>
#include <utility>
#include <string>
#include <vector>


class WliDataReader {
public:
    explicit WliDataReader(std::string file_path)
            : m_file_path(std::move(file_path)) {}

    ~WliDataReader() = default;

public:
    bool readFromDat() {
        if (m_file_path.empty() || m_file_path.substr(m_file_path.size() - 4, 4) != ".dat") return false;

#ifdef _MSC_VER
        FILE *dat_file = nullptr;
        errno_t err = _wfopen_s(&dat_file, string2wstring(m_file_path).c_str(), L"r");
#else
        FILE *dat_file = _wfopen(string2wstring(m_spm_path).c_str(), L"r");
#endif

        if (!dat_file) {
            std::cout << "Failed to open the dat file: " << m_file_path << std::endl;
            return {};
        }

        m_data.clear();

        std::string line;
        std::wstring line_w;
        wchar_t buffer[1024];
        while (fgetws(buffer, sizeof(buffer) / sizeof(buffer[0]), dat_file)) {
            line_w = buffer;
            line = wstring2string(line_w);
            line.assign(line.begin(), line.end() - 1);  // 去除 '\n'

            int first_semicolon_pos = (int) line.find_first_of(';');
            int second_semicolon_pos = (int) line.find_last_of(';');

            m_data.emplace_back(std::vector<double>{
                    std::stod(line.substr(0, first_semicolon_pos)),
                    std::stod(line.substr(first_semicolon_pos + 1, second_semicolon_pos - first_semicolon_pos - 1)),
                    std::stod(line.substr(second_semicolon_pos + 1))
            });
        }

        fclose(dat_file);

        return true;
    }

    std::vector<std::vector<double>> &getData() {
        return m_data;
    }

    /**
     * @brief string 转 wstring
     *
     * @param str src string
     * @param CodePage The encoding format of the file calling this function. CP_ACP for gbk, CP_UTF8 for utf-8.
     * @return dst string
     */
    static std::wstring string2wstring(const std::string &str, _In_ UINT CodePage = CP_ACP) {
        int len = MultiByteToWideChar(CodePage, 0, str.c_str(), -1, nullptr, 0);
        std::wstring wstr(len, L'\0');
        MultiByteToWideChar(CodePage, 0, str.c_str(), -1, const_cast<wchar_t *>(wstr.data()), len);
        wstr.resize(wcslen(wstr.c_str()));
        return wstr;
    }

    /**
     * @brief wstring 转 string
     *
     * @param wstr src wstring
     * @param CodePage The encoding format of the file calling this function. CP_ACP for gbk, CP_UTF8 for utf-8.
     * @return dst string
     */
    static std::string wstring2string(const std::wstring &wstr, _In_ UINT CodePage = CP_ACP) {
        int len = WideCharToMultiByte(CodePage, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string str(len, '\0');
        WideCharToMultiByte(CodePage, 0, wstr.c_str(), -1, const_cast<char *>(str.data()), len, nullptr, nullptr);
        str.resize(strlen(str.c_str()));
        return str;
    }

private:
    std::string m_file_path;

    std::vector<std::vector<double>> m_data;  // {{x1, y1, z1}, {x2, y2, z2}}
};


#endif //WLI_DATA_READER_HPP
