#include "random.h"
#include "mcuxClEls.h"
#include "mbedtls/ctr_drbg.h"

static mbedtls_ctr_drbg_context ctr_drbg;

bool random_init(void)
{
	bool status = false;
	uint8_t seed[32] = {0};
	static mbedtls_entropy_context entropy;

	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	//////////////////////// Initialize ELS, Enable the ELS ///////////////////////////////////////////
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Enable_Async());
    // mcuxClEls_Enable_Async is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Enable_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    // Wait for the mcuxClEls_Enable_Async operation to complete.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    // mcuxClEls_WaitForOperation is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    // Reset ELS
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Reset_Async(MCUXCLELS_RESET_DO_NOT_CANCEL));
    // mcuxClEls_Reset_Async is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Reset_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    // Wait for the mcuxClEls_Reset_Async operation to complete.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    // mcuxClEls_WaitForOperation is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    //////////////////////// Initialize the RNG ///////////////////////////////////////////
    // The RNG needs to be initialized; this can be done by calling mcuxClEls_KeyDelete_Async
	// to delete any key slot which can even be empty. Deleted keySlot 0
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_KeyDelete_Async(0));
    // mcuxClEls_KeyDelete_Async is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_KeyDelete_Async) != token) || (MCUXCLELS_STATUS_OK_WAIT != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    // Wait for the mcuxClEls_KeyDelete_Async operation to complete.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_WaitForOperation(MCUXCLELS_ERROR_FLAGS_CLEAR));
    // mcuxClEls_WaitForOperation is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_WaitForOperation) != token) || (MCUXCLELS_STATUS_OK != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    // Fill the seed buffer with random data from PRNG.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Prng_GetRandom(seed, sizeof(seed)));
    // mcuxClEls_Prng_GetRandom is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Prng_GetRandom) != token) || (MCUXCLELS_STATUS_OK != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

	// Disable the ELS.
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClEls_Disable());
    // mcuxClEls_Disable is a flow-protected function: Check the protection token and the return value
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_Disable) != token) || (MCUXCLELS_STATUS_OK != result))
    {
    	goto end;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    status = (0 == mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, seed, sizeof(seed)));

end:
    return status;
}

bool random_get(uint8_t *buffer, size_t size)
{
	return (0 == mbedtls_ctr_drbg_random(&ctr_drbg, buffer, size));
}
