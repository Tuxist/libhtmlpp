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
#include <fstream>

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

namespace libhtmlpp {
    class DocElements {
    public:
        union {
            libhtmlpp::HtmlElement* elhtml;
            libhtmlpp::TextElement* eltext;
        } element;
        bool                    terminator;
        class DocElements*      nextel;
        class DocElements*      prevel;
        int                     spos;
        int                     epos;

        DocElements() {
            nextel = nullptr;
            prevel = nullptr;
            terminator = false;
            spos = 0;
            epos = 0;
        }

        ~DocElements() {
            if (nextel)
                delete nextel;
        }
    };
};

libhtmlpp::HtmlString::HtmlString(){
    _InitString();
}

libhtmlpp::HtmlString::~HtmlString(){
   clear();
}

void libhtmlpp::HtmlString::append(const char* src, size_t srcsize){
    _Data.append(src,srcsize);
}

void libhtmlpp::HtmlString::push_back(const char src){
    _Data.push_back(src);
}

void libhtmlpp::HtmlString::append(const char* src) {
    append(src,strlen(src));
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
    append(src);
    return *this;
}

libhtmlpp::HtmlString & libhtmlpp::HtmlString::operator+=(libhtmlpp::HtmlString& hstring){
    _Data.append(hstring._Data);
    return *this;
}


libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator=(const char *src){
    _Data=src;
    return *this;
}

