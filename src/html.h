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

#include <sys/types.h>

#include <string>
#include <cstring>
#include <vector>

#pragma once

namespace libhtmlpp {
    class DocElements;
    class HtmlElement;

    enum ElementType{
        TextEl=0,
        HtmlEl=1
    };

    class Element {
    public:
        Element();

        virtual ~Element();

        Element(const Element &el);

        void insertAfter(Element* el);
        void insertBefore(Element* el);

        Element& operator=(const Element &hel);
        Element& operator=(const Element *hel);

        Element*       nextElement() const;
        Element*       prevElement() const;

        int            getType() const;
    protected:
        Element*      _prevElement;
        Element*      _nextElement;
        int           _Type;

        friend class HtmlElement;
        friend class TextElement;
        friend class HtmlString;
        friend void  print(Element* el, std::string &output);
        friend void _copy(libhtmlpp::Element *dest,const libhtmlpp::Element *src);
    };

    class HtmlElement : public Element {
    public:
        HtmlElement();
        HtmlElement(const char* tag);
        HtmlElement(const HtmlElement &hel);
        HtmlElement(const HtmlElement *hel);
        ~HtmlElement();

        HtmlElement& operator=(const Element  hel);
        HtmlElement& operator=(const Element &hel);
        HtmlElement& operator=(const Element *hel);

        void         setAttribute(const char* name, const char* value);
        void         setIntAttribute(const char* name, int value);

        const char*  getAtributte(const char* name) const;
        int          getIntAtributte(const char* name);

        void         insertChild(Element* el);
        void         appendChild(Element* el);

        void         setTagname(const char *name);
        const char  *getTagname() const;

        HtmlElement *getElementbyID(const char *id) const;
        HtmlElement *getElementbyTag(const char *tag) const;
    protected:

        Element*    _childElement;

        struct Attributes {
            Attributes();
            ~Attributes();
            std::string *_Key;
            std::string *_Value;
            Attributes* _nextAttr;
        };

    private:
        //if text tagname must be zero
        std::string   *_TagName;

        //if text Attributes must be zero
        Attributes*    _firstAttr;
        Attributes*    _lastAttr;

        friend class HtmlString;
        friend class HtmlTable;
        friend void  print(Element* el, std::string &output);
        friend void _copy(libhtmlpp::Element *dest,const libhtmlpp::Element *src);
    };

    class TextElement : public Element{
    public:
        TextElement();
        TextElement(const TextElement &texel);
        ~TextElement();

        TextElement& operator=(const Element &hel);
        TextElement& operator=(const Element *hel);

        const char *getText();
        void        setText(const char *txt);

    protected:
        std::vector<char> _Text;
        friend class HtmlString;
        friend void  print(Element* el, std::string &output);
        friend void _copy(libhtmlpp::Element *dest,const libhtmlpp::Element *src);
    };

    void print(Element* el, std::string &output);

    class HtmlString {
    public:
        HtmlString();
        HtmlString(const HtmlString &str);
        HtmlString(const HtmlString *str);
        HtmlString(const char *str);
        HtmlString(std::string *str);
        ~HtmlString();

        void append(const char* src, size_t srcsize);
        void append(const char* src);
        void append(HtmlString& hstring);

        void push_back(const char  src);

        void insert(size_t pos, char src);

        HtmlString& operator+=(const char* src);
        HtmlString& operator+=(HtmlString& hstring);
        HtmlString& operator=(const char* src);
        HtmlString& operator=(std::string *src);
        const char  operator[](size_t pos) const;

        HtmlString& operator<<(const char* src);
        HtmlString& operator<<(std::string &src);
        HtmlString& operator<<(std::string *src);
        HtmlString& operator<<(HtmlString src);
        HtmlString& operator<<(int src);
        HtmlString& operator<<(unsigned int src);
        HtmlString& operator<<(char src);
        HtmlString& operator<<(unsigned long src);

        const size_t       size() const;
        void               clear();
        bool               empty();
        const char *       c_str();
        HtmlElement*       parse();
        bool               validate(std::string *err);
    private:
        void               _parseTree();
        void               _serialelize(std::vector<char> in, HtmlElement** out);
        Element*           _buildTree(ssize_t& pos);
        DocElements       *_buildtreenode(DocElements* prev,DocElements* next,DocElements *start,DocElements *end);
        std::vector<char>  _Data;
        ssize_t**          _HTable;
        size_t             _HTableSize;
        HtmlElement*       _RootNode;
        friend void HtmlEncode(const char *input,HtmlString *output);
    };

    void HtmlEncode(const char *input,HtmlString *output);
    void HtmlEncode(const char *input,std::string &output);

    class HtmlPage {
    public:
        HtmlPage();
        ~HtmlPage();
        HtmlElement *loadFile(const char* path);
        void         saveFile(const char* path);
        HtmlElement *loadString(const std::string *src);
        HtmlElement *loadString(const char *src);
        HtmlElement *loadString(const HtmlString &node);
        HtmlElement *loadString(const HtmlString *node);
    private:
        void         _CheckHeader(const HtmlString& page);
        HtmlString  *_Page;
    };

    class HtmlTable {
    public:
        HtmlTable();
        ~HtmlTable();

        class Column {
        public:
            HtmlString  Data;
        private:
            Column();
            Column(const Column &col);
            Column(const HtmlString &data);
            ~Column();

            Column      *_nextColumn;
            friend class HtmlTable;
        };

        class Row {
        public:
            Row();
            Row(const Row &row);
            ~Row();

            Row& operator<<(Column &col);
            Row& operator<<(HtmlString  value);
            Row& operator<<(const char* value);
            Row& operator<<(int value);

            Column& operator[](size_t pos);

            void delColumn(size_t pos);
            void clear();
        private:
            Column *_firstColumn;
            Column *_lastColumn;
            Row    *_nextRow;
            size_t  _count;
            friend class HtmlTable;
        };

        Row& operator<<(const Row row);
        Row& operator[](size_t pos);

        void insert(HtmlElement *element);
        void parse(HtmlElement *element);

        void setHeader(int count,...);
        void deleteRow(size_t pos);
    private:
        Row     *_firstRow;
        Row     *_lastRow;
        Row      _header;
        size_t   _count;
    };
};
