// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/bits/defs.h>
#include <openenclave/bits/safecrt.h>
#include <openenclave/internal/raise.h>
// #include <openenclave/internal/report.h>
// #include <openenclave/internal/sgxtypes.h>
#include <openenclave/internal/utils.h>

#include <openenclave/internal/enclavelibc.h>
//#include "../common/common.h"
// #include <openenclave/internal/print.h>
// #include <stdio.h>

// Using mbedtls to create an extended X.509 certificate
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/certs.h>
#include <mbedtls/x509.h>
#include <mbedtls/ssl.h>
#include <mbedtls/error.h>
#include <mbedtls/debug.h>
#include <mbedtls/oid.h>
#include <mbedtls/sha256.h>
#include <mbedtls/x509_crt.h>

static unsigned char oid_oe_report[] = {0x2A, 0x86, 0x48, 0x86, 0xF8, 0x4D, 0x8A, 0x39, 0x01};
static int _extract_x509_extension
(   uint8_t* ext3_data,
    size_t exts_data_len,
    const uint8_t* report_oid,
    size_t report_oid_len,
    uint8_t** report_data,
    size_t* report_data_size
)
{
    int ret = 1;
    unsigned char *p = NULL;
    const unsigned char *end = NULL;
    mbedtls_x509_buf oid = {0, 0, NULL};
    size_t len = 0;

 
//TODO: 
// Should make this extension a critical one!
// if( ( ret = mbedtls_x509_get_ext( p, end, &crt->v3_ext, 3 ) ) != 0 )
// {
//     if( ret == MBEDTLS_ERR_ASN1_UNEXPECTED_TAG )
//         return( 0 );
//     return( ret );
// }

    p = (unsigned char *)ext3_data + 83; // need to find out why it;s 83!
    end = p + exts_data_len;

    // Search for target report OID
    while (p < end)
    {
        // Get extension OID ID
        if( ( ret = mbedtls_asn1_get_tag( &p, end, &oid.len,
                                            MBEDTLS_ASN1_OID ) ) != 0 )
            return( MBEDTLS_ERR_X509_INVALID_EXTENSIONS + ret );

        if (oid.len == report_oid_len)
        {
            oid.tag = MBEDTLS_ASN1_OID;
            oid.p = p;

            if (0 == oe_memcmp(oid.p, report_oid, report_oid_len)) 
            {
                p += report_oid_len;
                // Read the octet string tag, length encoded in two bytes
                ret = mbedtls_asn1_get_tag( &p, end, &len, MBEDTLS_ASN1_OCTET_STRING);
                if (ret)
                {
                    OE_TRACE_ERROR("ret=%d", ret);
                    goto done;
                }
                *report_data = p;
                *report_data_size = len;
                OE_TRACE_INFO("report_data_size = %d", *report_data_size);
                OE_TRACE_INFO("report_data = %p report_data[0]=0x%x  report_data_size=%d", *report_data, **report_data, *report_data_size);
                ret = 0;
                break;
            }
        }
        *p += oid.len;
    }
done:
    if (ret)
        OE_TRACE_ERROR("Expected x509 report extension not found");

    return ret;
}

static oe_result_t extract_x509_report_extension
(   mbedtls_x509_crt *crt,
    uint8_t** report_data,
    size_t* report_data_size
)
{
    oe_result_t result = OE_FAILURE;
    int ret = 0;

    ret = _extract_x509_extension(crt->v3_ext.p,
                                crt->v3_ext.len,
                                oid_oe_report,
                                sizeof(oid_oe_report),
                                report_data,
                                report_data_size);
    if (ret)
        OE_RAISE(OE_FAILURE, "ret = %d", ret);

    OE_TRACE_INFO("report_data = %p report_data[0]=0x%x report_data_size=%d", *report_data, **report_data, *report_data_size);
    result = OE_OK;

done:
    return result;
}

oe_result_t oe_verify_tls_cert( uint8_t* cert_in_der, size_t cert_in_der_len, 
                                tls_cert_verify_callback_t verify_enclave_identity_info_callback)
{
    oe_result_t result = OE_FAILURE;
    uint8_t* report = NULL;
    size_t report_size = 0;
    oe_report_t parsed_report = {0};
    int ret;    
    mbedtls_x509_crt crt;
    mbedtls_x509_crt_init(&crt);

    // create a mbedtls cert object from encoded cert data in DER format
    ret = mbedtls_x509_crt_parse(&crt, cert_in_der, cert_in_der_len);
    if (ret)
        OE_RAISE_MSG(OE_FAILURE, "ret = %d", ret);

    OE_CHECK(extract_x509_report_extension(&crt, &report, &report_size));

    OE_TRACE_INFO("extract_x509_report_extension() succeeded");
    OE_TRACE_INFO("report = %p report[0]=0x%x report_size=%d", report, *report, report_size);

    // 1)  Validate the report's trustworthiness
    // Verify the remote report to ensure its authenticity.
    // set enclave to NULL because we are dealing only with remote report now

    result = oe_verify_report(report, report_size, &parsed_report);
    OE_CHECK(result);
    OE_TRACE_INFO("oe_verify_report() succeeded");

    if (verify_enclave_identity_info_callback)
    {
        result = verify_enclave_identity_info_callback(&parsed_report);
        OE_CHECK(result);
        OE_TRACE_INFO("verify_enclave_identity_info_callback() succeeded");
    }
    else
    {
        OE_TRACE_WARNING("No verify_enclave_identity_info_callback provided in oe_verify_tls_cert call", NULL);
    }
done:
    mbedtls_x509_crt_free(&crt);
    return result;
}