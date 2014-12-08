#ifndef _DES_H
#define _DES_H

#include <string.h>
#include<stdio.h>
#include<stdlib.h>

/* desº”Ω‚√‹À„∑® */
// DES ADD
void des(unsigned char *text,unsigned char *mtext,unsigned char *key);
void undes(unsigned char *text,unsigned char *mtext,unsigned char *key);
void expand0(unsigned char *in,unsigned char *out);
void ip(unsigned char *text,char *ll,char *rr);
void _ip(unsigned char *text,char *ll,char *rr);
void compress0(unsigned char *out,unsigned char *in);
void compress016(char *out,unsigned char *in);
void LS(unsigned char *bits,unsigned char *buffer,int count);
void son(unsigned char *cc,unsigned char *dd,unsigned char *kk);
void s_box(char *aa,char *bb);



#endif
