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

#include <cstring>

#include "utils.h"
#include "html.h"

using namespace libhtmlpp;

HtmlTable::HtmlTable() {
    _ID=nullptr;
    _Class=nullptr;
    _Style=nullptr;
}


HtmlTable::~HtmlTable(){
    
}

void HtmlTable::setID(const char *id){
    setter(id,getlen(id),&_ID);
}

void HtmlTable::setClass(const char *cname){
    setter(cname,getlen(cname),&_Class);
}

void HtmlTable::setStyle(const char *css){
    setter(css,getlen(css),&_Style,":;(),+~'");
}

//     class Row {
//     public:
//         Row &operator<<(const char *value){
//             _Data.assign(value,value+libhttppp::getlen(value));
//             return *this;
//         };
//         
//         Row &operator<<(int value){
//             char buf[255];
//             snprintf(buf,255, "%d", value); 
//             return *this << buf;
//         };
//         
//     private:
//         std::vector<char> _Data;
//         int   _Size;
//         Row(){
//             _Size=0;
//             _nextRow=nullptr;
//         };
//         ~Row(){
//             delete _nextRow;
//         };
//         Row *_nextRow;
//         friend class HtmlTable;
//     };
//     
//     
//     
//     Row &createRow(){
//         if(_firstRow!=nullptr){
//             _lastRow->_nextRow=new Row;
//             _lastRow=_lastRow->_nextRow;
//         }else{
//             _firstRow=new Row;
//             _lastRow=_firstRow;
//         }
//         return *_lastRow;
//     }
//     
//     const char *printHtml(){
//         _Buffer.clear();
//         if(_Id!=NULL)
//             _Buffer << "<table id=\"" << _Id << "\">";
//         else
//             _Buffer << "<table>";
//         for(Row *curow=_firstRow; curow; curow=curow->_nextRow){
//             _Buffer << "<tr>";
//             _Buffer.assign(curow->_Data.begin(),curow->_Data.end());
//             _Buffer << "</tr>";
//         }
//         _Buffer << "</table>";
//         _Buffer.push_back('\0');
//         return _Buffer.data();
//     }
// private:
//     std::vector<char>  _Buffer;
//     
//     Row               *_firstRow;
//     Row               *_lastRow;
// };

