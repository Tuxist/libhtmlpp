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

#include <systempp/sysconsole.h>

#include "../src/html.h"

#define Red     "\033[0;31m"
#define Green   "\033[0;32m"
#define NOCOLOR "\033[0m"

void *__dso_handle __attribute__((__visibility__("hidden"))) __attribute__((weak)) = nullptr;

int main(){
    libhtmlpp::HtmlTable table;
    try{
        table.setAttribute("Style",":;(),+~'");
        sys::cout  << Green << "Test Passed!" << NOCOLOR << sys::endl;
    }catch(...){
        sys::cout   << Red << "Test not Passed!" << NOCOLOR << sys::endl;
        return -1;
    }
    
    try{
        table.setAttribute("Style", "\"");
        sys::cout   << Red << "Test not Passed!" << NOCOLOR<< sys::endl;
        return -1;
    }catch(...){
        sys::cout   << Green << "Test Passed!" << NOCOLOR << sys::endl;
    }
    return 0;
}
