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

#include <iostream>
#include <fstream>

#include <systempp/sysutils.h>
#include <systempp/sysexception.h>

#include "utils.h"
#include "html.h"
#include "config.h"

#define HTMLTAG_OPEN '<'
#define HTMLTAG_TERMINATE '/'
#define HTMLTAG_CLOSE '>'

#define HTMLELEMENT 0
#define HTMLTERMELEMENT 1
#define HTMLCOMMENT 2
#define HTMLHEADER 3

libhtmlpp::HtmlString::HtmlString(){
    _InitString();
    _HtmlHeader=nullptr;
}

libhtmlpp::HtmlString::HtmlString(char *header){
    _InitString();
    _HtmlHeader=new char[sys::getlen(header)+1];
    sys::scopy(header,header+sys::getlen(header)+1,_HtmlHeader);
}

libhtmlpp::HtmlString::~HtmlString(){
   clear();
   delete[] _HtmlHeader;
}

void libhtmlpp::HtmlString::assign(const char* src, size_t srcsize){
    _Data.insert(_Data.end(),src,src+srcsize);
}

void libhtmlpp::HtmlString::push_back(const char src){
    _Data.push_back(src);
}

void libhtmlpp::HtmlString::assign(const char* src) {
    assign(src,sys::getlen(src)+1);
}

void libhtmlpp::HtmlString::insert(size_t pos, char src){
    _Data[pos]=src;
}

