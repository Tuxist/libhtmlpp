/*******************************************************************************
 * Copyright (c) 2014, Jan Koester jan.koester@gmx.net
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

#include <stddef.h>

#include "config.h"
#include "system.h"

/*
 * Error Statas
 * Note: Notice only for Information
 * Warning: Could be ending in an error
 * Error: Error happend possible closed Client Connection
 * Critical: Some happend that will shutdown your Apllication
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef DEBUG

#endif

namespace libhtmlpp {
    
    class HTMLException {
    public:
        
        HTMLException(){
            _CType=Note;
            _Buffer=nullptr;
            _BufferSize=0;
        };
       
        ~HTMLException(){
            delete[] _Buffer;
        }
        
        int getErrorType(){
           return _CType; 
        }
        
        const char* what() const throw(){
            return _Buffer;
        }
        
        enum Type {Note,Warning,Error,Critical};
        
    
        HTMLException& asign(const char *src){
            if(!src)
                return *this;
            size_t srcsize=0;
EXCEPTIONLEN:
            if(src[srcsize]!='\0'){
                ++srcsize;
                goto EXCEPTIONLEN;
            }
            size_t nsize=(srcsize+_BufferSize);
            char *buf=new char[nsize+1];
            size_t i;
            for(i=0; i<_BufferSize; ++i)
                buf[i]=_Buffer[i];
            for(size_t ii = 0; ii<srcsize; ++ii)
                buf[i++]=src[ii];
            _BufferSize=nsize;
            buf[nsize]='\0';
            delete[] _Buffer;
            _Buffer=buf;
            return *this;   
        }

        HTMLException& operator[](int errtype){
            _CType=errtype;
            return *this;
        }
        
        HTMLException& operator<<(const char *src){
            return asign(src);       
        };

        HTMLException& operator<<(int src){
            char *buf=new char[sizeof(int)+1];
            int i, sign;
            if ((sign = src) < 0) 
                src = -src;         
            i = 0;
            do {       /* generate digits in reverse order */
                buf[i++] = src % 10 + '0';   /* get next digit */
            } while ((src /= 10) > 0);     /* delete it */
            if (sign < 0)
                buf[i++] = '-';
            buf[i] = '\0';
            char c;
            int slen=0;
EXCEPTIONLEN2:
            if(buf[slen]!='\0'){
                ++slen;
                goto EXCEPTIONLEN2;
            }
            for (int i = 0, j = slen-1; i<j; i++, j--) {
                c = buf[i];
                buf[i] = buf[j];
                buf[j] = c;
            }
            asign(buf);
            delete[] buf;
            return *this;
        }
    private:
        char  *_Buffer;
        size_t _BufferSize;        
        int    _CType;
    };
}
#endif

