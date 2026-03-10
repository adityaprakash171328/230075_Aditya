# 230075_Aditya
# 1. Description

The cpp file contains code to parse and memory efficiently word index text files via the process of tokenization . It supports the following queries : 
1. k most frequent words in a version the text file 
2. frequency of a word in a version of the text file 
3. Difference in frequency of a word in two versions of two text files

# 2. Features

The implementation has been carried out keeping different salient features in mind . Particularly the following have been implemented to follow good practices :

1. Object oriented programming using 7 user defined classes (e.g. FileBuffer , Tokenizer , QueryProcessor , VersionIndexing)
2. Inheritance using one abstract class (QueryProcessor) and three derived classes (WordQuery , TopKQuery , DiffQuery). 
3. Virtual function (processQuery) and dynamic dispatch display runtime polymorphism 
4. Function overloading in frequency retrieval functions by defining using two different parameter lists 
5. Exception handling using (try , catch , throw) to handle inappropriate inputs , file I/O errors and other issues .
6. User defined template function (printResult) to print messages 
7. Word level index that maps each unique word to the frequency of its occurrence . 
8. Word is a contiguous sequence of alphanumeric characters . All words are taken to be case-insensitive for all purposes .

# 3. Constraints

1. The program runs on a fixed size buffer ( constant throughout execution ) ranging between 256 KB and 1024 KB depending on user's program execution . 
2. The program handles token splits across boundaries .
3. Each indexed file corresponds to a different version and maintains a separate index for each version .
4. Memory usage increases only as the number of unique words increases .

# 4. Execution 

To run the program, you first need to compile the source code using a standard C++ compiler. 

Compile command:
`g++ -O3 230618_mahir.cpp -o analyzer`

### Command-Line Arguments

| Flag | Description |
| :--- | :--- |
| `--file <path>` | Path to input file (single-version queries) |
| `--file1 <path>` | First input file (diff query) |
| `--file2 <path>` | Second input file (diff query) |
| `--version <name>` | Version identifier (single-version queries) |
| `--version1 <name>` | First version identifier (diff query) |
| `--version2 <name>` | Second version identifier (diff query) |
| `--buffer <kb>` | Buffer size in kilobytes (256 to 1024) |
| `--query <type>` | `word` \| `diff` \| `top` |
| `--word <token>` | Word for word/diff queries |
| `--top <k>` | Number of top results (top query) |

# 5. Examples

1. **Word query (single file):**
`./analyzer --file dataset_v1.txt --version v1 --buffer 512 --query word --word error`

2. **Top-K query (single file):**
`./analyzer --file dataset_v1.txt --version v1 --buffer 512 --query top --top 10`

3. **Difference query (two files):**
`./analyzer --file1 dataset_v1.txt --version1 v1 --file2 dataset_v2.txt --version2 v2 --buffer 512 --query diff --word error`

# 6. Expected Output

The program outputs in the following format:
* Version name(s)
* Query result
* Allocated buffer size in KB
* Total execution time in seconds
