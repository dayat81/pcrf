//
//  diameter.cpp
//  diameter
//
//  Created by hidayat on 10/14/15.
//  Copyright © 2015 hidayat. All rights reserved.
//

#include "diameter.h"
#include <stdio.h>

diameter::diameter(char* h,char* b,int l){   //create empty
    diameter::h=h;
    diameter::b=b;
    diameter::len=l;
}

void diameter::compose(char* r){
    int i=0;
    while (i<4) {
        *r=*h;
        r++;
        h++;
        i++;
    }
    h=h-4;
    i=0;
    while (i<len) {
        *r=*b;
        b++;
        i++;
        r++;
    }
    b=b-len;
}

void diameter::dump(){
    //printf("diameter dump %i \n",len);
    int i=0;
    while (i<4) {
        printf("%02X",*h&0xff);
        i++;
        h++;
    }
    h=h-4;
    i=0;
    while (i<len) {
        printf("%02X",*b&0xff);
        i++;
        b++;
    }
    b=b-len;
}

void diameter::populateHeader(){
    version=*h;
    cflags=*b;
    ccode=b+1;
    appId=b+4;
    hbh=b+8;
    e2e=b+12;
    
    curr=16;    //current position
    b=b+curr; //set pointer to first avp
}

avp diameter::copyAVP(int acode, int vcode){
    avp a=avp(0, 0);
    bool found=false;
    int avpcode;
    int vendorbit;
    int lavpval;
    int padding;
    int vendorcode=0;
    while (curr<len&&!found) {
        int l=8;
        avpcode=(((*b& 0xff) << 24) | ((*(b+1) & 0xff) << 16)| ((*(b+2) & 0xff) << 8) | ((*(b+3)& 0xff)));
        curr=curr+4;
        b=b+4;
        vendorbit=(0x80&*b);
        curr=curr+1;
        b=b+1;
        lavpval=(((0x00 & 0xff) << 24) | ((*b & 0xff) << 16)| ((*(b+1) & 0xff) << 8) | ((*(b+2) & 0xff)))-8;
        //8=avp header length
        curr=curr+3;
        b=b+3;
        if(vendorbit!=0){
            vendorcode=(((*b& 0xff) << 24) | ((*(b+1) & 0xff) << 16)| ((*(b+2) & 0xff) << 8) | ((*(b+3) & 0xff)));
            curr=curr+4;
            b=b+4;
            lavpval=lavpval-4;  //actual avp value length
            l=l+4;
        }
        if(acode==avpcode&&vcode==vendorcode){
            b=b-l;
            padding=lavpval%4;
            if(padding!=0){
                padding=4-padding;
            }
            avp temp=avp(b,lavpval+padding+l);   //point to avp head
            a=avp(temp.copy(),temp.len);
            found=true;
            curr=curr-l;
        }else{
            //goto next avp
            b=b+lavpval;
            curr=curr+lavpval;
            padding=lavpval%4;
            if(padding!=0){
                padding=4-padding;
            }
            b=b+padding;
            curr=curr+padding;
        }
    }
    // current pointer is on next avp
    
    return a;
}

avp diameter::getAVP(int acode, int vcode){
    avp a=avp(0, 0);
    bool found=false;
    int avpcode;
    int vendorbit;
    int lavpval;
    int padding;
    int vendorcode;
    while (curr<len&&!found) {
        vendorcode=0;
        avpcode=(((*b& 0xff) << 24) | ((*(b+1) & 0xff) << 16)| ((*(b+2) & 0xff) << 8) | ((*(b+3)& 0xff)));
        curr=curr+4;
        b=b+4;
        vendorbit=(0x80&*b);
        curr=curr+1;
        b=b+1;
        lavpval=(((0x00 & 0xff) << 24) | ((*b & 0xff) << 16)| ((*(b+1) & 0xff) << 8) | ((*(b+2) & 0xff)))-8;
        //8=avp header length
        curr=curr+3;
        b=b+3;
        if(vendorbit!=0){
            vendorcode=(((*b& 0xff) << 24) | ((*(b+1) & 0xff) << 16)| ((*(b+2) & 0xff) << 8) | ((*(b+3) & 0xff)));
            curr=curr+4;
            b=b+4;
            lavpval=lavpval-4;  //actual avp value length
        }
        if(acode==avpcode&&vcode==vendorcode){
            a=avp(b,lavpval);   //point to avp val
            found=true;
        }
        //goto next avp
        b=b+lavpval;
        curr=curr+lavpval;
        padding=lavpval%4;
        if(padding!=0){
            padding=4-padding;
        }
        b=b+padding;
        curr=curr+padding;
    }
    // current pointer is on next avp
    
    return a;
}

