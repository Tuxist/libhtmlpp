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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <systempp/sysconsole.h>
#include <systempp/sysfile.h>
#include <systempp/sysutils.h>
#include <systempp/sysexception.h>
#include <systempp/config.h>

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
}

libhtmlpp::HtmlString::~HtmlString(){
   clear();
}

void libhtmlpp::HtmlString::assign(const char* src, size_t srcsize){
    _Data.write(src,srcsize);
}

void libhtmlpp::HtmlString::push_back(const char src){
    _Data.push_back(src);
}

void libhtmlpp::HtmlString::assign(const char* src) {
    assign(src,strlen(src));
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
    char buf[255];
    snprintf(buf,255,"%d",src);
    assign(buf);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(unsigned int src){
    char buf[255];
    snprintf(buf, 255, "%ul", src);
    assign(buf);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(unsigned long src){
    char buf[255];
    snprintf(buf, 255, "%u", src);
    assign(buf);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator<<(char src){
    push_back(src);
    return *this;
}

const char *libhtmlpp::HtmlString::c_str() {
    _Cstr.clear();
    _Cstr = _RootNode->printHtmlElement();
    return _Cstr.c_str();
}


size_t libhtmlpp::HtmlString::size(){
    return _Data.size();
}

size_t libhtmlpp::HtmlString::length() {
    return _Data.length();
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlString::parse(){
    HTMLException excp;
    _parseTree();
    ssize_t pos = 0;
    delete[] _RootNode;
    _RootNode = _buildTree(pos);
    return _RootNode;
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlString::_buildTree(ssize_t &pos){
    struct el {
        sys::array<char> data;
        HtmlElement*     elhtml;
        bool             terminator;
        struct el*       nextel;
        struct el*       prevel;

        el() {
            nextel = nullptr;
            prevel = nullptr;
            elhtml = nullptr;
            terminator = false;
        }

        ~el() {
            if(nextel)
                delete nextel;
        }

    }*firstEl=nullptr,*lastEl=nullptr,*before=nullptr;

    int startpos=0;

    for (int i = 0; i < _HTableSize; ++i) {
            if (!firstEl) {
                firstEl = new el;
                lastEl = firstEl;
            }else {

                lastEl->nextel = new el;
                lastEl->nextel->prevel = lastEl;
                lastEl = lastEl->nextel;
            }

            lastEl->data = _Data.substr((_HTable[i][0]+1),(_HTable[i][2]-(_HTable[i][0]+1)));

            if (lastEl->data[0] == '/')
                lastEl->terminator = true;

            _serialelize(lastEl->data, &lastEl->elhtml);
    }

    int lvl = 0;
    
    HtmlElement* firsthel = nullptr, * lasthel = nullptr;

    for (el* curel = firstEl; curel; curel = curel->nextel) {
        if (curel->terminator) {
            for (el* parent = curel; parent; parent=parent->prevel) {
                if (parent->elhtml->_TagName == curel->elhtml->_TagName) {
                    parent->elhtml->_Child = parent->elhtml->_nextElement;
                    parent->elhtml->_nextElement = nullptr;
                }
            }
        } else {
            if (lasthel) {
                lasthel->_nextElement = curel->elhtml;
                lasthel = lasthel->_nextElement;
            }else {
                firsthel=curel->elhtml;
                lasthel = firsthel;
            }            
        }
    }

    delete firstEl;

    return firsthel;
}

void libhtmlpp::HtmlString::_serialelize(sys::array<char> in, HtmlElement **out) {
    int i,s=0;

    for (i = 0; i < in.length(); ++i) {
        if(in[i] == '/'){
            ++s;
            continue;
        }else if (in[i] == ' ') {
            *out = new HtmlElement(in.substr(s, i-s).c_str());
            break;
        }else if (i == (in.length()-1)){
            *out = new HtmlElement(in.substr(s, i-s).c_str());
            break;
        }else if (in.substr(i,(i+3)-i) == "!--") {
            (*out) = new HtmlComment(in.substr(in.length() - 1).c_str());
            return;
        }
    }

    if (!*out) {
        HTMLException excp;
        throw excp[HTMLException::Critical] << "no tag in element found!";
    }

    int startpos = -1;
    sys::array<char> key;
    int vst=-1;
    bool comment = false;
    while (i <= in.length()) {
        switch (in[i]) {
            case ' ': {
                break;
            }
            case '=': {
                if (startpos > 0) {
                    key = in.substr(startpos, i - startpos);
                }
                break;
            }
            case '\"': {
                if (!key.empty() && vst < 0) {
                    vst = i;
                    ++vst;
                }else if(vst > 0) {
                    (*out)->setAttribute(key.c_str(), in.substr(vst, i - vst).c_str());
                    key.clear();
                    vst = -1;
                    startpos = -1;
                }
                break;
            }
            default: {
                if (startpos == -1) {
                    startpos = i;
                }
                break;
           }
        }
        ++i;
    }
}

void libhtmlpp::HtmlString::_InitString(){
    _HTable=nullptr;
    _HTableSize=0;
    _RootNode=nullptr;
}

void libhtmlpp::HtmlString::_parseTree(){
    HTMLException excp;
    if(_HTable){
        for(size_t i=0; i< _HTableSize; ++i){
            delete[] _HTable[i];
        }
        delete[] _HTable;
    }

    size_t opentag = 0, closetag = 0;
    for (size_t i = 0; i < _Data.size(); ++i) {
        switch (_Data[i]) {
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

    if (opentag == 0 || opentag != closetag) {
        HTMLException excp;
        throw excp[HTMLException::Critical] << "Wrong formated Document!";
    }


    _HTableSize = closetag;

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

libhtmlpp::HtmlElement::HtmlElement(const char *tagname){
    _nextElement=nullptr;
    _prevElement=nullptr;
    _Child=nullptr;
    _firstAttr = nullptr;
    _lastAttr = nullptr;
    _TagName = tagname;
}

libhtmlpp::HtmlElement::~HtmlElement(){
    delete   _firstAttr;
    delete   _nextElement;
    delete   _Child;
}

libhtmlpp::HtmlPage::HtmlPage(){
    _RootNode=nullptr;
}

libhtmlpp::HtmlPage::~HtmlPage(){
    delete _RootNode;
}

void libhtmlpp::HtmlPage::addElement(HtmlElement *element){
    return;
}

const char *libhtmlpp::HtmlPage::printHtml(){
    return _RootNode->printHtmlElement();
}

void libhtmlpp::HtmlPage::loadFile(const char* path){
    delete _RootNode;
    HtmlString tmp;
    sys::file fs;
    try{
        fs.open(path,0);
    }catch(sys::SystemException &e){
        HTMLException excp;
        throw excp[HTMLException::Critical] << e.what();
    }

    size_t readed = 0;
    while (readed < fs.getsize()) {
        char buf[HTML_BLOCKSIZE+1];
        int rd = fs.read(buf,HTML_BLOCKSIZE);
        readed += rd;
        if (rd > 0) {
            buf[rd] = '\0';
            tmp.assign(buf,rd);
        } else {
            HTMLException excp;
            throw excp[HTMLException::Critical] << "Could not read file";
        }
    }
    _RootNode=tmp.parse();
}

void libhtmlpp::HtmlElement::_print(HtmlElement* child) {
    for (HtmlElement* cur = child; cur; cur = cur->_nextElement) {
        if (cur->_Child) {
            _Cstr.append("<");
            _Cstr.append(cur->_TagName.c_str());
            for (HtmlElement::HtmlAttributes* curattr = cur->_firstAttr; curattr; curattr = curattr->_nextAttr) {
                _Cstr.append(" ");
                _Cstr.append(curattr->_Key.c_str());
                _Cstr.append("\"=");
                _Cstr.append(curattr->_Value.c_str());
            }
            _Cstr.append(">");
            _print(cur->_Child);
            _Cstr.append("</");
            _Cstr.append(cur->_TagName.c_str());
            _Cstr.append(">");
            return;
        }
        _Cstr.append("<");
        _Cstr.append(cur->_TagName.c_str());
        for (HtmlElement::HtmlAttributes* curattr = cur->_firstAttr; curattr; curattr = curattr->_nextAttr) {
            _Cstr.append(" ");
            _Cstr.append(curattr->_Key.c_str());
            _Cstr.append("\"=");
            _Cstr.append(curattr->_Value.c_str());
        }
        _Cstr.append(">");
    }
}

const char* libhtmlpp::HtmlElement::printHtmlElement() {
    _Cstr.clear();
    _print(this);
    return _Cstr.c_str();
}

void libhtmlpp::HtmlElement::setAttribute(const char* name, const char* value) {
    HtmlAttributes* cattr = nullptr;
    for (HtmlAttributes* curattr = _firstAttr; curattr; curattr=curattr->_nextAttr) {
        if (curattr->_Key == name) {
            cattr = curattr;
        }
    }
    if (!cattr) {
        if (_lastAttr){
            _lastAttr->_nextAttr = new HtmlAttributes();
            _lastAttr = _lastAttr->_nextAttr;
        }else {
            _firstAttr = new HtmlAttributes();
            _lastAttr = _firstAttr;
        }
        cattr = _lastAttr;
    }
    cattr->_Key = name;
    cattr->_Value = value;
}

void libhtmlpp::HtmlElement::setIntAttribute(const char* name, int value) {

}

const char* libhtmlpp::HtmlElement::getAtributte(const char* name) {
    for (HtmlAttributes* curattr = _firstAttr; curattr; curattr = curattr->_nextAttr) {
        if (curattr->_Key == name) {
            return curattr->_Value.c_str();
        }
    }
}

int libhtmlpp::HtmlElement::getIntAtributte(const char* name) {

}

libhtmlpp::HtmlElement::HtmlAttributes::HtmlAttributes() {
    _nextAttr = nullptr;
}

libhtmlpp::HtmlElement::HtmlAttributes::~HtmlAttributes() {
    delete _nextAttr;
}

libhtmlpp::HtmlComment::HtmlComment(const char* comment) : HtmlElement("!--") {

}

libhtmlpp::HtmlComment::~HtmlComment() {

}

const char* libhtmlpp::HtmlComment::printHtmlElement() {
    return _Cstr.c_str();
};

libhtmlpp::HtmlTable::HtmlTable() : HtmlElement("table") {
}

libhtmlpp::HtmlTable::~HtmlTable(){
}

libhtmlpp::HtmlDivLayer::HtmlDivLayer() : HtmlElement("div") {

}

libhtmlpp::HtmlDivLayer::~HtmlDivLayer(){

}