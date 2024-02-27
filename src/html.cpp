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
#include <stdarg.h>

#include <algorithm>
#include <fstream>
#include <stack>

#include "utils.h"
#include "html.h"
#include "config.h"
#include "encode.h"

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

        }
    };
};

libhtmlpp::HtmlString::HtmlString(){
    _HTable=nullptr;
    _HTableSize=0;
    _RootNode=nullptr;
}

libhtmlpp::HtmlString::HtmlString(const char* str) : HtmlString(){
    std::copy(str,str+strlen(str),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()) );
}

libhtmlpp::HtmlString::HtmlString(std::string& str) : HtmlString(){
    std::copy(str.begin(),str.end(),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));
}

libhtmlpp::HtmlString::~HtmlString(){
    if(_HTable){
        for(size_t i=0; i<_HTableSize; ++i){
            delete[] _HTable[i];
        }
    }
    delete[]   _HTable;
    delete     _RootNode;
}

libhtmlpp::HtmlString::HtmlString(const libhtmlpp::HtmlString& str) : HtmlString() {
    std::copy(str._Data.begin(),str._Data.end(),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));
}

libhtmlpp::HtmlString::HtmlString(const libhtmlpp::HtmlString* str) : HtmlString(){
    std::copy(str->_Data.begin(),str->_Data.end(),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));
}


void libhtmlpp::HtmlString::append(const char* src, size_t srcsize){
   std::copy(src,src+srcsize,std::insert_iterator<std::vector<char>>(_Data,_Data.end()));
}

void libhtmlpp::HtmlString::push_back(const char src){
   _Data.push_back(src);
}

void libhtmlpp::HtmlString::append(const char* src) {
    if(src)
        append(src,strlen(src));
}

void libhtmlpp::HtmlString::append(libhtmlpp::HtmlString& hstring) {
    std::copy(hstring._Data.begin(),hstring._Data.end(),std::insert_iterator<std::vector<char>>(_Data,_Data.end()));
}

void libhtmlpp::HtmlString::insert(size_t pos, char src){
    _Data.at(pos)=src;
}

void libhtmlpp::HtmlString::clear(){
    if(_HTable){
        for(size_t i=0; i<_HTableSize; ++i){
            delete[] _HTable[i];
        }
    }
    delete[]   _HTable;
    delete     _RootNode;

    _HTable=nullptr;
    _HTableSize=0;
    _RootNode=nullptr;
    _Data.clear();
}

bool libhtmlpp::HtmlString::empty(){
    return _Data.empty();
}


libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator+=(const char *src){
    append(src);
    return *this;
}

libhtmlpp::HtmlString & libhtmlpp::HtmlString::operator+=(libhtmlpp::HtmlString& hstring){
    append(hstring);
    return *this;
}

libhtmlpp::HtmlString &libhtmlpp::HtmlString::operator=(const char *src){
    clear();
    std::copy(src,src+strlen(src),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));

    return *this;
}

