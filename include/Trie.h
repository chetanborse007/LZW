/******************************************************************************//*!
* @File          Trie.h
* 
* @Title         Header file for Trie data structure.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This header file defines the prototypes of classes and functions 
*                for Trie data structure.
* 
*//*******************************************************************************/ 

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>


/******************************************************************************
* @Class		Node
*
* @Description	Class representing Trie Node.
* 				This class defines attributes and functionalities
*               of Trie Node.
******************************************************************************/
class Node
{
private:
    char               m_chSymbol;
    uint16_t           m_u2Code;
    bool               m_bIsWord;
    std::vector<Node*> m_vpChildren;

public:
    // Constructor
    Node(char chSymbol='\0', uint16_t u2Code=65535, bool bIsWord=false)
    {
        m_chSymbol = chSymbol;
        m_u2Code   = u2Code;
        m_bIsWord  = bIsWord;
    }

    // Destructor
    ~Node() {}

    // Public setter
    void SetSymbol(char chSymbol) { m_chSymbol = chSymbol; }
    void SetCode(uint16_t u2Code) { m_u2Code = u2Code; }
    void SetIsWord(bool bIsWord) { m_bIsWord = bIsWord; }
    
    // Public getter
    char const GetSymbol() { return m_chSymbol; }
    uint16_t const GetCode() { return m_u2Code; }
    bool const GetIsWord() { return m_bIsWord; }
    std::vector<Node*> const GetChildren() { return m_vpChildren; }

    // Check whether the current node is marked as a Word
    bool const IsWord() { return m_bIsWord; }

    // Add a child node
    void AddChildNode(Node* pChild) { m_vpChildren.push_back(pChild); }
    
    // Search for a child node
    Node* SearchChildNode(char chSymbol)
    {
        for (int i=0; i<m_vpChildren.size(); i++)
            if (chSymbol == m_vpChildren[i]->m_chSymbol)
                return m_vpChildren[i];
        
        return NULL;
    }
};


/******************************************************************************
* @Class		Trie
*
* @Description	Class representing Trie data structure.
* 				This class defines attributes and functionalities
*               of Trie data structure.
******************************************************************************/
class Trie
{
private:
    Node        *m_pRoot;
    
    static Trie *s_pTrie;

    // Constructor
    Trie()
    {
        m_pRoot = new Node;
    }
    
    // Destructor
    ~Trie() {}
    
public:
    // Get a Root Node of Trie data structure
    Node* const GetRootNode();
    
    // Get an instance of singleton Trie class
    static Trie *GetInstance();
    
    // Destroy an instance of singleton Trie class
    static void DestroyInstance();
    
    // Store a word into Trie data structure
    void AddWord(Node *pNode, std::string pszWord, uint16_t u2Code);

    // Search for a word into Trie data structure
    Node* SearchWord(Node *pNode, std::string pszWord);
};