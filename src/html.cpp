/*******************************************************************************
Copyright (c) 2014, Jan Koester jan.koester@gmx.net
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

#include <fcntl.h>

#include "utils.h"
#include "html.h"
#include "config.h"

#define HTMLTAG_OPEN '<'
#define HTMLTAG_TERMINATE '/'
#define HTMLTAG_CLOSE '>'

libhtmlpp::HtmlString::HtmlString(){
    _InitString();
}

libhtmlpp::HtmlString::~HtmlString(){
   clear();
}

void libhtmlpp::HtmlString::assign(const char* src, size_t srcsize){
    size_t nsize=_DataSize+srcsize;
    char *buf=new char [nsize+1];
    size_t i=0;
    scopy(_Data,_Data+_DataSize,buf);
    scopy(src,src+srcsize,buf+_DataSize);
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
        _HTMLException[HTMLException::Critical] << "HtmlString: out of String" << HTMLException::endl;
        throw _HTMLException;
    }
    _Data[pos]=src;
}

void libhtmlpp::HtmlString::clear(){
    for(size_t i=0; i<_HTableSize; ++i){
        delete[] _HTable[i];
    }
    delete   _HTable;
    delete   _HtmlRootNode;
    delete[] _Data;
    _InitString();
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
    char *buf=new char[sizeof(int)+1];
    itoa(src,buf);
    assign(buf);
    delete[] buf;
    return *this;
}

const char *libhtmlpp::HtmlString::c_str() {
    return _Data;
}

size_t libhtmlpp::HtmlString::size(){
    return _DataSize;
}
  
bool libhtmlpp::HtmlString::validate(){
    for(size_t i=0; i<_DataSize; ++i){
        switch(_Data[i]){
            case HTMLTAG_OPEN:
                ++_OpenTags;
                break;
            case HTMLTAG_CLOSE:
                ++_CloseTags;
                break;
            default:
                break;
        }
    }
    
    if(_OpenTags!=_CloseTags)
        return false;  
    
    return true;
}

void libhtmlpp::HtmlString::_InitString(){
    _Data=nullptr;
    _DataSize=0;
    _OpenTags=0;
    _CloseTags=0;
    _HTable=nullptr;
    _HTableSize=0;
    _HtmlRootNode=nullptr;   
}

void libhtmlpp::HtmlString::_parseTree(){
    if(!validate()){
        _HTMLException[HTMLException::Critical] << "HtmlString:" << "parseTree parse Error html not validate !" << HTMLException::endl;
        throw _HTMLException;
    }
    
    if(_HTable){
        for(size_t i=0; i<_HTableSize; ++i){
            delete[] _HTable[i];
        }
        delete _HTable;
    }
    
    _HTable = new ssize_t*[_OpenTags];
    for (size_t is = 0; is < _OpenTags; is++) {
        _HTable[is] = new ssize_t[3]{-1,-1,-1};
    }
    
    _HTableSize=_OpenTags;
    
    bool open=false;
    size_t ip=0;
    for(size_t ii=0; ii<_DataSize; ++ii){
        switch(_Data[ii]){
            case HTMLTAG_OPEN:
                open=true;
                _HTable[ip][0]=ii;
                break;
            case HTMLTAG_TERMINATE:
                _HTable[ip][1]=ii;
                break;
            case HTMLTAG_CLOSE:
                if(!open){
                    _HTMLException[HTMLException::Error] 
                        << "HtmlString: parseTree parse Error couldn't fid opentag" 
                        << HTMLException::endl;
                    throw _HTMLException;
                }
                _HTable[ip][2]=ii;
                ++ip;
                open=false;
                break;
            default:
                break;
        }
    }
}

void libhtmlpp::HtmlString::_buildTree(){
    Console con;
    size_t i = 0;
    while(i < _HTableSize) {
        size_t epos=0;
        if(_HTable[i][1]!=-1){
            epos=_HTable[i][1];
        }else{
            epos=_HTable[i][2];
        }
        for(size_t spos=_HTable[i][0]; spos<epos; ++spos){
            if(_Data[spos]==' ' || _Data[spos]==_HTable[i][2]){
                 if(!_HtmlRootNode){
                     _HtmlRootNode = new HtmlElement();
                     char *tag=nullptr;
                     substr(_Data,&tag,_HTable[i][0]+1,spos);
                     con << tag << Console::endl;
                     delete[] tag;
                 }else{
                     _HtmlRootNode->_Parent = new HtmlElement();
                     char *tag=nullptr;
                     substr(_Data,&tag,_HTable[i][0]+1,spos);
                     con << tag << Console::endl;
                     delete[] tag;
                 }
            }
        }
        ++i;
    }
}

libhtmlpp::HtmlElement::HtmlElement(){
    _Tag=nullptr;
    _Text=nullptr;
}

libhtmlpp::HtmlElement::~HtmlElement(){
    delete[] _Tag;
    delete[] _Text;
}

libhtmlpp::HtmlPage::HtmlPage(){
    _HtmlDocument=new HtmlString();
}

libhtmlpp::HtmlPage::~HtmlPage(){
    delete _HtmlDocument;
}

void libhtmlpp::HtmlPage::addElement(HtmlElement *element){
    return;
}

const char *libhtmlpp::HtmlPage::printHtml(){
    return _HtmlDocument->c_str();
}

void libhtmlpp::HtmlPage::loadFile(const char* path){
    int fd=open(path,O_RDONLY);
    if(fd<0){
        _HTMLException[HTMLException::Critical] << "HtmlPage can't open File: "<<path << HTMLException::endl;
        throw _HTMLException;
    }
    char buf[HTML_BLOCKSIZE];
READFILE:
    ssize_t rdd=read(fd,&buf,HTML_BLOCKSIZE);
    if(rdd>0){
        _HtmlDocument->assign(buf,rdd);
        goto READFILE;
    }
    close(fd);
    _HtmlDocument->_parseTree();
    _HtmlDocument->_buildTree();
}

libhtmlpp::HtmlTable::HtmlTable() {
    _ID=nullptr;
    _Class=nullptr;
    _Style=nullptr;
    
}


libhtmlpp::HtmlTable::~HtmlTable(){
    delete[] _Style;
    delete[] _Class;
    delete[] _ID;
}

void libhtmlpp::HtmlTable::setID(const char *id){
    if(!setter(id,getlen(id),&_ID)){
        _HTMLException[HTMLException::Error] << "HtmlTable can't id: " << id << HTMLException::endl;
        throw _HTMLException;        
    }
}

void libhtmlpp::HtmlTable::setClass(const char *cname){
    if(!setter(cname,getlen(cname),&_Class)){
        _HTMLException[HTMLException::Error] << "HtmlTable can't class name: " << cname << HTMLException::endl;
        throw _HTMLException;          
    }
}

void libhtmlpp::HtmlTable::setStyle(const char *css){
    if(!setter(css,getlen(css),&_Style,":;(),+~'")){
        _HTMLException[HTMLException::Error] << "HtmlTable can't set Style: " << css << HTMLException::endl;
        throw _HTMLException;          
    }
}

//     class Row {
//     public:
//         Row &operator<<(const char *value){
//             _Data.assign(value,value+libhttppp::getlen(value));
//             return *this;
//         };
//         
//         Row &operator<<(int value){
//             char buf[255];
//             snprintf(buf,255, "%d", value); 
//             return *this << buf;
//         };
//         
//     private:
//         std::vector<char> _Data;
//         int   _Size;
//         Row(){
//             _Size=0;
//             _nextRow=nullptr;
//         };
//         ~Row(){
//             delete _nextRow;
//         };
//         Row *_nextRow;
//         friend class HtmlTable;
//     };
//     
//     
//     
//     Row &createRow(){
//         if(_firstRow!=nullptr){
//             _lastRow->_nextRow=new Row;
//             _lastRow=_lastRow->_nextRow;
//         }else{
//             _firstRow=new Row;
//             _lastRow=_firstRow;
//         }
//         return *_lastRow;
//     }
//     
//     const char *printHtml(){
//         _Buffer.clear();
//         if(_Id!=NULL)
//             _Buffer << "<table id=\"" << _Id << "\">";
//         else
//             _Buffer << "<table>";
//         for(Row *curow=_firstRow; curow; curow=curow->_nextRow){
//             _Buffer << "<tr>";
//             _Buffer.assign(curow->_Data.begin(),curow->_Data.end());
//             _Buffer << "</tr>";
//         }
//         _Buffer << "</table>";
//         _Buffer.push_back('\0');
//         return _Buffer.data();
//     }
// private:
//     std::vector<char>  _Buffer;
//     
//     Row               *_firstRow;
//     Row               *_lastRow;
// };

