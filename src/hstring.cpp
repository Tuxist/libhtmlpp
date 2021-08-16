/*******************************************************************************
Copyright (c) 2021, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "utils.h"
#include "hstring.h"
#include "exception.h"

#define HTMLTAG_OPEN '<'
#define HTMLTAG_TERMINATE '/'
#define HTMLTAG_CLOSE '>'

libhtmlpp::HtmlString::HtmlString(){
    _Data=nullptr;
    _DataSize=0;
}

libhtmlpp::HtmlString::~HtmlString(){
    delete[] _Data;
}

void libhtmlpp::HtmlString::assign(const char* src, size_t srcsize){
    size_t nsize=_DataSize+srcsize;
    char *buf=new char [nsize+1];
    size_t i=0;
    while(i<_DataSize){
       buf[i]=_Data[i];
       ++i;
    }
    for(size_t ii = 0; ii<srcsize; ++ii)
        buf[i++]=src[ii];
    _DataSize=nsize;
    delete[] _Data;
    buf[nsize]='\0';
    _Data=buf;
}

void libhtmlpp::HtmlString::push_back(const char src){
    size_t nsize=_DataSize+1;
    char *buf=new char [nsize+1];
    size_t i;
    for(i = 0; i<_DataSize; ++i){
        buf[i]=_Data[i];
    }
    buf[++i]=src;
    buf[nsize]='\0';
    _DataSize=nsize;
    delete[] _Data;
    _Data=buf;
}

void libhtmlpp::HtmlString::assign(const char* src) {
    assign(src,getlen(src));
}

void libhtmlpp::HtmlString::insert(size_t pos, char src){
    if(pos < _DataSize){
        libhtmlpp::HTMLException excep;
        excep.Critical("HtmlString: out of String");
        throw excep;
    }
    _Data[pos]=src;
}

void libhtmlpp::HtmlString::clear(){
    delete[] _Data;
    _Data=nullptr;
    _DataSize=0;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator+=(const char *src){
    assign(src);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator=(const char *src){
    clear();
    _DataSize=getlen(src);
    _Data = new char[_DataSize];
    for(size_t i = 0; i<_DataSize; ++i){
        _Data[i]=src[i];
    } 
    return *this;
}

char &libhtmlpp::HtmlString::operator[](size_t pos){
    return _Data[pos];
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(const char* src){
    assign(src);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(int src){
    push_back((const char)src);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(size_t src){
    push_back((const char)src);
    return *this;
}

const char *libhtmlpp::HtmlString::c_str() {
    return _Data;
}

size_t libhtmlpp::HtmlString::size(){
    return _DataSize;
}
  
bool libhtmlpp::HtmlString::validate(){
    size_t oa=0,ca=0;  
    for(size_t i=0; i<_DataSize; ++i){
        switch(_Data[i]){
            case HTMLTAG_OPEN:
                ++oa;
                break;
            case HTMLTAG_CLOSE:
                ++ca;
                break;
            default:
                break;
        }
    }
    
    Console con;
    con << oa << ": " << ca << con.endl();
    
    if(oa!=ca)
        return false;  
    
    ssize_t **htable = new ssize_t*[oa];
    for (size_t is = 0; is < oa; is++) {
        htable[is] = new ssize_t[3]{-1,-1};
    }
    
    bool open=false;
    for(size_t ii=0; ii<ca; ++ii){
        for(size_t ip=0; ip<_DataSize; ++ip){
          switch(_Data[ip]){
            case HTMLTAG_OPEN:{
                open=true;
                htable[ii][0]=ip;
            }break;
            case HTMLTAG_CLOSE:{
                if(!open)
                    return false;
                htable[ii][2]=ip;
                open=false;
            }break;
            default:
                break;
            }
        }
    }
    
    return true;
}
