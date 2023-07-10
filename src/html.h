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

#pragma once

namespace libhtmlpp {
    class DocElements;

    class HtmlElement {
    public:
        HtmlElement(const char* tag);
        ~HtmlElement();

        void        setAttribute(const char* name, const char* value);
        void        setIntAttribute(const char* name, int value);

        const char* getAtributte(const char* name);
        int         getIntAtributte(const char* name);

        const char* printHtmlElement();

    protected:

        static void       _print(HtmlElement* el, HtmlElement* parent,std::string &output);
        HtmlElement*      _prevElement;
        HtmlElement*      _nextElement;
        HtmlElement*      _childElement;

        struct Attributes {
            Attributes();
            ~Attributes();
            std::string _Key;
            std::string _Value;
            Attributes*       _nextAttr;
        };

    private:
        //if text tagname must be zero
        std::string  _TagName;

        //for text and comment fields
        std::string  _Text;

        //if text Attributes must be zero
        Attributes*    _firstAttr;
        Attributes*    _lastAttr;

        //buffer for c_str() function
        std::string    _Cstr;

        friend class HtmlString;
    };

    class HtmlDivLayer : public HtmlElement {
    public:
        HtmlDivLayer();
        ~HtmlDivLayer();
    private:
    };

    class HtmlTable : public HtmlElement {
    public:
        HtmlTable();
        ~HtmlTable();
        class Row {
        public:
            Row& operator<<(const char* value);
            Row& operator<<(int value);
        private:
            Row();
            ~Row();
            Row* _nextRow;
            friend class HtmlTable;
        };
    private:
    };

    class HtmlPage {
    public:
        HtmlPage();
        ~HtmlPage();
        void loadFile(const char* path);
        void saveFile(const char* path);

        void   fromString(const char* src);
        size_t toString(const char** dest);

        void addElement(HtmlElement* element);
        const char* printHtml();
    private:
        std::string   _C_str;
        HtmlElement*  _RootNode;
    };

    class HtmlString {
    public:
        HtmlString();
        ~HtmlString();

        void assign(const char* src, size_t srcsize);
        void assign(const char* src);

        void push_back(const char  src);

        void insert(size_t pos, char src);

        HtmlString& operator+=(const char* src);
        HtmlString& operator+=(HtmlString& hstring);
        HtmlString& operator=(const char* src);
        const char operator[](size_t pos);

        HtmlString& operator<<(const char* src);
        HtmlString& operator<<(int src);
        HtmlString& operator<<(unsigned int src);
        HtmlString& operator<<(char src);
        HtmlString& operator<<(unsigned long src);

        const char*        c_str();
        size_t             size();
        size_t             length();
        void               clear();
        HtmlElement*       parse();
    private:
        void              _InitString();
        void              _parseTree();
        void              _serialelize(std::string  in, HtmlElement** out);
        HtmlElement*      _buildTree(ssize_t& pos);

        HtmlElement*      _buildTreeElement(libhtmlpp::DocElements* curel, libhtmlpp::DocElements* nexel, 
                                            HtmlElement* parent);
        std::string             _Data;
        std::string             _Cstr;
        ssize_t**               _HTable;
        size_t                  _HTableSize;
        HtmlElement*            _RootNode;
    };
};
