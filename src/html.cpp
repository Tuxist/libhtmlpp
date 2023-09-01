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
        libhtmlpp::Element*     element;
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

libhtmlpp::HtmlString::HtmlString(const libhtmlpp::HtmlString& str){
    _Data=str._Data;
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
    _RootNode = (HtmlElement*)_buildTree(pos);
    return _RootNode;
}

libhtmlpp::DocElements *libhtmlpp::HtmlString::_buildtreenode(DocElements* prev,libhtmlpp::DocElements* next,libhtmlpp::DocElements* start,libhtmlpp::DocElements* end){
    auto checkterminator = [end](DocElements *termel){
        int i=0;
        for (DocElements* curcel=termel->nextel; curcel!=end; curcel=curcel->nextel) {

            if (curcel->element->_Type==HtmlEl && !curcel->terminator &&
                ((HtmlElement*)curcel->element)->_TagName == ((HtmlElement*)termel->element)->_TagName) {
                ++i;
            }

            if (curcel->element->_Type==HtmlEl && curcel->terminator &&
                ((HtmlElement*)curcel->element)->_TagName == ((HtmlElement*)termel->element)->_TagName) {
                if(i==0)
                    return curcel;
                else
                    --i;
            }
        }
        return (DocElements*) nullptr;
    };

    if(!start->terminator){
        if(prev){
            start->element->_prevElement=prev->element;
            prev=start;
        }
    }

    if(start==end)
        return end;

    if(!start->terminator && start->element->_Type==HtmlEl){
        DocElements *parent=checkterminator(start);
        if(parent && next!=parent){
            ((HtmlElement*)start->element)->_childElement =next->element;
            next=_buildtreenode(nullptr,next->nextel,next,parent);
        }
    };

    while(next!=end){
        if(!next->terminator){
            start->element->_nextElement=next->element;
            break;
        }
        next=next->nextel;
    }

    return _buildtreenode(prev,next->nextel,next,end);
}


libhtmlpp::Element* libhtmlpp::HtmlString::_buildTree(ssize_t& pos) {
    DocElements *firstEl = nullptr, *lastEl = nullptr;

    auto addelement = [](DocElements **first,DocElements **last){
        if (!*first) {
            *first = new DocElements;
            *last = *first;
        } else {
            (*last)->nextel = new DocElements;
            (*last)->nextel->prevel = *last;
            *last = (*last)->nextel;
        }
    };

	for (size_t i = 0; i < _HTableSize; ++i) {
		addelement(&firstEl,&lastEl);

        lastEl->spos = _HTable[i][0];
        lastEl->epos = _HTable[i][2];

		if (_HTable[i][1] != -1){
			lastEl->terminator = true;
        }

        _serialelize(_Data.substr(lastEl->spos,(lastEl->epos - lastEl->spos)+1),
                     (HtmlElement**) &lastEl->element);

        if((i+1) < _HTableSize){
            int tlen = (_HTable[i+1][0]-_HTable[i][2]);
            if(tlen>1){
                addelement(&firstEl,&lastEl);
                lastEl->element=new TextElement();
                lastEl->spos = _HTable[i][2]+1;
                lastEl->epos = _HTable[i+1][0]-1;
                ((TextElement*) lastEl->element)->_Text=_Data.substr(lastEl->spos,tlen-1);
                lastEl->terminator=false;
            }
        }

    }
    
    Element* firsthel = firstEl->element;

    _buildtreenode(nullptr,firstEl->nextel,firstEl,lastEl);

    delete firstEl;

    return firsthel;
}