libhtmlpp::HtmlString & libhtmlpp::HtmlString::operator=(std::string *src){
    clear();
    std::copy(src->begin(),src->end(),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator=(const libhtmlpp::HtmlString& src){
    clear();
    std::copy(src._Data.begin(),src._Data.end(),std::insert_iterator<std::vector<char>>(_Data,_Data.begin()));
    return *this;
}

const char libhtmlpp::HtmlString::operator[](size_t pos) const{
    return _Data.at(pos);
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(const char* src) {
    append(src);
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(std::string &src) {
    append(src.c_str());
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(std::string *src) {
    append(src->c_str());
    return *this;
}

libhtmlpp::HtmlString& libhtmlpp::HtmlString::operator<<(HtmlString src) {
    append(src.c_str());
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

const size_t libhtmlpp::HtmlString::size() const{
    return _Data.size();
}

const char * libhtmlpp::HtmlString::c_str(){
    _CStr=_Data;
    _CStr.push_back('\0');
    return _CStr.data();
}

libhtmlpp::HtmlElement* libhtmlpp::HtmlString::parse() {
    HTMLException excp;
    _parseTree();
    ssize_t pos = 0;
    if(_RootNode)
        delete _RootNode;
    _RootNode = (HtmlElement*)_buildTree(pos);
    return _RootNode;
}

bool libhtmlpp::HtmlString::validate(std::string *err){
    try{
        _parseTree();
        ssize_t pos = 0;
        HtmlElement *el=(HtmlElement*)_buildTree(pos);
        if(el){
            delete el;
            return true;
        }
    }catch(HTMLException &e){
        *err=e.what();
    }
    return false;
}

libhtmlpp::DocElements *libhtmlpp::HtmlString::_buildtreenode(DocElements* prev,libhtmlpp::DocElements* next,libhtmlpp::DocElements* start,libhtmlpp::DocElements* end){

    struct cpyel {
        cpyel(){

        };

        cpyel(const cpyel &src){
            prev=src.prev;
            next=src.next;
            start=src.start;
            end=src.end;
        };

        libhtmlpp::DocElements *prev;
        libhtmlpp::DocElements *next;
        libhtmlpp::DocElements *start;
        libhtmlpp::DocElements *end;
    };

    std::stack<cpyel> cpylist;

    auto checkterminator = [end](DocElements *termel){
        int i=0;
        for (DocElements* curcel=termel->nextel; curcel; curcel=curcel->nextel) {

            if (curcel->element->_Type==HtmlEl && !curcel->terminator &&
                ((HtmlElement*)curcel->element) == ((HtmlElement*)termel->element)) {
                ++i;
            }

            if (curcel->element->_Type==HtmlEl && curcel->terminator &&
                ((HtmlElement*)curcel->element) == ((HtmlElement*)termel->element)) {
                if(i==0)
                    return curcel;
                else
                    --i;
            }
            if(curcel==end)
                break;
        }
        return (DocElements*) nullptr;
    };
NEXTDOCEL:
    if(!start->terminator){
        if(prev){
            start->element->_prevElement=prev->element;
            prev=start;
        }
    }

    if(start==end){
        while(!cpylist.empty()){
            cpyel cpy;
            cpy=cpylist.top();
            prev=cpy.prev;
            next->nextel=cpy.next;
            start=cpy.start;
            end=cpy.end;
            cpylist.pop();
            goto NEXTDOCEL;
        }
    }

    if(!start->terminator && start->element->_Type==HtmlEl){
        DocElements *parent=checkterminator(start);
        if(parent){
            ((HtmlElement*)start->element)->_childElement =next->element;
            cpyel cpy;
            cpy.prev=nullptr;
            cpy.next=next->nextel;
            cpy.start=next;
            cpy.end=parent;
            cpylist.push(cpy);
            prev=start;
            start=parent;
            next=parent->nextel;
            goto NEXTDOCEL;
        }
    };

    while(next!=end){
        if(!next->terminator){
            start->element->_nextElement=next->element;
            prev=start;
            start=next;
            next=next->nextel;
            goto NEXTDOCEL;
        }
        next=next->nextel;
    }

    return end;
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
        if(_HTable[i][0] == -1 || _HTable[i][2] == -1)
            continue;

        addelement(&firstEl,&lastEl);

        lastEl->spos = _HTable[i][0];
        lastEl->epos = _HTable[i][2]+1;

        if (_HTable[i][1] != -1){
            lastEl->terminator = true;
        }

        std::vector<char> el;
        std::copy(_Data.begin()+lastEl->spos,_Data.begin()+lastEl->epos,std::inserter<std::vector<char>>(el,el.begin()));

        _serialelize(el,(HtmlElement**) &lastEl->element);

        size_t epos = i+1 < _HTableSize ? _HTable[i+1][0] :  _Data.size();

        if(epos - _HTable[i][2] > 0){
            addelement(&firstEl,&lastEl);
            lastEl->element=new TextElement();
            lastEl->spos = _HTable[i][2]+1;
            lastEl->epos = epos;
            std::copy(_Data.begin()+lastEl->spos,_Data.begin()+lastEl->epos,
                        std::inserter<std::vector<char>>(((TextElement*) lastEl->element)->_Text,((TextElement*) lastEl->element)->_Text.begin()));
        }

    }

    if(!firstEl)
        return nullptr;

    Element* firsthel = firstEl->element;

    _buildtreenode(nullptr,firstEl->nextel,firstEl,lastEl);

    DocElements *dnext=nullptr,*dcurel=firstEl;
    while(dcurel){
        dnext=dcurel->nextel;
        dcurel->nextel=nullptr;
        dcurel->element=nullptr;
        delete dcurel;
        dcurel=dnext;
    }

    return firsthel;
}
#include <iostream>

void libhtmlpp::HtmlString::_serialelize(std::vector<char> in, libhtmlpp::HtmlElement **out) {
    size_t st=0,et=0;

    for (size_t i = 0; i < in.size(); ++i) {
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
    for(et=st; et<in.size(); ++et){
        if(in[et]==' ' || in[et]=='/' || in[et]=='>' ){
            break;
        }
    }

    std::vector<char> tag;

    std::copy(in.begin()+st,in.begin()+et,std::insert_iterator<std::vector<char>>(tag,tag.begin()) );
    tag.push_back('\0');

    *out = new HtmlElement(tag.data());

    if (!*out) {
        HTMLException excp;
        throw excp[HTMLException::Critical] << "no tag in element found!";
    }

    int startpos =-1,vst=-1;
    bool value=false;
    std::vector<char> key;

    while(et < in.size()) {
        if(in[et]==' ' || in[et]=='>' || in[et]=='=') {
            if(startpos!=-1 && !value){
                std::copy(in.begin()+startpos,in.begin()+et,std::insert_iterator<std::vector<char>>(key,key.begin()) );
                (*out)->setAttribute(key.data(),key.size(),nullptr,0);
                startpos=-1;
            }
            if(in[et]=='='){
                value=true;
            }
        }else if(in[et]=='\"') {
            if( vst==-1 ){
                vst=et;
            }else if(!key.empty() && value){
                std::vector<char> val;
                ++vst;
                std::copy(in.begin()+vst,in.begin()+et,std::insert_iterator<std::vector<char>>(val,val.begin()) );
                (*out)->setAttribute(key.data(),key.size(),val.data(),val.size());
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

void libhtmlpp::HtmlString::_parseTree(){
 HTMLException excp;
    if(_HTable){
        for(size_t i=0; i< _HTableSize; ++i){
            delete[] _HTable[i];
        }
        delete[] _HTable;
    }

    size_t closetag = 0;
    for (size_t i = 0; i < _Data.size(); ++i) {
        switch (_Data[i]) {
            case HTMLTAG_CLOSE:
                if (memcmp(_Data.data()+(i - 2),"-->",3)!=0) {
                    ++closetag;
                }
            break;
        default:
            break;
        }
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
    for(size_t ii=0; ii<_Data.size(); ++ii){
        switch(_Data[ii]){
            case HTMLTAG_OPEN:
                if(!open){
                    if (memcmp(_Data.data()+ii,"<!--",4) != 0) {
                        open = true;
                        pterm = true;
                        _HTable[ip][0] = ii;
                    }else{
                        ii+=3;
                    }
                }
                break;
            case HTMLTAG_TERMINATE:
                if(pterm==true)
                    _HTable[ip][1]=ii;
                break;
            case HTMLTAG_CLOSE:
                if (memcmp(_Data.data()+ii,"<--",3) !=0 && open) {
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

void libhtmlpp::HtmlEncode(const char* input, std::string &output){
    HtmlString tmp(output);
    HtmlEncode(input,&tmp);
    output=tmp.c_str();
}

void libhtmlpp::HtmlEncode(const char* input, HtmlString* output){
    size_t ilen=strlen(input);
    for(size_t i=0; i<ilen; ++i){
        size_t ii=0;
        bool changed=false;
        while(HtmlSigns[ii][0]){
            if(input[i]==HtmlSigns[ii][0][0]){
                output->append(HtmlSigns[ii][1]);
                changed=true;
            }
            ++ii;
        }
        if(!changed)
            output->push_back(input[i]);
    }
}


libhtmlpp::HtmlElement::HtmlElement(const char *tagname) : Element(){
    _childElement=nullptr;
    _firstAttr=nullptr;
    _lastAttr=nullptr;
    _Type=HtmlEl;
    std::copy(tagname,tagname+strlen(tagname),std::insert_iterator<std::vector<char>>(_TagName,_TagName.begin()) );
}

libhtmlpp::HtmlElement::HtmlElement() : Element() {
    _childElement=nullptr;
    _firstAttr=nullptr;
    _lastAttr=nullptr;
    _Type=HtmlEl;
}

libhtmlpp::HtmlElement::HtmlElement(const libhtmlpp::HtmlElement& hel) : HtmlElement(){
    _copy(this,&hel);
}

libhtmlpp::HtmlElement::HtmlElement(const libhtmlpp::HtmlElement* hel) : HtmlElement(){
    _copy(this,hel);
}

libhtmlpp::HtmlElement::~HtmlElement(){
    Attributes *cura=_firstAttr;
    while(cura){
        Attributes *next=cura->_nextAttr;
        cura->_nextAttr=nullptr;
        delete cura;
        cura=next;
    }
}

void libhtmlpp::HtmlElement::setTagname(const char* name){
    std::copy(name,name+strlen(name),std::insert_iterator<std::vector<char>>(_TagName,_TagName.begin()) );
}

const char* libhtmlpp::HtmlElement::getTagname(){
    _CStr=_TagName;
    _CStr.push_back('\0');
    return _CStr.data();
}

void libhtmlpp::HtmlElement::insertChild(libhtmlpp::Element* el){
    if(_childElement){
        delete _childElement;
        _childElement=nullptr;
    }
    if(el->getType()==HtmlEl)
        _childElement=new HtmlElement;
    else if(el->getType()==TextEl)
        _childElement=new TextElement;
    _copy(_childElement,el);
}

void libhtmlpp::HtmlElement::appendChild(libhtmlpp::Element* el){
    if(_childElement){
        Element *curel=_childElement,*prev=nullptr;
        do{
            prev=curel;
            curel=curel->nextElement();
        }while(curel);

        if(el->getType()==HtmlEl)
            curel=new HtmlElement;
        else if(el->getType()==TextEl)
            curel=new TextElement;
        if(prev){
            prev->_nextElement=curel;
            curel->_prevElement=prev;
        }
        _copy(curel,el);
    }else{
        insertChild(el);
    }
}


bool libhtmlpp::HtmlElement::operator==(const HtmlElement *hel){
    if(_TagName.size() != hel->_TagName.size())
        return false;
    if(memcmp(_TagName.data(),hel->_TagName.data(),_TagName.size())==0)
        return true;
    return false;
}

bool libhtmlpp::HtmlElement::operator==(const HtmlElement &hel){
    if(_TagName.size() != hel._TagName.size())
        return false;
    if(memcmp(_TagName.data(),hel._TagName.data(),_TagName.size())==0)
        return true;
    return false;
}

libhtmlpp::HtmlElement & libhtmlpp::HtmlElement::operator=(const libhtmlpp::Element hel){
    _copy(this,&hel);
    return *this;
}

libhtmlpp::HtmlElement & libhtmlpp::HtmlElement::operator=(const libhtmlpp::Element& hel){
    _copy(this,&hel);
    return *this;
}

libhtmlpp::HtmlElement & libhtmlpp::HtmlElement::operator=(const libhtmlpp::Element* hel){
    _copy(this,hel);
    return *this;
}

namespace libhtmlpp {

    void _copy(libhtmlpp::Element *dest,const libhtmlpp::Element *src){
        const libhtmlpp::Element* prev=nullptr;
        if(!src || !dest)
            return;
        struct cpyel {
            cpyel(){

            };

            cpyel(const cpyel &src){
                destin=src.destin;
                source=src.source;
            };

            libhtmlpp::Element       *destin;
            const libhtmlpp::Element *source;
        };

        std::stack<cpyel> cpylist;

NEWEL:
        if(src->getType()==libhtmlpp::HtmlEl && dest->getType()==libhtmlpp::HtmlEl){
            libhtmlpp::HtmlElement *hdest=(libhtmlpp::HtmlElement*)dest;
            libhtmlpp::HtmlElement *hsrc=(libhtmlpp::HtmlElement*)src;

            hdest->setTagname(hsrc->getTagname());
            for(libhtmlpp::HtmlElement::Attributes *cattr=hsrc->_firstAttr; cattr; cattr=cattr->_nextAttr){
                if(!cattr->_Value.empty())
                    hdest->setAttribute(cattr->_Key.data(),cattr->_Key.size(),cattr->_Value.data(),cattr->_Value.size());
                else
                    hdest->setAttribute(cattr->_Key.data(),cattr->_Key.size(),nullptr,0);
            }

            if(hsrc->_childElement){
                if(hsrc->_childElement->getType()==HtmlEl){
                    hdest->_childElement= new HtmlElement;
                    hdest->_Type=HtmlEl;
                }else if(hsrc->_childElement->getType()==TextEl){
                    hdest->_childElement= new TextElement;
                    hdest->_Type=TextEl;
                }
                cpyel childel;
                childel.destin=hdest->_childElement;;
                childel.source=hsrc->_childElement;
                cpylist.push(childel);
            }
        }else if(src->getType()==libhtmlpp::TextEl && dest->getType()== libhtmlpp::TextEl){
            ((TextElement*)dest)->_Text=(((TextElement*)src)->_Text);
        }

        if(prev)
            dest->_prevElement=(Element*)prev;

        Element* next=src->nextElement();

        if(next){
             if(next->getType()==HtmlEl)
                dest->_nextElement= new HtmlElement();
             else if(next->getType()==TextEl)
                dest->_nextElement= new TextElement();
             prev=dest;
             src=next;
             dest=dest->_nextElement;
             goto NEWEL;
        }

        if(!cpylist.empty()){
            cpyel childel(cpylist.top());
            prev=nullptr;
            dest=childel.destin;
            src=childel.source;
            cpylist.pop();
            goto NEWEL;
        }
    }
};

void libhtmlpp::Element::insertBefore(libhtmlpp::Element* el){
    if(el->getType()==HtmlEl){
        HtmlElement *nel=new HtmlElement();
        _copy(nel,el);
        _prevElement->_nextElement=nel;
    }else if(el->getType()==TextEl){
        TextElement *txt= new TextElement;
        _copy(txt,el);
        _prevElement->_nextElement=txt;
    }
    Element *nexel=_prevElement->_nextElement,*prev=nullptr;

    while(nexel){
        prev=nexel;
        nexel=nexel->nextElement();
    }

    nexel=this;
    _prevElement=prev;
}

void libhtmlpp::Element::insertAfter(libhtmlpp::Element* el){
    Element *nexel=nullptr,*prev=_nextElement;

    if(el->getType()==HtmlEl){
        _nextElement= new HtmlElement;

    }else if(el->getType()==TextEl){
        _nextElement= new TextElement;
    }

    _copy(_nextElement,el);

    nexel=_nextElement;

    while(nexel){
        nexel=nexel->nextElement();
    }

    nexel=prev;

}

libhtmlpp::Element& libhtmlpp::Element::operator=(const Element &hel){
    _copy(this,&hel);
    return *this;
}

libhtmlpp::Element& libhtmlpp::Element::operator=(const Element *hel){
    _copy(this,hel);
    return *this;
}

libhtmlpp::Element *libhtmlpp::Element::nextElement() const{
    return _nextElement;
}

libhtmlpp::Element *libhtmlpp::Element::prevElement() const{
    return _prevElement;
}

int libhtmlpp::Element::getType() const{
    return _Type;
}

libhtmlpp::Element::Element(){
    _prevElement=nullptr;
    _nextElement=nullptr;
    _firstElement=nullptr;
    _Type=-1;
}

libhtmlpp::Element::Element(const libhtmlpp::Element& el){
    _prevElement=nullptr;
    _nextElement=nullptr;
    _firstElement=nullptr;
    _Type=-1;
    _copy(this,&el);
}

libhtmlpp::Element::~Element(){
    if(this==_firstElement){
        Element *curel=this;
        while(curel){
            Element *next=curel->_nextElement;
            curel->_nextElement=nullptr;
            delete curel;
            curel=next;
        }
    }
};

libhtmlpp::TextElement::TextElement() : Element(){
    _Type=TextEl;
}

libhtmlpp::TextElement::TextElement(const TextElement &texel) : Element(){
    _Type=TextEl;
    _copy(this,&texel);
}

libhtmlpp::TextElement::~TextElement(){
}

libhtmlpp::TextElement & libhtmlpp::TextElement::operator=(const libhtmlpp::Element& hel){
    _copy(this,&hel);
    return *this;
}

libhtmlpp::TextElement & libhtmlpp::TextElement::operator=(const libhtmlpp::Element* hel){
    _copy(this,hel);
    return *this;
}

void libhtmlpp::TextElement::setText(const char* txt){
    std::copy(txt,txt+strlen(txt),
              std::insert_iterator<std::vector<char>>(_Text,_Text.begin()));
}

const char * libhtmlpp::TextElement::getText(){
    _CStr=_Text;
    _CStr.push_back('\0');
    return _Text.data();
}


libhtmlpp::HtmlPage::HtmlPage(){
}

libhtmlpp::HtmlPage::~HtmlPage(){
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlPage::loadFile(const char* path){
    std::string data;
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
        data.append(tmp,fs.gcount());
    }
    fs.close();

    libhtmlpp::HtmlElement *el=loadString(data);
    return el;
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlPage::loadString(const std::string &src){
    _Page=src.c_str();
    return loadString(_Page);
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlPage::loadString(const char *src){
    _Page=src;
    return loadString(_Page);
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlPage::loadString(const HtmlString &node){
    _CheckHeader(node);
    return _Page.parse();
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlPage::loadString(const HtmlString *node){
    _CheckHeader(node);
    return _Page.parse();
}

void libhtmlpp::HtmlPage::saveFile(const char* path){
    HtmlString data;
    std::ofstream fs;

    print(_Page.parse(),data);

    try{
        fs.open(path);
    }catch(std::exception &e){
        HTMLException excp;
        throw excp[HTMLException::Critical] << e.what();
    }

    fs << data.c_str();

    fs.close();

}

void libhtmlpp::HtmlPage::_CheckHeader(const HtmlString &page){
    const char type[] = { '!','D','O','C','T','Y','P','E' };
    int i = 0;

    while (i < 8) {
        if (page[i+1] != type[i]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "No Doctype found arborting";
            throw excp;
        }
        ++i;
    }

    do{
        ++i;
    } while (page[i] == ' ');

    const char typevalue[] = { 'h','t','m','l' };
    size_t tpvl = 4;

    if ((i + tpvl) > page.size()) {
        HTMLException excp;
        excp[HTMLException::Critical] << "Doctype header broken or wrong type";
        throw excp;
    }

    int ii = 0,ie=i+tpvl;

    while (i < ie) {
        if (page[i] != typevalue[ii]) {
            HTMLException excp;
            excp[HTMLException::Critical] << "wrong Doctype";
            throw excp;
        }
        ++i;
        ++ii;
    }
}

void libhtmlpp::print(Element* el, HtmlString &output) {

    std::stack<libhtmlpp::Element*> *cpylist = new std::stack<libhtmlpp::Element*>;

PRINTNEXTEL:
    switch(el->_Type){
        case HtmlEl:{
            output.append("<");
            output.append(((HtmlElement*) el)->getTagname());
            for (HtmlElement::Attributes* curattr = ((HtmlElement*) el)->_firstAttr; curattr; curattr = curattr->_nextAttr) {
                output.append(" ");
                output.append(curattr->_Key.data(),curattr->_Key.size());
                if(!curattr->_Value.empty()){
                    output.append("=\"");
                    output.append(curattr->_Value.data(),curattr->_Value.size());
                    output.append("\"");
                }
            }
            output.append(">");

            if (((HtmlElement*) el)->_childElement) {
                cpylist->push(el);
                el=((HtmlElement*) el)->_childElement;
            }

            if (el->_nextElement) {
                el=el->_nextElement;
                goto PRINTNEXTEL;
            }

            while(!cpylist->empty()){
                el=cpylist->top();

                output.append("</");
                output.append(((HtmlElement*) el)->getTagname());
                output.append(">");

                cpylist->pop();
                if(el->_nextElement){
                    el=el->_nextElement;
                    goto PRINTNEXTEL;
                }
            }
        }break;

        case TextEl :{
            output.append(((TextElement*)el)->_Text.data(),((TextElement*)el)->_Text.size());

            if (el->_nextElement) {
                el=el->_nextElement;
                goto PRINTNEXTEL;
            }

            while(!cpylist->empty()){
                el=cpylist->top();

                output.append("</");
                output.append(((HtmlElement*) el)->_TagName.data(),((HtmlElement*) el)->_TagName.size());
                output.append(">");

                cpylist->pop();
                if(el->_nextElement){
                    el=el->_nextElement;
                    goto PRINTNEXTEL;
                }
            }
        }break;

        default:
            HTMLException excp;
            excp[HTMLException::Error] << "Unkown Elementtype";
            throw excp;
            break;
    }
    delete cpylist;
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlElement::getElementbyID(const char *id) const{
    std::stack <Element*> *childs=new std::stack <Element*>;
    const Element *curel=this;
SEARCHBYID:
    if(curel->getType()==HtmlEl){
        if(((HtmlElement*)curel)->_childElement){
            childs->push(((HtmlElement*)curel)->_childElement);
        }
        const char *key=((HtmlElement*)curel)->getAtributte("id");
        if(key && strcmp(key,id)==0){
            delete childs;
            return (HtmlElement*)curel;
        }
    }

    if(curel->nextElement()){
        curel=curel->nextElement();
        goto SEARCHBYID;
    }

    if(!childs->empty()){
        curel=childs->top();
        childs->pop();
        goto SEARCHBYID;
    }

    delete childs;
    return nullptr;
}

libhtmlpp::HtmlElement *libhtmlpp::HtmlElement::getElementbyTag(const char *tag) const{
    std::stack <Element*> *childs=new std::stack <Element*>;
    const Element *curel=this;
SEARCHBYTAG:
    if(curel->getType()==HtmlEl){
        if(((HtmlElement*)curel)->_childElement){
            HtmlElement *find=((HtmlElement*)((HtmlElement*)curel)->_childElement)->getElementbyTag(tag);
            if(find)
                return find;
        }
        const char *tname=((HtmlElement*)curel)->getTagname();
        if(tname && strcmp(tname,tag)==0){
            return (HtmlElement*)curel;
        }
    }

    if(curel->nextElement()){
        curel=curel->nextElement();
        goto SEARCHBYTAG;
    }

    if(!childs->empty()){
        curel=childs->top();
        childs->pop();
        goto SEARCHBYTAG;
    }

    delete childs;
    return nullptr;
}

void libhtmlpp::HtmlElement::setAttribute(const char* name, const char* value) {
    if(value)
        setAttribute(name,strlen(name),value,strlen(value));
    else
        setAttribute(name,strlen(name),nullptr,0);
}

void libhtmlpp::HtmlElement::setAttribute(const char* name,size_t nlen, const char* value,size_t vlen) {
    Attributes* cattr = nullptr;

    for (Attributes* curattr = _firstAttr; curattr; curattr=curattr->_nextAttr) {
        if(curattr->_Key.size() == nlen){
            if ( memcmp(curattr->_Key.data(),name,curattr->_Key.size()) ==0 ) {
                cattr = curattr;
            }
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
        std::copy(name,name+nlen,std::insert_iterator<std::vector<char>>(cattr->_Key,cattr->_Key.begin()) );
    }
    if(vlen>0)
        std::copy(value,value+vlen,std::insert_iterator<std::vector<char>>(cattr->_Value ,cattr->_Value .begin()) );
    else
        cattr->_Value.clear();
}

void libhtmlpp::HtmlElement::setIntAttribute(const char* name, int value) {
    char buf[255];
    snprintf(buf,255,"%d",value);
    setAttribute(name,buf);
}

const char* libhtmlpp::HtmlElement::getAtributte(const char* name) const{
    for (Attributes* curattr = _firstAttr; curattr; curattr = curattr->_nextAttr) {
        if(curattr->_Key.size() > strlen(name))
            return nullptr;
        if ( memcmp(curattr->_Key.data(),name,curattr->_Key.size()) == 0 ) {
            curattr->_CStr=curattr->_Value;
            curattr->_CStr.push_back('\0');
            return curattr->_CStr.data();
        }
    }
    return nullptr;
}

int libhtmlpp::HtmlElement::getIntAtributte(const char* name) {
    return atoi(getAtributte(name));
}

libhtmlpp::HtmlElement::Attributes::Attributes() {
    _nextAttr=nullptr;
}

libhtmlpp::HtmlElement::Attributes::~Attributes() {
}

libhtmlpp::HtmlTable::HtmlTable(){
    _firstRow=nullptr;
    _lastRow=nullptr;
}

libhtmlpp::HtmlTable::~HtmlTable(){
    Row *next=_firstRow,*curel=nullptr;
    while(next){
        next=curel->_nextRow;
        curel->_nextRow =nullptr;
        delete curel;
    }
}

libhtmlpp::HtmlTable::Row &libhtmlpp::HtmlTable::operator<<(const libhtmlpp::HtmlTable::Row row){
    if(_firstRow){
        _lastRow->_nextRow=new Row(row);
        _lastRow=_lastRow->_nextRow;
    }else{
        _firstRow= new Row(row);
        _lastRow=_firstRow;
    }
    ++_count;
    return *_lastRow;
}

libhtmlpp::HtmlTable::Row & libhtmlpp::HtmlTable::operator[](size_t pos){
    if(!_firstRow || _count<pos){
        libhtmlpp::HTMLException exp;
        exp[HTMLException::Error] << "HtmlTable: Row at this position won't exists !";
        throw exp;
    }
    size_t cpos=0;
    Row *curel=nullptr;
    for(curel=_firstRow; curel; curel=curel->_nextRow){
        if(cpos==pos)
            return *curel;
        ++cpos;
    }
    return *curel;
}

void libhtmlpp::HtmlTable::insert(libhtmlpp::HtmlElement* element){
    element->setTagname("table");
    for(Row *crow=_firstRow; crow; crow=crow->_nextRow){
        HtmlElement hrow("tr");

        for(Column *ccol=crow->_firstColumn; ccol; ccol=ccol->_nextColumn ){
            HtmlString buf;
            buf << "<td>";
            buf << ccol->Data.c_str();
            buf << "</td>";
            hrow.appendChild(buf.parse());
        }

        element->appendChild(&hrow);
    }
}

void libhtmlpp::HtmlTable::parse(libhtmlpp::HtmlElement* element){
}

void libhtmlpp::HtmlTable::setHeader(int count,...){
    va_list args;
    va_start(args,count);

    for (int i = 0; i < count; ++i) {
        _header << va_arg(args, const char*);
    }

}

void libhtmlpp::HtmlTable::deleteRow(size_t pos){
    Row *drow=&(*this)[pos];
    try{
        Row *prev=&(*this)[pos-1];
        prev->_nextRow=drow->_nextRow;
    }catch(...){}
    drow->_nextRow=nullptr;
    --_count;
    delete drow;
}

libhtmlpp::HtmlTable::Column::Column(){
    _nextColumn=nullptr;
}

libhtmlpp::HtmlTable::Column::Column(const libhtmlpp::HtmlTable::Column& col){
     _nextColumn=nullptr;
    Data = col.Data;
}

libhtmlpp::HtmlTable::Column::Column(const libhtmlpp::HtmlString &data){
     _nextColumn=nullptr;
    Data=data;
}

libhtmlpp::HtmlTable::Column::~Column(){
}

libhtmlpp::HtmlTable::Row::Row(){
    _firstColumn=nullptr;
    _lastColumn=nullptr;
    _nextRow=nullptr;
    _count=0;
}

libhtmlpp::HtmlTable::Row::~Row(){
    Column *cnext=_firstColumn,*ccurel=nullptr;
    while(cnext){
        ccurel=cnext->_nextColumn;
        cnext->_nextColumn=nullptr;
        delete cnext;
        cnext=ccurel;
    }
}

libhtmlpp::HtmlTable::Row::Row(const libhtmlpp::HtmlTable::Row& row){
    _firstColumn=nullptr;
    _lastColumn=nullptr;
    _nextRow=nullptr;
    _count=0;

    for(Column *curel=row._firstColumn; curel; curel=curel->_nextColumn){
        *this << curel->Data;
    }
}

libhtmlpp::HtmlTable::Row& libhtmlpp::HtmlTable::Row::operator<<(Column &col){
    if(_firstColumn){
        _lastColumn->_nextColumn=new Column(col);
        _lastColumn=_lastColumn->_nextColumn;
    }else{
        _firstColumn= new Column(col);
        _lastColumn=_firstColumn;
    }
    ++_count;
    return *this;
}

libhtmlpp::HtmlTable::Row &libhtmlpp::HtmlTable::Row::operator<<(libhtmlpp::HtmlString value){
    Column col(value);
    *this << col;
    return *this;
}

libhtmlpp::HtmlTable::Row &libhtmlpp::HtmlTable::Row::operator<<(const char* value){
    HtmlString buf;
    buf << value;
    *this << buf;
    return  *this;
}

libhtmlpp::HtmlTable::Row & libhtmlpp::HtmlTable::Row::operator<<(int value){
    char buf[255];
    snprintf(buf,255,"%d",value);
    return *this << buf;
}

libhtmlpp::HtmlTable::Column & libhtmlpp::HtmlTable::Row::operator[](size_t pos){
    if(!_firstColumn || _count<pos){
        libhtmlpp::HTMLException exp;
        exp[HTMLException::Error] << "HtmlTable: Column at this position won't exists !";
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

void libhtmlpp::HtmlTable::Row::delColumn(size_t pos){
    Column *dcol=&(*this)[pos];
    try{
        Column *prev=&(*this)[pos-1];
        prev->_nextColumn=dcol->_nextColumn;
    }catch(...){}
    dcol->_nextColumn=nullptr;
    --_count;
    delete dcol;
}

void libhtmlpp::HtmlTable::Row::clear(){
    Column *cnext=_firstColumn,*ccurel=nullptr;
    while(cnext){
        ccurel=cnext;
        cnext=cnext->_nextColumn;
        delete ccurel;
    }
    _firstColumn = nullptr;
    _lastColumn = nullptr;
    _count = 0;
}
