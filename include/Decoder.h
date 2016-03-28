/******************************************************************************//*!
* @File          Decoder.h
* 
* @Title         Header file for LZW Decoder.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This header file defines the prototypes of classes and functions 
*                for LZW Decoder.
* 
*//*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string.h>
#include <fstream>
#include <cmath>
#include <stdint.h>
#include <map>
#include <cstdlib>
	
#include "FileStream.h"


/******************************************************************************
* @Class		Decoder
*
* @Description	Class representing LZW Decoder.
* 				This class defines attributes and functionalities
*               for LZW Decoder.
******************************************************************************/
class Decoder
{
private:
    unsigned int    m_uiBitLength;
    unsigned int    m_uiMaxTableSize;
    static uint16_t s_u2Code;
    
    // Initialise a Trie with ASCII characters
    void InitialiseMap(std::map<uint16_t, std::string> &CodeWordMap);
    
public:
    // Constructor
    Decoder(unsigned int uiBitLength=16)
    {
        if (uiBitLength > 16)
            std::cerr << "Bit Length should not be greater than 16."
                      << std::endl;
        
        m_uiBitLength    = uiBitLength;
        m_uiMaxTableSize = (unsigned int) pow(2.0, uiBitLength);
    }
    
    // Destructor
    ~Decoder() {}
    
    // Public setter
    void SetBitLength(unsigned int uiBitLength)
    {
        m_uiBitLength    = uiBitLength;
        m_uiMaxTableSize = (unsigned int) pow(2.0, uiBitLength);
    }
    
    // Public getter
    unsigned int GetBitLength() { return m_uiBitLength; }
    
    // LZW decoding
    void Decode(std::string pszCompressedFile);
};
