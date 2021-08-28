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

#include <unistd.h>
#include "fcntl.h"

#include "exception.h"
#include <ctype.h>

#ifndef UTIL_H
#define UTIL_H

namespace libhtmlpp {
    inline size_t getlen(const char *str){
        if(!str)
            return 0;
        size_t len= 0;
        while (*(str++) != '\0')
            ++len;
        return len;
    }

    inline void reverse(char s[]){
        int i, j;
        char c;
        
        for (i = 0, j = getlen(s)-1; i<j; i++, j--) {
            c = s[i];
            s[i] = s[j];
            s[j] = c;
        }
    }    
    
    inline void itoa(int n, char s[]){
        int i, sign;
        if ((sign = n) < 0) 
            n = -n;
        i = 0;
        do {       /* generate digits in reverse order */
            s[i++] = n % 10 + '0';   /* get next digit */
        } while ((n /= 10) > 0);     /* delete it */
        if (sign < 0)
            s[i++] = '-';
        s[i] = '\0';
        reverse(s);
    }

    inline bool isdigit(const char src){
        const char numbers[10]={0,1,2,3,4,5,6,7,8,9};
        for(int i=0; i<10; ++i){
            if((char)numbers[i]==src)
                return true;
        }
        return false;
    }
    
    inline bool isalpha(const char src){
        const char signs[52]={'A','B','C','D','E','F','G','H','I','J','K','L',
                            'M','N','O','P','Q','R','S','T','U','V','W','X','Y',
                            'Z','a','b','c','d','e','f','g','h','i','j','k','l',
                            'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};
        for(int i=0; i<52; ++i){
            if(signs[i]==src)
                return true;
        }
        return false;  
    }
    
    inline bool setter(const char *src,int srcsize,char **dest,const char *ssigns=nullptr){
        char *buf = new char[srcsize+1];
        bool nallowd=false;
        for(int i=0; i<srcsize; ++i){
            if(!isdigit(src[i]) || !isalpha(src[i]) || ssigns){
                nallowd=true;
                for(size_t pos=0; pos<getlen(ssigns); ++pos){
                    if(ssigns[pos]==src[i]){
                        nallowd=false;
                    }
                }
                if(nallowd){
                    delete[] buf;
                    return false;
                }
            }
            buf[i]=src[i];
        }
        delete[] *dest;
        buf[srcsize]='\0';
        *dest=buf;
        return true;
    }
    
    inline int substr(const char *src,char **dest,size_t spos,size_t endpos){
        size_t srcsize=endpos-spos;
        char *buf = new char[srcsize+1];
        for(int i=spos,j=0; j<srcsize; ++i,++j){
            buf[j]=src[i];
        }
        buf[srcsize]='\0';
        *dest=buf;
        return srcsize;
    }
    
    inline const  char *scopy(const char* first, const char* last, char* des){
        while (first != last) {
            *des++ = *first++;
        }
        return des;
    }
    
    inline size_t cleannewline(const char *src,size_t srcsize,char **dest){
        struct buffer {
            buffer(){
                nextbuffer=nullptr;
            }
            ~buffer(){
                delete[] text;
                delete nextbuffer;
            }
            char   *text;
            size_t  tsize;
            buffer *nextbuffer;
        }*ftbuf=nullptr,*ltbuf=nullptr;
        
        size_t spos=0;
        
        for(size_t i=0; i<srcsize; ++i){
            if(src[i] == '\n' || src[i] == '\r'){
                if(ftbuf){
                    ltbuf->nextbuffer= new buffer;
                    ltbuf=ltbuf->nextbuffer;
                }else{
                    ftbuf=new buffer;
                    ltbuf=ftbuf;
                }
                ltbuf->tsize=substr(src,&ltbuf->text,spos,i);
                if((i+1)<srcsize && (i+1)=='\n')
                    spos=(i+=2);
                else
                    spos=(++i);
            }
        }
        size_t ssize=0,written=0;
        for(buffer *cbuf=ftbuf; cbuf; cbuf=cbuf->nextbuffer)
            ssize+=cbuf->tsize;
        
        *dest=new char[ssize+1];
        
        for(buffer *cbuf=ftbuf; cbuf; cbuf=cbuf->nextbuffer){
            scopy(cbuf->text,cbuf->text+cbuf->tsize,*dest+written);
            written+=cbuf->tsize;
        }
        
        delete ftbuf;
        
        return ssize;
    }
    
    inline bool ncompare(char *src,size_t ssize,char *comp,size_t csize){
        if(ssize<csize)
            return false;
        for(int i=0; i<csize; ++i){
            if(src[i]!=comp[i])
                return false;
        }
        return true;
    }
    
};

#endif
