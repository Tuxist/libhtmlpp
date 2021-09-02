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
    HTMLException excp;
    if(pos < _DataSize){
        excp[HTMLException::Critical] << "HtmlString: out of String";
        throw excp;
    }
    _Data[pos]=src;
}

void libhtmlpp::HtmlString::clear(){
    for(size_t i=0; i<_HTableSize; ++i){
        delete[] _HTable[i];
    }   
    delete[]   _HTable;
    delete[]   _Data;
    delete     _HtmlRootNode;
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
    Console con;
    HtmlElement *curel=_HtmlRootNode,*nextel=nullptr;
PRINTELEMENTS:
    if(curel){
            if(curel->_Child){
                curel=curel->_Child;
                con << "    <" << curel->_Tag << ">" << con.endl;
                goto PRINTELEMENTS;
            }
            
            con << "<" << curel->_Tag << ">" << con.endl;
            curel=curel->_nextElement;
            if(curel)
                con << "</" << curel->_Tag << ">" << con.endl;
            
            goto PRINTELEMENTS;
    }
    return _Data;
}

size_t libhtmlpp::HtmlString::size(){
    return _DataSize;
}
  
bool libhtmlpp::HtmlString::validate(){
    _HTableSize=0;
    size_t opentag=0,closetag=0;
    for(size_t i=0; i<_DataSize; ++i){
        switch(_Data[i]){
            case HTMLTAG_OPEN:
                ++opentag;
                break;
            case HTMLTAG_CLOSE:
                ++closetag;
                break;
            default:
                break;
        }
    }
    
    if(opentag==0 || opentag!=closetag)
        return false;  
    
    _HTableSize=closetag;
    return true;
}

void libhtmlpp::HtmlString::parse(){
    HTMLException excp;
    if(!validate()){
        excp[HTMLException::Critical] << "HtmlString: " << "parseTree parse Error html not validate !";
        throw excp;
    }
    _parseTree();
    size_t tpos=(_HTableSize-1),spos=0;
    _buildTree(&_HtmlRootNode,spos,tpos);
}


void libhtmlpp::HtmlString::_InitString(){
    _Data=nullptr;
    _DataSize=0;
    _HTable=nullptr;
    _HTableSize=0;
    _HtmlRootNode=nullptr;
}

void libhtmlpp::HtmlString::_parseTree(){
    HTMLException excp;
    if(_HTable){
        for(size_t i=0; i<_HTableSize; ++i){
            delete[] _HTable[i];
        }
        delete[] _HTable;
    }
    
    _HTable = new ssize_t*[_HTableSize];
    for (size_t is = 0; is < _HTableSize; is++) {
        _HTable[is] = new ssize_t[3];
		_HTable[is][0] = -1;
		_HTable[is][1] = -1;
		_HTable[is][2] = -1;
    }
    
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
                    excp[HTMLException::Error] << "HtmlString: parseTree parse Error couldn't fid opentag";
                    throw excp;
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

size_t libhtmlpp::HtmlString::_getTagName(size_t spos, size_t epos, char ** tagname){
    size_t anpos=0,enpos=0;
    for(size_t i=spos; i<epos; ++i){
        switch(_Data[i]){
            case('/'):
                continue;
            case('!'):
                tagname=nullptr;
                return 0;
            case('<'):
                continue;
            default:
                anpos=i;
                enpos=anpos;
                goto FINDTAGNAMEPOS;
        }
    }
    
FINDTAGNAMEPOS:
    if(enpos < epos && !(_Data[enpos]=='>' || _Data[enpos]==' ')){
        ++enpos;
        goto FINDTAGNAMEPOS;
    }
    return substr(_Data,tagname,anpos,enpos);
}

void libhtmlpp::HtmlString::_buildTree(HtmlElement **node,size_t &spos,size_t &tpos){
    Console con;
    HtmlElement *prevnode=nullptr,*rootnode=nullptr;
    char *prvname=nullptr,*tag=nullptr;
    if(tpos>0) {
        size_t tagsize=_getTagName(_HTable[tpos][0],_HTable[tpos][2],&tag);
        if(tagsize>0){
            HtmlElement *tagel = new HtmlElement();
            tagel->_Tag = tag;
            if(!*node)
                *node=tagel;
            else
                (*node)->_nextElement=tagel;
            size_t ctagsize=_getTagName(_HTable[spos][0],_HTable[spos][2],&prvname);
            if(ctagsize>0 && ncompare(tag,tagsize,prvname,ctagsize)){
               _buildTree(&tagel->_Child,++spos,--tpos);
               con << "test" << tagel->_Tag << con.endl;
            }else{
               _buildTree(&tagel->_prevElement,++spos,--tpos); 
            }
        }
    }
}

libhtmlpp::HtmlElement::HtmlElement(){
    _Tag=nullptr;
    _Text=nullptr;
    _Comment=nullptr;
    _nextElement=nullptr;
    _prevElement=nullptr;
    _Child=nullptr;
    _ID=nullptr;
    _Class=nullptr;
    _Style=nullptr;
}

libhtmlpp::HtmlElement::~HtmlElement(){
    delete[] _Tag;
    delete[] _Text;
    delete[] _Comment;
    delete[] _Style;
    delete[] _Class;
    delete[] _ID;
    delete   _Child;
    delete   _nextElement;
}

libhtmlpp::HtmlPage::HtmlPage(){
    _HtmlDocument=nullptr;
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
    delete _HtmlDocument;
    _HtmlDocument= new HtmlString();
    FileWriter fd(path);
    char buf[HTML_BLOCKSIZE];
READFILE:
    ssize_t rdd=fd.read(buf,HTML_BLOCKSIZE);
    if(rdd>0){
        char *dest;
        size_t cdd=cleannewline(buf,rdd,&dest);
        _HtmlDocument->assign(dest,cdd);
        delete[] dest;
        goto READFILE;
    }
    _HtmlDocument->parse();
}

void libhtmlpp::HtmlElement::setID(const char *id){
    HTMLException excp;
    if(!setter(id,getlen(id),&_ID)){
        excp[HTMLException::Error] << "HtmlTable can't id: " << id;
        throw excp;        
    }
}

void libhtmlpp::HtmlElement::setClass(const char *cname){
    HTMLException excp;
    if(!setter(cname,getlen(cname),&_Class)){
        excp[HTMLException::Error] << "HtmlTable can't class name: " << cname;
        throw excp;          
    }
}

void libhtmlpp::HtmlElement::setStyle(const char *css){
    HTMLException excp;
    if(!setter(css,getlen(css),&_Style,":;(),+~'")){
        excp[HTMLException::Error] << "HtmlTable can't set Style: " << css;
        throw excp;          
    }
}

libhtmlpp::HtmlTable::HtmlTable() {
}

libhtmlpp::HtmlTable::~HtmlTable(){
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

