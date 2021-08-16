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

#include "hstring.h"

#ifndef HTML_H
#define HTML_H

namespace libhtmlpp {
    
    class HtmlElement {
    protected:
        virtual void setID(const char *id)=0;
        virtual void setClass(const char *cname)=0;
        virtual void setStyle(const char *css)=0;
        virtual const char *printHtmlElement()=0;
    };
    
    class HtmlPage{
    public:
        HtmlPage();
        ~HtmlPage();
        void loadFile(const char *path);
        void addElement(HtmlElement *element);
        const char *printHtml();
    private:
        HtmlString *_HtmlDocument;
    }; 
    
    class HtmlTable : public HtmlElement{
    public:
        HtmlTable();
        ~HtmlTable();
        void setID(const char *id);
        void setClass(const char *cname);
        void setStyle(const char *css);
        const char *printHtmlElement();
    private:
        char       *_ID;
        char       *_Class;
        char       *_Style;
    };
};

#endif
