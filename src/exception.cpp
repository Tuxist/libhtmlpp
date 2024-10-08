/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "exception.h"


libhtmlpp::HTMLException::HTMLException() {
    curCType=Note;
};

libhtmlpp::HTMLException::HTMLException(const HTMLException &exp){
    msg=exp.msg;
    curCType = exp.curCType;
}

libhtmlpp::HTMLException::~HTMLException(){
}

int libhtmlpp::HTMLException::getErrorType(){
    return curCType; 
}

const char* libhtmlpp::HTMLException::what(){
    return msg.c_str();
}

libhtmlpp::HTMLException& libhtmlpp::HTMLException::append(const char *src){
    msg.append(src);
    return *this;   
}

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator[](int errtype){
    curCType=errtype;
    return *this;
}

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator<<(const char *src){
    return append(src);
};

libhtmlpp::HTMLException& libhtmlpp::HTMLException::operator<<(int src){
    char buffer[255];
    snprintf(buffer,255,"%d",src);
    msg.append(buffer);
    return *this;
}