void libhtmlpp::HtmlString::clear(){
    _Data.clear();
    for(size_t i=0; i<_HTableSize; ++i){
        delete[] _HTable[i];
    }
    delete[]   _HTable;
    delete     _HtmlRootNode;
    _InitString();
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator+=(const char *src){
    assign(src);
    return *this;
}

libhtmlpp::HtmlString & libhtmlpp::HtmlString::operator+=(libhtmlpp::HtmlString& hstring){
    this->assign(hstring.c_str());
    return *this;
}


libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator=(const char *src){
    _Data.clear();
    assign(src);
    return *this;
}

const char libhtmlpp::HtmlString::operator[](size_t pos){
    return _Data[pos];
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(const char* src){
    assign(src);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(int src){
    char *buf=new char[sizeof(int)+1];
    sys::itoa(src,buf);
    assign(buf);
    delete[] buf;
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(unsigned long src){
    char *buf=new char[sizeof(int)+1];
    sys::ultoa(src,buf);
    assign(buf);
    delete[] buf;
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(char src){
    push_back(src);
    return *this;
}

const char *libhtmlpp::HtmlString::c_str() {
    size_t level=0;
    _printHtml(_HtmlRootNode,level);
    return reinterpret_cast<const char*>(_Data.data());
}

void libhtmlpp::HtmlString::_printHtml(libhtmlpp::HtmlElement* node,size_t &level){
    if(node){
            for(size_t i=0; i<level; ++i){
                std::cout << " ";
            }
            std::cout << "<" << node->_Tag << ">"
                                         << std::endl;
            
            if(node->_Child){
                size_t lvl=level+2;
                _printHtml(node->_Child,lvl);
            }
            
            for(size_t i=0; i<level; ++i){
                std::cout << " ";
            }
            std::cout << "</" << node->_Tag << ">" 
                                         << std::endl;
            if(node->_nextElement)
                _printHtml(node->_nextElement,level);
    }           
}


size_t libhtmlpp::HtmlString::size(){
    return _Data.size();
}
  
bool libhtmlpp::HtmlString::validate(){
    _HTableSize=0;
    size_t opentag=0,closetag=0;
    for(size_t i=0; i<_Data.size(); ++i){
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
   
    delete[] _HtmlHeader;
    _HtmlHeader=nullptr;
    delete _HtmlRootNode;
    ssize_t pos = 0;
    _HtmlRootNode=_serialzeElements(nullptr,pos);
    HtmlElement *parent=nullptr;
    _buildTree(_HtmlRootNode,parent,--pos);
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlString::_serialzeElements(HtmlElement *prevnode,ssize_t &pos){
    HTMLException excp;
    char *tag=nullptr;
    HtmlElement *tagel=nullptr;
    if(pos<_HTableSize) {
        size_t tagsize=0;
        int ret=_serialzeTags(_HTable[pos][0],_HTable[pos][2],&tag,tagsize);
        if(tagsize<=0)
            throw excp[HTMLException::Error] << "That shouldn't happend: "<< tag;
        if(ret==HTMLHEADER){
            if(_HtmlHeader)
                throw excp[HTMLException::Error] << "Htmlheader exist too often: "<< tag;
            _HtmlHeader = new char[tagsize+1];
            sys::scopy(tag,tag+(tagsize+1),_HtmlHeader);
            return _serialzeElements(prevnode,++pos);
        }else if(ret==HTMLELEMENT){
            if(prevnode){
                    tagel= new HtmlElement;
                    tagel->_Tag = tag;
                    tagel->_prevElement=prevnode;
                    tagel->_nextElement=_serialzeElements(tagel,++pos);
                    return tagel;
            }else{
                tagel = new HtmlElement;
                tagel->_Tag = tag;
                tagel->_nextElement=_serialzeElements(tagel,++pos);
                return tagel;
            }
        }else if(ret==HTMLCOMMENT){
            if(!prevnode)
                throw excp[HTMLException::Error] << "Misplaced comment: "<< tag;
            prevnode->setComment(tag);
            return _serialzeElements(prevnode,++pos);
        }
        return _serialzeElements(prevnode,++pos);
    }
    return nullptr;
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlString::_buildTree(HtmlElement *node,HtmlElement *parent,ssize_t &pos){
    char *tag=nullptr;
    if(pos>0) {
        size_t tagsize=0;
        int ret=_serialzeTags(_HTable[pos][0],_HTable[pos][2],&tag,tagsize);
        if(node){
            if(ret==HTMLELEMENT){
                if(sys::ncompare(node->_Tag,sys::getlen(node->_Tag),
                                         tag,tagsize)==0){
                    node->_nextElement=parent;
                }
            }else if(ret==HTMLTERMELEMENT){
                if(!node->_Child && sys::ncompare(node->_Tag,sys::getlen(node->_Tag),
                                                          tag,tagsize)==0){
                    parent=node->_nextElement;
                    node->_nextElement=nullptr;
                    node->_Child=_buildTree(node,parent,--pos);
                    return node;
                }else{
                    return _buildTree(node->_nextElement,parent,pos);
                }
            }
        }
        delete[] tag;
        return _buildTree(node,parent,--pos);
    }
    return nullptr;
}


void libhtmlpp::HtmlString::_InitString(){
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
    for(size_t ii=0; ii<_Data.size(); ++ii){
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

int libhtmlpp::HtmlString::_serialzeTags(size_t spos, size_t epos, char **value,size_t &valuesize){
    size_t anpos=0,enpos=0,i=spos;
    int term=-1;
    std::string Doctype;
//     while(i<epos){
//         switch(_Data[i]){
//             case('!'):
//                 if(_Data[i+1]=='-' && _Data[i+2]=='-'){ 
//                     i+=2;
//                     goto FINDCOMMENTEND;
//                 }else if(libsystempp::ncompare(_Data+i,7,"DOCTYPE",7)==0){
//                     ++i;
//                     goto FINDHTMLHEADER;
//                 }
//             case('/'):
//                 term=i;
//                 ++i;
//                 anpos=i;
//                 continue;
//             case '"':
//                 for(; _Data[i]!='"'; ++i);
//                 continue;
//             case('<'):
//                 anpos=++i;
//                 enpos=i;
//                 continue;
//             default:
//                 goto FINDTAGNAMEPOS;
//         }
//     }
// FINDHTMLHEADER:
//     if(enpos < epos && _Data[++enpos]!='>')
//         goto FINDHTMLHEADER;
//     valuesize=libsystempp::substr(_Data,value,anpos,(enpos-anpos));
//     return HTMLHEADER;
// FINDCOMMENTEND:
//     if(enpos < epos && !(_Data[enpos]=='!' && _Data[++enpos]=='-' &&
//         _Data[++enpos]=='-' && _Data[++enpos]=='>')
//     ){
//         ++enpos;
//         goto FINDCOMMENTEND;
//     }
//     valuesize=libsystempp::substr(_Data,value,anpos,(enpos-anpos));
//     return HTMLCOMMENT;
// FINDTAGNAMEPOS:
//     if(enpos < epos && !(_Data[enpos]==' ' || _Data[enpos]=='>')){
//         ++enpos;
//         if(enpos > 0 && _Data[enpos]!='/'){
//             goto FINDTAGNAMEPOS;
//         }
//     }
//     valuesize=libsystempp::substr(_Data,value,anpos,(enpos-anpos));
//     if(term >0 && term<enpos)
//         return HTMLTERMELEMENT;
//     return HTMLELEMENT;
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
    std::fstream fs;
    try{
        fs.open(path,std::ios::in);
    }catch(sys::SystemException &e){
        HTMLException excp;
        throw excp[HTMLException::Critical] << e.what();
    }
    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);
    
    char *buf=new char[length];
    if(fs.read(buf,HTML_BLOCKSIZE)){
        char *dest;
        size_t cdd=sys::cleannewline(buf,length,&dest);
        _HtmlDocument->assign(dest,cdd);
        delete[] dest;
        _HtmlDocument->parse();
    }
    HTMLException excp;
    throw excp[HTMLException::Critical] << "Could not read file";
}

void libhtmlpp::HtmlElement::setID(const char *id){
    HTMLException excp;
    if(!setter(id,sys::getlen(id),&_ID)){
        excp[HTMLException::Error] << "HtmlElement can't id: " << id;
        throw excp;        
    }
}

void libhtmlpp::HtmlElement::setClass(const char *cname){
    HTMLException excp;
    if(!setter(cname,sys::getlen(cname),&_Class)){
        excp[HTMLException::Error] << "HtmlElement can't class name: " << cname;
        throw excp;          
    }
}

void libhtmlpp::HtmlElement::setStyle(const char *css){
    HTMLException excp;
    if(!setter(css,sys::getlen(css),&_Style,":;(),+~'")){
        excp[HTMLException::Error] << "HtmlElement can't set Style: " << css;
        throw excp;          
    }
}

void libhtmlpp::HtmlElement::setComment(const char* comment){
    HTMLException excp;
    if(!setter(comment,sys::getlen(comment),&_Style,":;(),+~'")){
        excp[HTMLException::Error] << "HtmlElement can't set Comment: " << comment;
        throw excp;          
    }    
}


libhtmlpp::HtmlTable::HtmlTable() {
}

libhtmlpp::HtmlTable::~HtmlTable(){
}
