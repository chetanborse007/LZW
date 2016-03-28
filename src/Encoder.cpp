/******************************************************************************//*!
* @File          Encoder.cpp
* 
* @Title         Implementation of LZW Encoder.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This file implements member functions of LZW Encoder class.
*
*//*******************************************************************************/ 

#include "Encoder.h"


/* Initializing LZW Encoder's static data member. */
uint16_t Encoder::s_u2Code = 0;


/******************************************************************************
* @Function		Encoder::InitialiseTrie
*
* @Description	Initialise a Trie with ASCII characters.
*
* @Input		Node*		pRootNode       Pointer to Trie node
*
* @Return		void                        Returns nothing
******************************************************************************/
void Encoder::InitialiseTrie(Node *pRootNode)
{
    for(s_u2Code=0; s_u2Code<=255; s_u2Code++)
        Trie::GetInstance()->AddWord(pRootNode,
                                     std::string(1, (char) s_u2Code),
                                     s_u2Code);
}


/******************************************************************************
* @Function		Encoder::Encode
*
* @Description	Encode a text file using LZW compression algorithm.
*
* @Input		string		pszTextFile     Text file to be compressed
*
* @Return		void                        Returns nothing
******************************************************************************/
void Encoder::Encode(std::string pszTextFile)
{
    bool          bIsOverflow = false;
    char          chSymbol;
    std::string   pszWord = "";
    std::string   pszNewWord = "";
    Node          *pRootNode;
    Node          *pWord = NULL;
    char          *pszCompressedFile = new char[250];
    std::ifstream hTextFile;
    std::ofstream hCompressedFile;
    
    // Construct a name of compressed file
    pszTextFile.copy(pszCompressedFile, pszTextFile.rfind("."));
    strcat(pszCompressedFile, ".lzw");
    
    // Open a text file for reading text data and
    // a compressed file for writing encrypted data
    hTextFile.open(pszTextFile);
    hCompressedFile.open(pszCompressedFile, std::ios::binary | std::ios::out);
    
    // Create a EncryptStream using which encrypted data is written to a file
    EncryptStream encout(hCompressedFile);
    
    // Initialise Trie data structure with ASCII characters
    pRootNode = Trie::GetInstance()->GetRootNode();
    InitialiseTrie(pRootNode);
    
    // Fetch text data character by character from a text file,
    // till the EOF is reached
    while (hTextFile.get(chSymbol))
    {
        // Construct a new string using new extracted character
        pszNewWord = pszWord + chSymbol;
        
        // If new word exists in Trie,
        // then update 'word' with new word
        if (Trie::GetInstance()->SearchWord(pRootNode, pszNewWord) != NULL)
        {
            pszWord = pszNewWord;
        }
        // If new word does not exist in Trie,
        else
        {
            // Output the code for 'word'
            pWord = Trie::GetInstance()->SearchWord(pRootNode, pszWord);
            encout << pWord->GetCode();
            
            // Add new word into Trie, if Trie is not full
            if (s_u2Code < m_uiMaxTableSize && !bIsOverflow)
            {
                Trie::GetInstance()->AddWord(pRootNode,
                                             pszNewWord,
                                             s_u2Code);
                if (s_u2Code != (m_uiMaxTableSize-1))
                    s_u2Code++;
                else
                    bIsOverflow = true;
            }
            
            // Update 'word' with new extracted character
            pszWord = chSymbol;
        }
    }
    
    // Output the code for remaining 'word'
    pWord = Trie::GetInstance()->SearchWord(pRootNode, pszWord);
    encout << pWord->GetCode();
    
    // Clean memory occupied by Trie
    Trie::DestroyInstance();
    
    // Close files
    hTextFile.close();
    hCompressedFile.close();
}


/* Helper */
void ShowUsage(std::string pszExecutableName)
{
    std::cerr << "Usage: " << pszExecutableName << " <File Path> <Bit Length>\n"
    << "\tFile Path\t\t Path of text file to be encoded.\n"
    << "\tBit Length\t\t N-bit representation of code."
    << std::endl;
}


/* Entry point */
int main(int argc, const char *argv[])
{
    std::string  pszTextFile;
    unsigned int uiBitLength;
    
    // Parse commandline arguments
    if (argc != 3)
    {
        ShowUsage(argv[0]);
        return -1;
    }
    
    pszTextFile = argv[1];
    uiBitLength = atoi(argv[2]);
    
    // Create 'Encoder' instance
    Encoder *enc = new Encoder(uiBitLength);
    
    // Start encoding
    std::cout << __FUNCTION__
              << "(): Encrypting \'"
              << pszTextFile
              << "\'.."
              << std::endl;
    enc->Encode(pszTextFile);
    std::cout << __FUNCTION__
              << "(): Encrypting finished!"
              << std::endl;
    
    return 0;
}
