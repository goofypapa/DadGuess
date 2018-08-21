#pragma once
#ifndef __MD5_HPP__
#define __MD5_HPP__

#include <string>
#include <fstream>

namespace WS_TOOLS
{
    /* Type define */  
    typedef unsigned char byte;   
    typedef unsigned int uint32;   

    class md5
    {
    public:
        md5( void );   
        md5( const void * input, size_t length );   
        md5( const std::string & str );   
        md5( std::ifstream & in );   
        void update( const void * input, size_t length );   
        void update( const std::string &  str );   
        void update( std::ifstream & in );   
        const std::string digest( void );   
        void reset( void );

    private:   
        void update( const byte * input, size_t length );   
        void final( void );   
        void transform( const byte block[64] );   
        void encode( const uint32 * input, byte * output, size_t length );   
        void decode( const byte * input, uint32 * output, size_t length );   
        std::string bytesToHexString( const byte * input, size_t length );   
        
    
        /* class uncopyable */  
        md5( const md5 & );   
        md5 & operator=( const md5 & );   
    
        uint32 _state[4]; /* state (ABCD) */  
        uint32 _count[2]; /* number of bits, modulo 2^64 (low-order word first) */  
        byte _buffer[64]; /* input buffer */  
        byte _digest[16]; /* message digest */  
        bool _finished; /* calculate finished ? */  

        static const byte * PADDING( void );
        static const char * HEX( void );
    
        // static const byte PADDING[64]; /* padding for calculate */  
        // static const char HEX[16];
        enum { BUFFER_SIZE = 1024 };   
    };
}

#endif //__MD5_HPP__

#include "achieve/md5.inl"
