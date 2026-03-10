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
# Constraints

1. The program runs on a fixed size buffer ( constant throughout execution ) ranging between 256 KB and 1024 KB depending on user's program execution . 
2. The program handles token splits across boundaries .
3. Each indexed file corresponds to a different version and maintains a separate index for each version .
4. Memory usage increases only as the number of unique words increases.

# Expected Output

The program outputs in the following format:
* Version name(s)
* Query result
* Allocated buffer size in KB
* Total execution time in seconds
