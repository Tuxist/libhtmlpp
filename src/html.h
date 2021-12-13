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

#ifndef HTML_H
#define HTML_H

namespace libhtmlpp {
    class HtmlElement {
    public:
        void setID(const char *id);
        void setClass(const char *cname);
        void setStyle(const char *css);
        void setComment(const char *comment);
        const char *printHtmlElement();
    protected:
        HtmlElement();
        ~HtmlElement();
        char            *_Tag;
        char            *_Text;
        char            *_Comment;
        HtmlElement     *_Child;
        HtmlElement     *_prevElement;
        HtmlElement     *_nextElement;
        
        class HtmlAttributes {
        protected:
            HtmlAttributes();
            ~HtmlAttributes();
            char           *_Key;
            char           *_Value;
            HtmlAttributes *_nextHtmlAttributes;
        };
    private:
        char          *_ID;
        char          *_Class;
        char          *_Style;
        friend class HtmlString;
    };
        
    class HtmlString {
    public:
        HtmlString();
        HtmlString(char *header);
        ~HtmlString();

        void assign(const char *src,size_t srcsize);
        void assign(const char *src);
        
        void push_back(const char  src);
        
        void insert(size_t pos,char src);
        
        HtmlString &operator+=(const char *src);
        HtmlString &operator+=(HtmlString &hstring);
        HtmlString &operator=(const char *src);
        char &operator[](size_t pos);
        
        HtmlString &operator<<(const char *src);
        HtmlString &operator<<(int src);
        HtmlString &operator<<(char src);
        HtmlString &operator<<(unsigned long src);

        const char        *c_str();
        size_t             size();
        void               clear();
        bool               validate();
        void               parse();
     private:
        void              _InitString();
        void              _parseTree();
        HtmlElement      *_buildTree(HtmlElement *node,HtmlElement *parent,ssize_t &pos);
        HtmlElement      *_serialzeElements(HtmlElement *prevnode,ssize_t &pos);
        int               _serialzeTags(size_t spos,size_t epos,char **value,size_t &valuesize);
        void              _printHtml(HtmlElement *node,size_t &level);
        char             *_Data;
        size_t            _DataSize;
        ssize_t         **_HTable;
        size_t            _HTableSize;
        char             *_HtmlHeader;
        HtmlElement      *_HtmlRootNode;
    };

    class HtmlPage {
    public:
        HtmlPage();
        ~HtmlPage();
        void loadFile(const char *path);
        void saveFile(const char *path);
        
        void   fromString(const char *src);
        size_t toString(const char **dest);
        
        void addElement(HtmlElement *element);
        const char *printHtml();
    private:
        HtmlString    *_HtmlDocument;
    }; 
    
    class HtmlTable : public HtmlElement{
    public:
        HtmlTable();
        ~HtmlTable();
        class Row {
        public:
            Row &operator<<(const char *value);
            Row &operator<<(int value);
        private:
            Row();
            ~Row();
            Row *_nextRow;
            friend class HtmlTable;
        };
    private:
    };
};

#endif
