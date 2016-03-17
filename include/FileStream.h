/******************************************************************************//*!
* @File          FileStream.h
* 
* @Title         Header file for special input and output file streams.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This header file defines the prototypes of classes and functions 
*                for special input/output file streams 
*                required for reading/writing encrypted data.
* 
*//*******************************************************************************/ 

#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>


/******************************************************************************
* @Class		EncryptStream
*
* @Description	Class representing EncryptStream.
* 				This class defines attributes and functionalities
*               required for writing encrypted data into file.
******************************************************************************/
class EncryptStream
{
private:
    std::ostream &m_OutStream;
    
public:
    // Constructor
    EncryptStream(std::ostream &OutStream) : m_OutStream(OutStream) {}
    
    // Operator overloading for '<<'
    // Note: This operator writes encrypted code into file
    //       in Big Endien notation
    void operator<<(uint16_t u2Code)
    {
        m_OutStream.put((u2Code >> 8) & 0xff);
        m_OutStream.put(u2Code & 0xff);
    }
};


/******************************************************************************
* @Class		DecryptStream
*
* @Description	Class representing DecryptStream.
* 				This class defines attributes and functionalities
*               required for reading encrypted data from file.
******************************************************************************/
class DecryptStream
{
private:
    std::istream &m_InStream;
    
public:
    // Constructor
    DecryptStream(std::istream &InStream) : m_InStream(InStream) {}
    
    // Operator overloading for '>>'
    bool operator>>(uint16_t &u2Code)
    {
        char ch;
        
        if (m_InStream.get(ch))
        {
            u2Code = ch & 0xff;
            
            if (m_InStream.get(ch))
            {
                u2Code = (u2Code << 8) | (ch & 0xff);
                return true;
            }
        }
        
        return false;
    }
};