const char libhtmlpp::HtmlString::operator[](size_t pos) {
    return _Data[pos];
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(const char* src) {
    append(src);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(std::string src) {
    _Data.append(src);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(int src) {
    char buf[255];
    snprintf(buf, 255, "%d", src);
    append(buf);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(unsigned int src) {
    char buf[255];
    snprintf(buf, 255, "%ul", src);
    append(buf);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(unsigned long src) {
    char buf[255];
    snprintf(buf, 255, "%zu", src);
    append(buf);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(char src) {
    push_back(src);
    return *this;
}

const size_t libhtmlpp::HtmlString::size() {
    return _Data.size();
}

const size_t libhtmlpp::HtmlString::length() {
    return _Data.length();
}

libhtmlpp::HtmlElement* libhtmlpp::HtmlString::parse() {
    HTMLException excp;
    _parseTree();
    ssize_t pos = 0;
    delete[] _RootNode;
    _RootNode = _buildTree(pos);
    return _RootNode;
}

libhtmlpp::HtmlElement* libhtmlpp::HtmlString::_buildTree(ssize_t& pos) {
    DocElements *firstEl = nullptr, *lastEl = nullptr;

	for (size_t i = 0; i < _HTableSize; ++i) {
		if (!firstEl) {
			firstEl = new DocElements;
			lastEl = firstEl;
		}
		else {

			lastEl->nextel = new DocElements;
			lastEl->nextel->prevel = lastEl;
			lastEl = lastEl->nextel;
		}

        lastEl->spos = _HTable[i][0]+1;
        lastEl->epos = _HTable[i][2]-1;

		if (_HTable[i][1] != -1)
			lastEl->terminator = true;

        _serialelize(_Data.substr(lastEl->spos,(lastEl->epos - lastEl->spos)+1),
                     (HtmlElement**) &lastEl->element.elhtml);

        if(i+1 < _HTableSize){
            if(_HTable[i+1][1]-_HTable[i][2]<0){
                lastEl->nextel = new DocElements;
                lastEl->nextel->prevel = lastEl;
                lastEl = lastEl->nextel;
                lastEl->element.eltext=new TextElement();
                lastEl->element.eltext->_Text=_Data.substr(_HTable[i][2]+1,_HTable[i][1+1]-(_HTable[i][2]+1));
            }
        }

    }
    
    HtmlElement* firsthel = nullptr, * lasthel = nullptr;

    for (DocElements* curel = firstEl->nextel; curel; curel = curel->nextel) {
        if (!curel->terminator) {
            if (lasthel) {
                lasthel->_nextElement = curel->element.elhtml;
                lasthel->_nextElement->_prevElement = lasthel;
                lasthel = (HtmlElement*) lasthel->_nextElement;
            }else {
                firsthel=(HtmlElement*)curel->element.elhtml;
                lasthel = firsthel;
                lasthel->_prevElement = nullptr;
            }            
        }
    }

	for (DocElements* curel = firstEl; curel; curel = curel->nextel) {
        if (curel->terminator ) {
		    for (DocElements* parent = curel->prevel; parent; parent = parent->prevel) {
                if (((HtmlElement*)parent->element.elhtml)->_TagName == ((HtmlElement*)curel->element.elhtml)->_TagName) {
                    if (parent->element.elhtml->_nextElement)
                        parent->element.elhtml->_nextElement->_prevElement = nullptr;

                    ((HtmlElement*)parent->element.elhtml)->_childElement = (HtmlElement*)parent->element.elhtml->_nextElement;

                    DocElements* endcurel = nullptr;

                    while (endcurel) {
                        if (!curel->terminator) {
                            endcurel = curel;
                        }
                        curel = curel->nextel;
                    }

                    if (endcurel)
                        parent->element.elhtml->_nextElement = endcurel->element.elhtml;
                    parent->element.elhtml->_nextElement = nullptr;
                    break;
                }
		    }
        }
	}

    _buildTreeElement(firstEl,firstEl->nextel,firsthel);

    delete firstEl;

    return firsthel;
}

libhtmlpp::HtmlElement* libhtmlpp::HtmlString::_buildTreeElement(libhtmlpp::DocElements* curel,
                                                                 libhtmlpp::DocElements* nexel,
                                                                 libhtmlpp::HtmlElement* parent) {
    if (curel->element.elhtml && !((HtmlElement*)curel->element.elhtml)->_TagName.empty()) {
        if (!parent) {
            parent=(HtmlElement*)curel->element.elhtml;
        }
    }
    return nullptr;
}

void libhtmlpp::HtmlString::_serialelize(std::string in, libhtmlpp::HtmlElement **out) {
    size_t i,s=0;

    for (i = 0; i <= in.length(); ++i) {
        if(in[i] == '/'){
            ++s;
            continue;
        }else if (in[i] == ' ') {
            *out = new HtmlElement(in.substr(s, i-s).c_str());
            break;
        }else if (i == in.length()){
            *out = new HtmlElement(in.substr(s, i-s).c_str());
            break;
        }
    }

    if (!*out) {
        HTMLException excp;
        throw excp[HTMLException::Critical] << "no tag in element found!";
    }

    int startpos = -1;
    std::string key;
    int vst=-1;
    while (i < in.length()) {
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
            if (_Data.substr(i, 4) != "<!--") {
                ++opentag;
            }
            break;
        case HTMLTAG_CLOSE:
            if (_Data.substr(i - 2, 3) != "-->") {
                ++closetag;
            }
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
    for(size_t ii=0; ii<_Data.length(); ++ii){
        switch(_Data[ii]){
            case HTMLTAG_OPEN:
                if (_Data.substr(ii, 4) != "<!--") {
                    open = true;
                    _HTable[ip][0] = ii;
                }else{
                    ii+=4;
                }
                break;
            case HTMLTAG_TERMINATE:
                _HTable[ip][1]=ii;
                break;
            case HTMLTAG_CLOSE:
                if (_Data.substr(ii - 2, 3) != "-->") {
                    if (!open) {
                        excp[HTMLException::Error] << "HtmlString: parseTree parse Error couldn't fid opentag";
                        throw excp;
                    }
                    _HTable[ip][2] = ii;
                    ++ip;
                    open = false;
                }
                break;
            default:
                break;
        }
    }
}

libhtmlpp::HtmlElement::HtmlElement(const char *tagname){
    _nextElement=nullptr;
    _prevElement=nullptr;
    _childElement = nullptr;
    _firstAttr = nullptr;
    _lastAttr = nullptr;
    _Type=HtmlEl;
    if(tagname)
        _TagName = tagname;
}

libhtmlpp::HtmlElement::~HtmlElement(){
    if(_Type==HtmlEl){
        delete   _firstAttr;
        delete   _childElement;
    }
}

libhtmlpp::HtmlPage::HtmlPage(){
    _RootNode=nullptr;
}

libhtmlpp::HtmlPage::~HtmlPage(){
    delete _RootNode;
}

void libhtmlpp::HtmlPage::printHtml(std::string &html){
    html.clear();
    html.append("<!DOCTYPE html>");
    if(_RootNode)
        print(_RootNode,nullptr,html);
}

void libhtmlpp::HtmlPage::loadFile(const char* path){
    char tmp[HTML_BLOCKSIZE];
    std::ifstream fs;
    std::string fscontent;
    try{
        fs.open(path);
    }catch(std::exception &e){
        HTMLException excp;
        throw excp[HTMLException::Critical] << e.what();
    }

    while (fs.good()) {
        fs.read(tmp,HTML_BLOCKSIZE);
        fscontent.append(tmp);
    }
    fs.close();
    loadString(fscontent);
}

void libhtmlpp::HtmlPage::loadString(const std::string src){
    HtmlString buf;
    buf << src;
    loadString(buf);
}

void libhtmlpp::HtmlPage::loadString(HtmlString node){
    delete _RootNode;

    const char type[] = { '!','D','O','C','T','Y','P','E' };
    int i = 0;

    while (i < 8) {
        if (node[i+1] != type[i]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "No Doctype found arborting";
            throw excp;
        }
        ++i;
    }

    do{
        ++i;
    } while (node[i] == ' ');

    const char typevalue[] = { 'h','t','m','l' };
    size_t tpvl = 4;

    if ((i + tpvl) > node.length()) {
        HTMLException excp;
        excp[HTMLException::Critical] << "Doctype header broken or wrong type";
        throw excp;
    }

    int ii = 0,ie=i+tpvl;

    while (i < ie) {
        if (node[i] != typevalue[ii]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "wrong Doctype";
            throw excp;
        }
        ++i;
        ++ii;
    }
    _RootNode=node.parse();
}

void libhtmlpp::print(Element* el, HtmlElement* parent,std::string& output) {
    switch(el->_Type){
        case HtmlEl:{
            if (!((HtmlElement*) el)->_TagName.empty()) {
                output.append("<");
                output.append(((HtmlElement*) el)->_TagName);
                for (HtmlElement::Attributes* curattr = ((HtmlElement*) el)->_firstAttr; curattr; curattr = curattr->_nextAttr) {
                    output.append(" ");
                    output.append(curattr->_Key);
                    output.append("=\"");
                    output.append(curattr->_Value);
                    output.append("\"");
                }
                output.append(">");

                if (((HtmlElement*) el)->_childElement) {
                    print((Element*)((HtmlElement*) el)->_childElement, (HtmlElement*)el, output);
                }
            }

            if (el->_nextElement) {
                print(el->_nextElement,parent,output);
            }else if(parent){
                output.append("</");
                output.append(parent->_TagName);
                output.append(">");
            }
        }break;

        case TextEl :{
            output.append(((TextElement*)el)->_Text);

            if (el->_nextElement) {
                print(el->_nextElement,parent,output);
            }

        }break;

        default:
            break;
    }
}


void libhtmlpp::HtmlElement::setAttribute(const char* name, const char* value) {
    Attributes* cattr = nullptr;
    for (Attributes* curattr = _firstAttr; curattr; curattr=curattr->_nextAttr) {
        if (curattr->_Key == name) {
            cattr = curattr;
        }
    }
    if (!cattr) {
        if (_lastAttr){
            _lastAttr->_nextAttr = new Attributes();
            _lastAttr = _lastAttr->_nextAttr;
        }else {
            _firstAttr = new Attributes();
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
    for (Attributes* curattr = _firstAttr; curattr; curattr = curattr->_nextAttr) {
        if (curattr->_Key == name) {
            return curattr->_Value.c_str();
        }
    }
    return nullptr;
}

int libhtmlpp::HtmlElement::getIntAtributte(const char* name) {
    return atoi(getAtributte(name));
}

libhtmlpp::HtmlElement::Attributes::Attributes() {
    _nextAttr = nullptr;
}

libhtmlpp::HtmlElement::Attributes::~Attributes() {
    delete _nextAttr;
}

libhtmlpp::HtmlTable::HtmlTable() : HtmlElement("table") {
}

libhtmlpp::HtmlTable::~HtmlTable(){
}

libhtmlpp::HtmlDivLayer::HtmlDivLayer() : HtmlElement("div") {

}

libhtmlpp::HtmlDivLayer::~HtmlDivLayer(){

}

libhtmlpp::TextElement::TextElement(){
    _Type=TextEl;
    _nextElement=nullptr;
    _prevElement=nullptr;
}
