/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include <stddef.h>

#include "utils.h"
#include "exception.h"

libhtmlpp::HTMLException::HTMLException(){
    _CType=Note;
    _Buffer=nullptr;
    _BufferSize=0;
};

libhtmlpp::HTMLException::~HTMLException(){
    delete[] _Buffer;
}

int libhtmlpp::HTMLException::getErrorType(){
    return _CType; 
}

const char* libhtmlpp::HTMLException::what() const throw(){
    return _Buffer;
}


libhtmlpp::HTMLException& libhtmlpp::HTMLException::asign(const char *src){
    if(!src)
        return *this;
    size_t srcsize=0;
EXCEPTIONLEN:
    if(src[srcsize++]!='\0')
        goto EXCEPTIONLEN;
    size_t nsize=(srcsize+_BufferSize);
    char *buf=new char[nsize];
    size_t i;
    for(i=0; i<_BufferSize; ++i)
        buf[i]=_Buffer[i];
    for(size_t ii = 0; ii<srcsize; ++ii)
        buf[i++]=src[ii];
    _BufferSize=nsize;
    buf[nsize]='\0';
    delete[] _Buffer;
    _Buffer=buf;
    return *this;   
}

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator[](int errtype){
    _CType=errtype;
    return *this;
}

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator<<(const char *src){
    return asign(src);       
};

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator<<(int src){
    char *buf=new char[sizeof(int)+1];
    itoa(src,buf);
    asign(buf);
    delete[] buf;
    return *this;
}