void libhtmlpp::HtmlString::_serialelize(std::string in, libhtmlpp::HtmlElement **out) {
    size_t st=0,et=0;

    for (size_t i = 0; i < in.length(); ++i) {
        switch (in[i]) {
            case '<':
                continue;
            case '/':
                continue;
            case ' ':
                continue;
            default:
                st=i;
                goto GETTAGEND;
        }
    }

    GETTAGEND:
    for(et=st; et<in.length(); ++et){
        if(in[et]==' ' || in[et]=='/' || in[et]=='>' ){
            break;
        }
    }

    *out = new HtmlElement(in.substr(st,et-st).c_str());

    if (!*out) {
        HTMLException excp;
        throw excp[HTMLException::Critical] << "no tag in element found!";
    }

    int startpos =-1,vst=-1;
    bool value=false;
    std::string key;

    while(et < in.length()) {
        if(in[et]==' ' || in[et]=='>' || in[et]=='=') {
            if(startpos!=-1 && !value){
                key=in.substr(startpos,et-startpos);
                (*out)->setAttribute(key.c_str(),nullptr);
                startpos=-1;
            }
            if(in[et]=='='){
                value=true;
            }
        }else if(in[et]=='\"') {
            if( vst==-1 ){
                vst=et;
            }else if(!key.empty()){
                (*out)->setAttribute(key.c_str(),in.substr(vst+1,(et-vst)-1).c_str());
                key.clear();
                vst=-1;
                value=false;
            }
        }else{
            if(startpos==-1 && !value){
                startpos=et;
            }
        };
        ++et;
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
    bool pterm=false;
    size_t ip=0;
    for(size_t ii=0; ii<_Data.length(); ++ii){
        switch(_Data[ii]){
            case HTMLTAG_OPEN:
                if (_Data.substr(ii, 4) != "<!--") {
                    open = true;
                    pterm = true;
                    _HTable[ip][0] = ii;
                }else{
                    ii+=4;
                }
                break;
            case HTMLTAG_TERMINATE:
                if(pterm==true)
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
            case ' ':
                break;
            default:
                pterm=false;
                break;
        }
    }
}

libhtmlpp::HtmlElement::HtmlElement(const char *tagname){
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

void libhtmlpp::HtmlElement::insertChild(libhtmlpp::Element* el){
    if(_childElement){
        delete _childElement;
    }
    _childElement=el;
}

void libhtmlpp::HtmlElement::appendChild(libhtmlpp::Element* el){
    if(_childElement){
        Element *curel=_childElement;
        while(curel->_nextElement){
            curel=curel->nextElement();
        }
        curel->_nextElement=el;
    }else{
        _childElement=el;
    }
}


void libhtmlpp::Element::insertBefore(libhtmlpp::Element* el){
    _prevElement->_nextElement=el;
    Element *nexel;
    do{
        nexel=el->_nextElement;
    }while(nexel!=nullptr);

    nexel->_nextElement=_nextElement;
}

void libhtmlpp::Element::insertAfter(libhtmlpp::Element* el){
    el->_nextElement=_nextElement;
    _nextElement=el;
}

libhtmlpp::Element *libhtmlpp::Element::nextElement(){
    return _nextElement;
}

libhtmlpp::Element *libhtmlpp::Element::prevElement(){
    return _prevElement;
}

int libhtmlpp::Element::getType(){
    return _Type;
}


libhtmlpp::HtmlPage::HtmlPage(){
}

libhtmlpp::HtmlPage::~HtmlPage(){
}

libhtmlpp::HtmlString *libhtmlpp::HtmlPage::loadFile(const char* path){
    _Page.clear();
    char tmp[HTML_BLOCKSIZE];
    std::ifstream fs;
    try{
        fs.open(path);
    }catch(std::exception &e){
        HTMLException excp;
        throw excp[HTMLException::Critical] << e.what();
    }

    while (fs.good()) {
        fs.read(tmp,HTML_BLOCKSIZE);
        _Page.append(tmp,fs.gcount());
    }
    fs.close();
    _CheckHeader();
    return &_Page;
}

libhtmlpp::HtmlString *libhtmlpp::HtmlPage::loadString(const std::string src){
    _Page.clear();
    _Page << src;
    _CheckHeader();
    return &_Page;
}

libhtmlpp::HtmlString *libhtmlpp::HtmlPage::loadString(HtmlString node){
    _Page.clear();
    _Page=node;
    _CheckHeader();
    return &_Page;
}

void libhtmlpp::HtmlPage::saveFile(const char* path){
}

void libhtmlpp::HtmlPage::_CheckHeader(){
    const char type[] = { '!','D','O','C','T','Y','P','E' };
    int i = 0;

    while (i < 8) {
        if (_Page[i+1] != type[i]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "No Doctype found arborting";
            throw excp;
        }
        ++i;
    }

    do{
        ++i;
    } while (_Page[i] == ' ');

    const char typevalue[] = { 'h','t','m','l' };
    size_t tpvl = 4;

    if ((i + tpvl) > _Page.length()) {
        HTMLException excp;
        excp[HTMLException::Critical] << "Doctype header broken or wrong type";
        throw excp;
    }

    int ii = 0,ie=i+tpvl;

    while (i < ie) {
        if (_Page[i] != typevalue[ii]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "wrong Doctype";
            throw excp;
        }
        ++i;
        ++ii;
    }
}


void libhtmlpp::print(Element* el, HtmlElement* parent,std::string& output) {
    switch(el->_Type){
        case HtmlEl:{
            output.append("<");
            output.append(((HtmlElement*) el)->_TagName);
            for (HtmlElement::Attributes* curattr = ((HtmlElement*) el)->_firstAttr; curattr; curattr = curattr->_nextAttr) {
                output.append(" ");
                output.append(curattr->_Key);
                if(!curattr->_Value.empty()){
                    output.append("=\"");
                    output.append(curattr->_Value);
                    output.append("\"");
                }else{
                    output.append(" ");
                }
            }
            output.append(">");

            if (((HtmlElement*) el)->_childElement) {
                print((Element*)((HtmlElement*) el)->_childElement, (HtmlElement*)el, output);
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
            }else if(parent){
                output.append("</");
                output.append(parent->_TagName);
                output.append(">");
            }
        }break;

        default:
            HTMLException excp;
            excp[HTMLException::Error] << "Unkown Elementtype";
            throw excp;
            break;
    }
}

libhtmlpp::HtmlElement * libhtmlpp::getRootNode(libhtmlpp::Element* el){
    for(Element *curel=el; curel; curel=curel->nextElement()){
        if(curel->getType()==HtmlEl){
            if(strcmp(((libhtmlpp::HtmlElement*)curel)->getTagname(),"html")==0){
                return (libhtmlpp::HtmlElement*)curel;
            }
        }
    }
    return nullptr;
}

const char * libhtmlpp::HtmlElement::getTagname(){
    return _TagName.c_str();
}


libhtmlpp::HtmlElement *libhtmlpp::HtmlElement::getElementbyID(const char *id){
    for(Element *curel=this; curel; curel=curel->nextElement()){
        if(curel->getType()==HtmlEl){
            if(((HtmlElement*)curel)->_childElement){
                HtmlElement *find=((HtmlElement*)((HtmlElement*)curel)->_childElement)->getElementbyID(id);
                if(find)
                    return find;
            }
            const char *key=((HtmlElement*)curel)->getAtributte("id");
            if(key && strcmp(key,id)==0){
                return (HtmlElement*)curel;
            }
        }
    }
    return nullptr;
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
    if(value)
        cattr->_Value = value;
    else
        cattr->_Value .clear();
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

libhtmlpp::TextElement::TextElement(){
    _Type=TextEl;
}

libhtmlpp::TextElement::~TextElement(){
}

libhtmlpp::HtmlTable::HtmlTable(){
    _firstRow=nullptr;
    _lastRow=nullptr;
}

libhtmlpp::HtmlTable::~HtmlTable(){
}

void libhtmlpp::HtmlTable::insert(libhtmlpp::Element* element){
}

void libhtmlpp::HtmlTable::parse(libhtmlpp::Element* element){
}

libhtmlpp::HtmlTable::Column::Column(){
    _nextColumn=nullptr;
}

libhtmlpp::HtmlTable::Column::~Column(){
    delete _nextColumn;
}

libhtmlpp::HtmlTable::Column::Column(const libhtmlpp::HtmlTable::Column& col){
    Data = col.Data;
}


libhtmlpp::HtmlTable::Row::Row(){
    _firstColumn=nullptr;
    _lastColumn=nullptr;
    _nextRow=nullptr;
    _count=0;
}

libhtmlpp::HtmlTable::Row::~Row(){
    delete _firstColumn;
    delete _nextRow;
}

libhtmlpp::HtmlTable::Column & libhtmlpp::HtmlTable::Row::operator<<(libhtmlpp::HtmlString value){
    if(_firstColumn){
        _lastColumn->_nextColumn=new Column;
        _lastColumn=_lastColumn->_nextColumn;
    }else{
        _firstColumn= new Column;
        _lastColumn=_firstColumn;
    }
    _lastColumn->Data=value;
    ++_count;
    return *_lastColumn;
}


libhtmlpp::HtmlTable::Column &libhtmlpp::HtmlTable::Row::operator<<(const char* value){
    HtmlString buf;
    buf << value;
    return *this << buf;
}

libhtmlpp::HtmlTable::Column & libhtmlpp::HtmlTable::Row::operator<<(int value){
    char buf[255];
    snprintf(buf,255,"%d",value);
    return *this << buf;
}

libhtmlpp::HtmlTable::Row &libhtmlpp::HtmlTable::operator<<(const libhtmlpp::HtmlTable::Column col){
    return *_lastRow;
}

libhtmlpp::HtmlTable::Column & libhtmlpp::HtmlTable::Row::operator[](int pos){
    if(!_firstColumn || _count<pos){
        libhtmlpp::HTMLException exp;
        exp[HTMLException::Error] << "HtmlTable: Row at this position won't exists !";
        throw exp;
    }
    size_t cpos=0;
    Column *curel=nullptr;
    for(curel=_firstColumn; curel; curel=curel->_nextColumn){
        if(cpos==pos)
            return *curel;
        ++cpos;
    }
    return *curel;
}


void libhtmlpp::HtmlTable::Row::setHeader(const libhtmlpp::HtmlTable::Column col){
    _header = col;
}


