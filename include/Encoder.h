/******************************************************************************//*!
* @File          Encoder.h
* 
* @Title         Header file for LZW Encoder.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This header file defines the prototypes of classes and functions 
*                for LZW Encoder.
* 
*//*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <stdint.h>

#include "Trie.h"
#include "FileStream.h"


/******************************************************************************
* @Class		Encoder
*
* @Description	Class representing LZW Encoder.
* 				This class defines attributes and functionalities
*               for LZW Encoder.
******************************************************************************/
class Encoder
{
private:
    unsigned int    m_uiBitLength;
    unsigned int    m_uiMaxTableSize;
    static uint16_t s_u2Code;
    
    // Initialise a Trie with ASCII characters
    void InitialiseTrie(Node *pRootNode);
    
public:
    // Constructor
    Encoder(unsigned int uiBitLength=16)
    {
        // LZW Encoder does not support bit length greater than 16.
        if (uiBitLength > 16)
            std::cerr << "Bit Length should not be greater than 16."
                      << std::endl;
        
        m_uiBitLength    = uiBitLength;
        m_uiMaxTableSize = (unsigned int) pow(2.0, uiBitLength);
    }
    
    // Destructor
    ~Encoder() {}
    
    // Public setter
    void SetBitLength(unsigned int uiBitLength)
    {
        m_uiBitLength    = uiBitLength;
        m_uiMaxTableSize = (unsigned int) pow(2.0, uiBitLength);
    }
    
    // Public getter
    unsigned int GetBitLength() { return m_uiBitLength; }
    
    // LZW encoding
    void Encode(std::string pszTextFile);
};