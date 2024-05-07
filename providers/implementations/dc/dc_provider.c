#include <openssl/core_dispatch.h>
#include <openssl/provider.h>
#include <openssl/crypto.h>  // For OPENSSL_zalloc and OPENSSL_free
#include "dc.h"
#include <string.h>  // For memcpy

static void *dc_newctx(void *provctx) {
    DC_CIPHER_CTX *ctx = OPENSSL_zalloc(sizeof(*ctx));
    if (ctx == NULL) return NULL;
    return ctx;
}

static void dc_freectx(void *vctx) {
    DC_CIPHER_CTX *ctx = (DC_CIPHER_CTX *)vctx;
    OPENSSL_free(ctx);
}

static int dc_encrypt_init(void *vctx, const unsigned char *key, size_t keylen, const unsigned char *iv, size_t ivlen) {
    DC_CIPHER_CTX *ctx = (DC_CIPHER_CTX *)vctx;
    memcpy(ctx->key, key, BLOCK_SIZE);
    return 1;
}

static int dc_cipher_update(void *vctx, unsigned char *out, size_t *outl, size_t outsize, const unsigned char *in, size_t inl) {
    DC_CIPHER_CTX *ctx = (DC_CIPHER_CTX *)vctx;
    if (outsize < inl) return 0;
    encrypt_block(in, out, ctx->key);
    *outl = inl;
    return 1;
}

static const OSSL_DISPATCH dc_cipher_functions[] = {
    { OSSL_FUNC_CIPHER_NEWCTX, (void (*)(void))dc_newctx },
    { OSSL_FUNC_CIPHER_FREECTX, (void (*)(void))dc_freectx },
    { OSSL_FUNC_CIPHER_ENCRYPT_INIT, (void (*)(void))dc_encrypt_init },
    { OSSL_FUNC_CIPHER_CIPHER, (void (*)(void))dc_cipher_update },
    { 0, NULL }
};

static const OSSL_ALGORITHM dc_algorithms[] = {
    { "DC=provider:dcprovider", NULL, dc_cipher_functions },
    { NULL, NULL, NULL }
};

int OSSL_provider_init(const OSSL_CORE_HANDLE *handle, const OSSL_DISPATCH *in, const OSSL_DISPATCH **out, void **provctx) {
    *out = dc_algorithms[0].implementation; 
    return 1;
}

