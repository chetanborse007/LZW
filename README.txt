**************************************
*  LEMPEL–ZIV–WELCH (LZW) ALGORITHM  *
**************************************


* Introduction
* File List
* Requirements
* Installation
* Usage
* Design Decision
* Bottleneck
* Future Development
* References
* Maintainer


I. INTRODUCTION
---------------

The Lempel–Ziv–Welch (LZW) algorithm is a lossless data compression algorithm.

LZW is an adaptive compression algorithm that does not assume prior knowledge
of the input data distribution. This algorithm works well when the input data
is sufficiently large and there is redundancy in the data.

Two examples of commonly used file formats that use LZW compression are the
GIF image format served from websites and the TIFF image format. LZW compression
is also suitable for compressing text files, and is the algorithm in the
compress Unix file compression utility.

This algorithm has two modules:
1. Encoding/Compressing
2. Decoding/Decompressing


II. FILE LIST
-------------

A. Header Files:
    Encoder.h		Header for LZW compression module
    Decoder.h		Header for LZW decompression module
    Trie.h			Header for Trie data structure
    FileStream.h	Header for customized File Streams

B. Source Files:
    Encoder.cpp		Implementation of LZW compression module
    Decoder.cpp		Implementation of LZW decompression module
    Trie.cpp		Implementation of Trie data structure

C. Sample Data Files:
    input1.txt		Small size data file
    input2.txt		Small size data file
    input3.txt		Large data file
    input4.txt		Large data file

D. Makefile generator:
    CMakeList.txt	CMake configuration to generate Makefile for LZW project

E. README


III. REQUIREMENTS
-----------------

