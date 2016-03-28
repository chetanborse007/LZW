/******************************************************************************//*!
* @File          Decoder.cpp
* 
* @Title         Implementation of LZW Decoder.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This file implements member functions of LZW Decoder class.
*
*//*******************************************************************************/ 

#include "Decoder.h"


/* Initializing LZW Decoder's static data member. */
uint16_t Decoder::s_u2Code = 0;


/******************************************************************************
* @Function		Decoder::InitialiseMap
*
* @Description	Initialise a Map with ASCII characters.
*
* @Input		map<uint16_t, string> &		CodeWordMap     Map of Code -> Word
*
* @Return		void                                        Returns nothing
******************************************************************************/
void Decoder::InitialiseMap(std::map<uint16_t, std::string> &CodeWordMap)
{
    for(s_u2Code=0; s_u2Code<=255; s_u2Code++)
        CodeWordMap[s_u2Code] = std::string(1, (char) s_u2Code);
}


/******************************************************************************
* @Function		Decoder::Decode
*
* @Description	Decode a compressed file using LZW decompression algorithm.
*
* @Input		string		pszCompressedFile     Compressed file
*                                                 to be decompressed
*
* @Return		void                        Returns nothing
******************************************************************************/
void Decoder::Decode(std::string pszCompressedFile)
{
    bool                            bIsOverflow = false;
    uint16_t                        u2Code;
    std::string                     pszWord = "";
    std::string                     pszNewWord = "";
    char                            *pszTextFile = new char[250];
    std::ifstream                   hCompressedFile;
    std::ofstream                   hTextFile;
    std::map<uint16_t, std::string> CodeWordMap;
    
    // Construct a name of decompressed file
    pszCompressedFile.copy(pszTextFile, pszCompressedFile.rfind("."));
    strcat(pszTextFile, "_decoded.txt");
    
    // Open a compressed file for reading encrypted data and
    // a decompressed file for writing text data
    hCompressedFile.open(pszCompressedFile.c_str(), std::ios_base::binary);
    hTextFile.open(pszTextFile, std::ios::out);
    
    // Create a DecryptStream using which encrypted data can be read
    DecryptStream decin(hCompressedFile);
    
    // Initialise Map with ASCII characters
    InitialiseMap(CodeWordMap);
    
    // Read first code from compressed file and output corresponding word
    decin >> u2Code;
    pszWord = CodeWordMap[u2Code];
    hTextFile << pszWord;
    
    // Fetch encrypted data two bytes at a time from a compressed file,
    // till the EOF is reached
    while (decin >> u2Code)
    {
        // If code does not exist in Map,
        // then create a new word
        // by appending first character of 'word' to 'word'.
        // else fetch word corresponding to code from Map.
        if (CodeWordMap[u2Code].empty())
            pszNewWord = pszWord + pszWord[0];
        else
            pszNewWord = CodeWordMap[u2Code];
        
        // Output word
        hTextFile << pszNewWord;
        
        // Add ('word' + first character of new word) into Map,
        // if Map is not full
        if (s_u2Code < m_uiMaxTableSize && !bIsOverflow)
        {
            CodeWordMap[s_u2Code] = pszWord + pszNewWord[0];
            if (s_u2Code != (m_uiMaxTableSize-1))
                s_u2Code++;
            else
                bIsOverflow = true;
        }
        
        // Update 'word' with a new word
        pszWord = pszNewWord;
    }
    
    // Close files
    hCompressedFile.close();
    hTextFile.close();
}


/* Helper */
void ShowUsage(std::string pszExecutableName)
{
    std::cerr << "Usage: " << pszExecutableName << " <File Path> <Bit Length>\n"
    << "\tFile Path\t\t Path of encrypted file to be decompressed.\n"
    << "\tBit Length\t\t N-bit representation of code."
    << std::endl;
}


/* Entry point */
int main(int argc, const char *argv[])
{
    std::string  pszCompressedFile;
    unsigned int uiBitLength;
    
    // Parse commandline arguments
    if (argc != 3)
    {
        ShowUsage(argv[0]);
        return -1;
    }
    
    pszCompressedFile = argv[1];
    uiBitLength       = atoi(argv[2]);
    
    // Create 'Decoder' instance
    Decoder *dec = new Decoder(uiBitLength);
    
    // Start decoding
    std::cout << __FUNCTION__
              << "(): Decrypting \'"
              << pszCompressedFile
              << "\'.." << std::endl;
    dec->Decode(pszCompressedFile);
    std::cout << __FUNCTION__
              << "(): Decrypting finished!"
              << std::endl;
    
    return 0;
}
