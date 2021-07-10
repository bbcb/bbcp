/* this ALWAYS GENERATED file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 2.00.0102 */
/* at Wed Oct 30 17:25:28 1996
 */
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef IID_DEFINED
#define IID_DEFINED

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // IID_DEFINED

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ILookup = {0xc4910d71,0xba7d,0x11cd,{0x94,0xe8,0x08,0x00,0x17,0x01,0xa8,0xa3}};


#ifdef __cplusplus
}
#endif