This application requires the following modules:
1. g++ (Preferably Version 4.8 and above)
2. CMake (https://cmake.org) (Preferably Version 3.4.1 and above)
3. Mac OS or Linux (Not tested on Windows)


IV. INSTALLATION
----------------

A. MAC OS:
    1. Download `LZW.zip`

    2. Uncompress `LZW.zip`
        $ unzip LZW.zip

    3. Create build directory anywhere you want
        $ mkdir build

    4. Change to build directory
        $ cd build

    5. Generate Makefile
        $ cmake <Source Directory>
        e.g. $ cmake ../LZW/

    6. Build source
        $ make

    7. Find `Encoder` and `Decoder` utility within build directory itself

B. Linux:
    Same as described in (IV)-[A].
    Note: Few commands will be different depending upon your Linux flavour.

C. Windows:
    Not tested.


V. USAGE
--------

A. Mac OS:
    Note: File name should be absolute.

    1. Compress file using `Encoder` utility,
        $ ./Encoder <File Name> <Bit Length>
        e.g. ./Encoder /Users/chetan/Desktop/LZW/data/input1.txt 16

    2. Decompress file using `Decoder` utility,
        $ ./Decoder <Compressed File Name> <Bit Length>
        e.g. ./Decoder /Users/chetan/Desktop/LZW/data/input1.lzw 16

B. Linux:
    Same as described in (V)-[A].

C. Windows:
    Not tested.


VI. DESIGN DECISION
-------------------

A. Modules:
    * Encoder
    * Decoder

B. Encoder:
    1. Psuedo Code -
        MAX_TABLE_SIZE = 2^(bit_length)
        Initialize TABLE[0 to 255] = code for individual characters
        STRING = null
        While there are still input symbols:
            SYMBOL = get input symbol
            If STRING + SYMBOL is in TABLE:
                STRING = STRING + SYMBOL
            else:
                Output the code for STRING
                If TABLE.size < MAX_TABLE_SIZE:
                    Add STRING + SYMBOL to TABLE
                STRING = SYMBOL
        Output the code for STRING

    2. Data Structure -
        For LZW Encoder implementation, Trie data structure is used for storing
        table of pairs (String, Code) where the `String` has only single
        character.
        A Trie (Prefix Tree) is an ordered tree data structure that is used
        to store a dynamic set or associative array where the keys are usually
        strings.
        Reason of using Trie:
            > Dynamic data structure compare to Hash Table
            > Predictable O(k) lookup time where k is the size of the Word
            > No need for a hash function and hence no issue of collision

        a. Trie Node:
            Attributes:
                m_chSymbol      Character (from Word) to be stored into Trie.
                m_u2Code        16 bit code corresponding to Word.
                                Note: It is stored in Trie Node representing
                                last character of a word.
                m_bIsWord       Boolean flag to check whether the current Trie node
                                is Word or not.
                m_vpChildren    Children of current Trie node i.e. Vector of Trie nodes

            Methods:
                IsWord()                        To check whether the current node
                                                is marked as a Word.
                AddChildNode(Node* pChild)      To add a child node.
                SearchChildNode(char chSymbol)  To search for a child node.

        b. Trie:
            Trie is implemented using Singleton Design Pattern, so that only one
            instance of Trie will exist and will be used throughout the application.

            Methods:
                [AddWord(Node *pNode, string pszWord, uint16_t u2Code)]
                    To store a word into Trie data structure.

                    Algorithm -
                        1. Extract leftmost character from a word.
                        2. Search extracted character into a collection of children.
                        3. If not found,
                            Create a new node and add it into the vector of children.
                           Else,
                            Recursively traverse Trie downwards, until entire
                            new word is added.
                        4. Once reached at the end of word, store equivalent code
                           and mark it as a 'Word'.

                [SearchWord(Node *pNode, string pszWord)]
                    Search for a word into Trie data structure.

                    Algorithm -
                        1. Extract leftmost character from a word.
                        2. Search extracted character into a collection of children.
                        3. If not found,
                            Return NULL.
                           Else,
                            Recursively traverse Trie downwards, until entire
                            word is found.
                        4. Once reached at the end of word, return pointer to current
                           Trie node.

    3. Customized File Stream (EncryptStream) -
            Attribute:
                m_OutStream     File Stream Writer.

            Method:
                operator<<(uint16_t u2Code)     Operator overloading for writing
                                                16 bit code in Big Endien notation
                                                into compressed file.

B. Decoder:
    1. Psuedo Code -
        MAX_TABLE_SIZE=2^(bit_length)
        Initialize TABLE[0 to 255] = code for individual characters
        CODE = read next code from encoder
        STRING = TABLE[CODE]
        Output STRING
        While there are still codes to receive:
            CODE = read next code from encoder
            If TABLE[CODE] is not defined:
                NEW_STRING = STRING + STRING[0]
            else:
                NEW_STRING = TABLE[CODE]
            Output NEW_STRING
            If TABLE.size < MAX_TABLE_SIZE:
                Add STRING + NEW_STRING[0] to TABLE
            STRING = NEW_STRING

    2. Data Structure -
        For LZW Decoder implementation, `map` data structure is used for storing
        table of pairs (Code, String).
        `map` is an associative container and is usually implemented with
        a red-black tree which is a height balanced tree.

        a. Map:
            CodeWordMap      map<uint16_t, string>

    3. Customized File Stream (DecryptStream) -
        Attribute:
            m_InStream     File Stream Reader.

        Method:
            operator>>(uint16_t &u2Code)    Operator overloading for reading
                                            16 bit code from compressed file.

C. Fixed size data type for `Code`:
    In implementation, `uint16_t` data type is used, which takes 16 bit storage
    and is independant of platform.


VII. BOTTLENECK
---------------

This application can encode/decode a file with bit length of code <= 16 only.


VIII. FUTURE DEVELOPMENT
------------------------

1. Encode a file with variable length codes, which will further compress the data.


IX. REFERENCES
--------------

1. https://en.wikipedia.org/wiki/Trie#cite_note-13
2. http://fbim.fh-regensburg.de/~saj39122/sal/skript/progr/pr45102/Tries.pdf
3. https://www.topcoder.com/community/data-science/data-science-tutorials/using-tries/
4. https://en.wikipedia.org/wiki/Lempel–Ziv–Welch


X. MAINTAINER
-------------

Name        Chetan Borse
EMail ID    cborse@uncc.edu or chetanborse2106@gmail.com
LinkedIn    https://www.linkedin.com/in/chetanrborse