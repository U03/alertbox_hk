#ifndef _CERTIFICATES_H_
#define _CERTIFICATES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* This file is auto-generated by the pycert_bearssl tool.  Do not change it manually.
 * Certificates are BearSSL br_x509_trust_anchor format.  Included certs:
 *
 * Index:    0
 * Label:    takeshiba.u03.fr
 * Subject:  1.2.840.113549.1.9.1=takeshiba@u03.fr,CN=takeshiba.u03.fr,O=YURIKAMOME,L=PARIS,ST=FRANCE,C=FR
 * Type:     Certificate Authority
 */

#define TAs_NUM 1

static const unsigned char TA_DN0[] = {
    0x30, 0x7f, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x46, 0x52, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04, 0x08,
    0x13, 0x06, 0x46, 0x52, 0x41, 0x4e, 0x43, 0x45, 0x31, 0x0e, 0x30, 0x0c,
    0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x05, 0x50, 0x41, 0x52, 0x49, 0x53,
    0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x0a, 0x59,
    0x55, 0x52, 0x49, 0x4b, 0x41, 0x4d, 0x4f, 0x4d, 0x45, 0x31, 0x19, 0x30,
    0x17, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x10, 0x74, 0x61, 0x6b, 0x65,
    0x73, 0x68, 0x69, 0x62, 0x61, 0x2e, 0x75, 0x30, 0x33, 0x2e, 0x66, 0x72,
    0x31, 0x1f, 0x30, 0x1d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x09, 0x01, 0x16, 0x10, 0x74, 0x61, 0x6b, 0x65, 0x73, 0x68, 0x69,
    0x62, 0x61, 0x40, 0x75, 0x30, 0x33, 0x2e, 0x66, 0x72,
};

static const unsigned char TA_RSA_N0[] = {
    0xed, 0x18, 0x69, 0x61, 0x7a, 0x93, 0xed, 0xc0, 0x14, 0xe4, 0x7e, 0xd1,
    0xcb, 0x3d, 0x7d, 0xc0, 0xd9, 0xc7, 0x60, 0x38, 0xd7, 0xf0, 0x54, 0x84,
    0x63, 0x43, 0x70, 0x63, 0x17, 0xd3, 0x18, 0x12, 0x02, 0x51, 0x63, 0xe3,
    0x3b, 0xda, 0xa2, 0x25, 0x55, 0xd7, 0x13, 0x6a, 0x39, 0x3a, 0x8f, 0x3a,
    0xcb, 0xb1, 0x81, 0x3d, 0x98, 0x5f, 0x3a, 0x5c, 0xd3, 0x37, 0x7b, 0x08,
    0x27, 0x3a, 0xd2, 0x93, 0x75, 0x5a, 0xac, 0x1e, 0xe2, 0x1d, 0xb6, 0x64,
    0xbe, 0xf9, 0x16, 0x8a, 0x63, 0xe1, 0xf0, 0xbb, 0xe4, 0x46, 0x13, 0xb8,
    0xb4, 0x88, 0xc3, 0xcf, 0x88, 0x58, 0xeb, 0xe8, 0xbe, 0x8a, 0x87, 0x6e,
    0xa5, 0x5d, 0xa7, 0xa9, 0xcb, 0xe7, 0x0f, 0xca, 0x27, 0x09, 0x63, 0x84,
    0x76, 0xbd, 0x6a, 0xcd, 0x7b, 0x9f, 0x30, 0x9a, 0x66, 0x63, 0xd3, 0x27,
    0xa7, 0x88, 0x0b, 0x5b, 0x17, 0x48, 0x7e, 0xff, 0xfe, 0x96, 0xfd, 0xae,
    0x4a, 0x0d, 0x7d, 0xf3, 0x43, 0xc9, 0x10, 0x51, 0xf6, 0x80, 0x7c, 0xde,
    0xd9, 0x91, 0x18, 0x33, 0xee, 0x3d, 0xde, 0x3d, 0xc0, 0xaf, 0xda, 0x49,
    0x80, 0x2d, 0xef, 0x28, 0x15, 0xeb, 0xfe, 0x52, 0x2f, 0xc9, 0x46, 0x21,
    0x18, 0xbf, 0xb5, 0xa7, 0xf4, 0x16, 0x80, 0xf9, 0x57, 0x53, 0x53, 0xed,
    0x67, 0x45, 0x75, 0x45, 0xcb, 0xfd, 0x0e, 0x11, 0xe1, 0xb2, 0x03, 0xe9,
    0x8d, 0x92, 0xb6, 0x04, 0x12, 0xa1, 0x6c, 0x9c, 0x98, 0x69, 0xe2, 0x98,
    0x1c, 0xd9, 0x7a, 0xd8, 0xd2, 0xfc, 0xc5, 0xaf, 0xee, 0xb3, 0xdf, 0x9d,
    0x7f, 0xab, 0x3a, 0x0f, 0x89, 0x28, 0x66, 0x68, 0xa4, 0x9d, 0x15, 0xe2,
    0xb0, 0x40, 0xdd, 0xbb, 0x27, 0xa3, 0x90, 0x15, 0x43, 0x3b, 0x15, 0xe8,
    0xbf, 0xd6, 0x38, 0xec, 0xa3, 0x25, 0x6c, 0x8e, 0xb7, 0xd3, 0xf7, 0xb4,
    0xd9, 0xee, 0xac, 0xa7,
};

static const unsigned char TA_RSA_E0[] = {
    0x01, 0x00, 0x01,
};

static const br_x509_trust_anchor TAs[] = {
    {
        { (unsigned char *)TA_DN0, sizeof TA_DN0 },
        BR_X509_TA_CA,
        {
            BR_KEYTYPE_RSA,
            { .rsa = {
                (unsigned char *)TA_RSA_N0, sizeof TA_RSA_N0,
                (unsigned char *)TA_RSA_E0, sizeof TA_RSA_E0,
            } }
        }
    },
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _CERTIFICATES_H_ */
