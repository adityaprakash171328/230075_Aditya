#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include <chrono>
#include <cstring>
#include <cctype>
#include <stdexcept>
#include <string>        // Explicit include
#include <string_view>   // Added for memory optimization

using namespace std;


template<typename T>
void printTopK(const vector<pair<string, T>>& data, size_t k) {
    for (size_t i = 0; i < min(k, data.size()); ++i) {
        cout << data[i].first << " -> " << data[i].second << endl;
    }
}

class VersionedIndex {
private:
    string versionName;
    unordered_map<string, size_t> wordCount;

public:
    VersionedIndex(const string& name) : versionName(name) {}

    void addWord(const string& word) {
        ++wordCount[word];
    }

    size_t getWordCount(const string& word) const {
        auto it = wordCount.find(word);
        return (it != wordCount.end()) ? it->second : 0;
    }

    const unordered_map<string, size_t>& getIndex() const {
        return wordCount;
    }

    const string& getVersionName() const {
        return versionName;
    }
};

class BufferedFileReader {
private:
    ifstream file;
    size_t bufferSize;
    vector<char> buffer;

public:
    BufferedFileReader(const string& path, size_t kb) {
        bufferSize = kb * 1024;

        if (bufferSize < 256 * 1024 || bufferSize > 1024 * 1024)
            throw invalid_argument("Buffer size must be between 256KB and 1024KB");

        buffer.resize(bufferSize);

        file.open(path, ios::binary);
        if (!file)
            throw runtime_error("Unable to open file: " + path);
    }

    bool readChunk(string_view& outChunk) {
        if (!file.good()) return false;

        file.read(buffer.data(), bufferSize);
        streamsize bytesRead = file.gcount();

        if (bytesRead <= 0) return false;

        outChunk = string_view(buffer.data(), bytesRead);
        return true;
    }

    size_t getBufferSizeKB() const {
        return bufferSize / 1024;
    }
};


class Tokenizer {
public:
    void tokenizeAndAdd(string_view chunk, string& leftover, VersionedIndex& index) {
        string current = leftover;

        for (char c : chunk) {
            if (isalnum(static_cast<unsigned char>(c))) {
                current += tolower(c);
            } else {
                if (!current.empty()) {
                    index.addWord(current);
                    current.clear();
                }
            }
        }

        leftover = current;
    }
};

class QueryProcessor {
public:
    virtual void execute() = 0;
    virtual ~QueryProcessor() {}
};


class WordQuery : public QueryProcessor {
private:
    const VersionedIndex& index;
    string word;

public:
    WordQuery(const VersionedIndex& idx, const string& w)
        : index(idx), word(w) {}

    void execute() override {
        cout << "Version: " << index.getVersionName() << endl;
        cout << "Word Count [" << word << "] = "
             << index.getWordCount(word) << endl;
    }
};

class TopKQuery : public QueryProcessor {
private:
    const VersionedIndex& index;
    size_t k;

public:
    TopKQuery(const VersionedIndex& idx, size_t top)
        : index(idx), k(top) {}

    void execute() override {
        cout << "Version: " << index.getVersionName() << endl;

        vector<pair<string, size_t>> vec(index.getIndex().begin(),
                                         index.getIndex().end());

        sort(vec.begin(), vec.end(),
             [](auto& a, auto& b) {
                 return a.second > b.second;
             });

        printTopK(vec, k);
    }
};

class DiffQuery : public QueryProcessor {
private:
    const VersionedIndex& v1;
    const VersionedIndex& v2;
    string word;

public:
    DiffQuery(const VersionedIndex& a,
              const VersionedIndex& b,
              const string& w)
        : v1(a), v2(b), word(w) {}

    void execute() override {
        cout << "Version1: " << v1.getVersionName() << endl;
        cout << "Version2: " << v2.getVersionName() << endl;

        long diff = static_cast<long>(v2.getWordCount(word))
                  - static_cast<long>(v1.getWordCount(word));

        cout << "Difference for [" << word << "] = "
             << diff << endl;
    }
};


VersionedIndex buildIndex(const string& path,
                          const string& version,
                          size_t bufferKB) {
    BufferedFileReader reader(path, bufferKB);
    Tokenizer tokenizer;
    VersionedIndex index(version);

    string_view chunk;
    string leftover;

    while (reader.readChunk(chunk)) {
        tokenizer.tokenizeAndAdd(chunk, leftover, index);
    }

    if (!leftover.empty())
        index.addWord(leftover);

    return index;
}

string getArg(int argc, char* argv[], const string& key) {
    for (int i = 1; i < argc - 1; ++i)
        if (key == argv[i])
            return argv[i + 1];
    return "";
}


int main(int argc, char* argv[]) {
    try {
        auto start = chrono::high_resolution_clock::now();

        string queryType = getArg(argc, argv, "--query");
        if (queryType.empty()) 
            throw invalid_argument("Missing --query argument");

        // SAFETY: Check if buffer exists before passing to stoi
        string bufferStr = getArg(argc, argv, "--buffer");
        if (bufferStr.empty()) 
            throw invalid_argument("Missing --buffer argument");
        size_t bufferKB = stoi(bufferStr);

        if (queryType == "word") {
            string file = getArg(argc, argv, "--file");
            string version = getArg(argc, argv, "--version");
            string word = getArg(argc, argv, "--word");

            auto index = buildIndex(file, version, bufferKB);
            WordQuery query(index, word);
            query.execute();
        }
        else if (queryType == "top") {
            string file = getArg(argc, argv, "--file");
            string version = getArg(argc, argv, "--version");
            string topStr = getArg(argc, argv, "--top");
            if (topStr.empty()) throw invalid_argument("Missing --top argument");
            size_t k = stoi(topStr);

            auto index = buildIndex(file, version, bufferKB);
            TopKQuery query(index, k);
            query.execute();
        }
        else if (queryType == "diff") {
            string file1 = getArg(argc, argv, "--file1");
            string version1 = getArg(argc, argv, "--version1");
            string file2 = getArg(argc, argv, "--file2");
            string version2 = getArg(argc, argv, "--version2");
            string word = getArg(argc, argv, "--word");

            auto index1 = buildIndex(file1, version1, bufferKB);
            auto index2 = buildIndex(file2, version2, bufferKB);

            DiffQuery query(index1, index2, word);
            query.execute();
        }
        else {
            throw invalid_argument("Invalid query type: " + queryType);
        }

        auto end = chrono::high_resolution_clock::now();
        double duration = chrono::duration<double>(end - start).count();

        cout << "Buffer Size: " << bufferKB << " KB" << endl;
        cout << "Execution Time: " << duration << " seconds" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}