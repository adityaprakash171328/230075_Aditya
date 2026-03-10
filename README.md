# Memory-Efficient Versioned File Indexer

CS253 Course - Programming Assignment C++
Made By: 230075_Aditya

# Project Description:
This program processes one or more log files and builds an index that records each word along with how often it appears in different versions of the files. The files are read in chunks of a user-defined size so the entire file does not need to be loaded into memory at once. The collected word frequencies are stored in a version-based index, which allows users to perform queries and compare how often specific words appear across multiple file versions. The system supports three different types of queries for analyzing the indexed data.

## How to Compile

Make sure you have GCC installed. On Windows with MinGW, run:

    g++ -O2 -o analyzer 230075_Aditya.cpp -static-libgcc -static-libstdc++

The -static-libgcc -static-libstdc++ flags are needed on Windows so the executable does not depend on MinGW DLLs being in PATH.

    g++ -O2 -o analyzer 230075_Aditya.cpp 

 # Execution 

To run the program, you first need to compile the source code using a standard C++ compiler. 

Compile command:
`g++ -O3 230075_Aditya.cpp -o analyzer`

### Command-Line Arguments

--file        (word, top)  Path to the log file
--file1       (diff)       Path to the first file
--file2       (diff)       Path to the second file
--version     (word, top)  Version label for the file
--version1    (diff)       Version label for the first file
--version2    (diff)       Version label for the second file
--query       (all)        Query type: word / top / diff
--word        (word, diff) The word to search for
--top         (top)        How many top words to return
--buffer      (all)        Buffer size in KB (256 to 1024, default 512)

## Project Structure

| Class Name | Primary Responsibility |
| --- | --- |
| **`VersionedIndex`** | Manages a versioned word-frequency map and provides lookup access. |
| **`BufferedFileReader`** | Efficiently reads file data into memory using a fixed-size binary buffer. |
| **`Tokenizer`** | Cleans raw text, handles word fragments, and populates the index. |
| **`QueryProcessor`** | Provides an interface for executing different analytical operations. |
| **`WordQuery`** | Retrieves and displays the frequency of a specific word in an index. |
| **`TopKQuery`** | Identifies and displays the most frequent words in a given index. |
| **`DiffQuery`** | Calculates the frequency change of a word between two index versions. |

---
# THINGS WHICH ARE NOTED:
1. All informational messages such as logs and execution timing are written to stderr, ensuring they remain separate from the actual query results printed to stdout.
2. The buffer size must be within the range of 256 KB to 1024 KB. If a value outside this range is provided, the program will generate an error.
3. Before indexing, all words are converted to lowercase, meaning words like “Error” and “error” are treated as the same entry.

# Expected Output: 
The program outputs in the following format:
* Version name(s)
* Query result
* Allocated buffer size in KB
* Total execution time in seconds
