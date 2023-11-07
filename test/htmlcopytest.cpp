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

#include "html.h"
#include "exception.h"

#define Red     "\033[0;31m"
#define Green   "\033[0;32m"
#define NOCOLOR "\033[0m"

class HtmlCopy{
public:
    HtmlCopy(libhtmlpp::Element *index){
        index2=index;
        libhtmlpp::HtmlElement  el("span");
        el.setAttribute("id","txt3");
        libhtmlpp::HtmlElement *body=index2.getElementbyTag("body");
        if(body)
            body->appendChild(&el);
    }

    void printModify(){
        std::string html;
        libhtmlpp::print(&index2,nullptr,html);
        std::cout << html << std::endl;
    }

    ~HtmlCopy(){

    }
private:
    libhtmlpp::HtmlElement index2;
};

int main(int arc,char *argv[]){
    libhtmlpp::HtmlPage page;
    try{
        libhtmlpp::Element *index=page.loadFile(argv[1]);

        HtmlCopy cpy(index);


        std::string html;

        std::cout << "Orginal html:" << std::endl;
        libhtmlpp::print(index,nullptr,html);

        std::cout << "Modified html:" << std::endl;

        cpy.printModify();


        std::cout << html << std::endl;
        std::cout << Green << "Test Passed!" << NOCOLOR << std::endl;
    }catch(libhtmlpp::HTMLException &exp){
        std::cout << exp.what() << std::endl;
        std::cout << Red << "Test not Passed!" << NOCOLOR << std::endl;
        return -1;
    }
    return 0;
}

