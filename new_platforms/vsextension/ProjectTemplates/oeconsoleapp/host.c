#include <openenclave/host.h>
#include <stdio.h>

// Include the untrusted header that is generated during the build.
//#include "$projectname$_u.h"

int main(int argc, const char* argv[])
{
#if 0
    oe_result_t result;
#endif
    int ret = 1;
    oe_enclave_t* enclave = NULL;
    uint32_t enclave_flags = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s enclave_image_path\n", argv[0]);
        goto exit;
    }

    // Create the enclave
#ifdef _DEBUG
    enclave_flags |= OE_ENCLAVE_FLAG_DEBUG;
#endif
#if 0
    result = oe_create_$safeprojectname$_enclave(
        argv[1], OE_ENCLAVE_TYPE_UNDEFINED, enclave_flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_create_enclave(): result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }

    // Call into the enclave
    result = ecall_DoWorkInEnclave(enclave);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "calling into ecall_DoWorkInEnclave failed: result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }
#endif

    ret = 0;

exit:
    // Clean up the enclave if we created one
    if (enclave)
        oe_terminate_enclave(enclave);

    return ret;
}
