#include <unix.h>
#include <pkcs11.h>
#include <stdio.h>
#include <string.h>

#include "testlib.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_OPENSSL
#include <openssl/rsa.h>
#include <openssl/engine.h>

CK_BYTE digest_sha1[] = {
                0x0b, 0xee, 0xc7, 0xb5,
                0xea, 0x3f, 0x0f, 0xdb,
                0xc9, 0x5d, 0x0d, 0xd4,
                0x7f, 0x3c, 0x5b, 0xc2,
                0x75, 0xda, 0x8a, 0x33
};

int verify_sig(char* sig, CK_ULONG siglen, CK_BYTE_PTR modulus, CK_ULONG modlen, CK_BYTE_PTR exponent, CK_ULONG explen) {
	RSA* rsa = RSA_new();
	unsigned char* s = malloc(siglen);
	int ret;

	rsa->n = BN_bin2bn(modulus, (int) modlen, NULL);
	rsa->e = BN_bin2bn(exponent, (int) explen, NULL);

	int v = RSA_verify(NID_sha1, digest_sha1, sizeof(digest_sha1), sig, siglen, rsa);

	printf("Signature verification returned: %d\n", v);
	if(!v) {
		unsigned long e = ERR_get_error();
		printf("error %ld: %s\n", e, ERR_error_string(e, NULL));
		ret = TEST_RV_FAIL;
	} else {
		ret = TEST_RV_OK;
	}

	free(s);
	RSA_free(rsa);

	return ret;
}

#endif

TEST_FUNC(sign) {
	int ret;
	CK_SESSION_HANDLE session;
	CK_MECHANISM mech;
	CK_BYTE data[] = { 'f', 'o', 'o' };
	CK_SLOT_ID slot;
	CK_BYTE_PTR sig, mod, exp;
	CK_ULONG sig_len, type, count;
	CK_OBJECT_HANDLE object;
	ckrv_mod m[] = {
		{ CKR_PIN_INCORRECT, TEST_RV_SKIP },
		{ CKR_FUNCTION_CANCELED, TEST_RV_SKIP },
	};
	CK_ATTRIBUTE attr[2];
	int i;

	if(!have_pin()) {
		fprintf(stderr, "Cannot test signature without a pin code\n");
		return TEST_RV_SKIP;
	}

	check_rv(C_Initialize(NULL_PTR));

	if((ret = find_slot(CK_TRUE, &slot)) != TEST_RV_OK) {
		return ret;
	}

	check_rv(C_OpenSession(slot, CKF_SERIAL_SESSION, NULL_PTR, NULL_PTR, &session));

	if(!can_enter_pin(slot)) {
		return TEST_RV_SKIP;
	}

	check_rv_long(C_Login(session, CKU_USER, NULL_PTR, 0), m);

	attr[0].type = CKA_CLASS;
	attr[0].pValue = &type;
	type = CKO_PRIVATE_KEY;
	attr[0].ulValueLen = sizeof(CK_ULONG);

	attr[1].type = CKA_LABEL;
	attr[1].pValue = "Signature";
	attr[1].ulValueLen = strlen("Signature");

	check_rv(C_FindObjectsInit(session, attr, 2));
	check_rv(C_FindObjects(session, &object, 1, &count));
	verbose_assert(count == 1);
	check_rv(C_FindObjectsFinal(session));

	mech.mechanism = CKM_SHA1_RSA_PKCS;
	check_rv(C_SignInit(session, &mech, object));

	check_rv(C_Sign(session, data, sizeof(data), NULL, &sig_len));
	sig = malloc(sig_len);
	check_rv(C_Sign(session, data, sizeof(data), sig, &sig_len));

	printf("Received a signature with length %lu:\n", sig_len);

	hex_dump(sig, sig_len);

	type = CKO_PUBLIC_KEY;
	check_rv(C_FindObjectsInit(session, attr, 2));
	check_rv(C_FindObjects(session, &object, 1, &count));
	verbose_assert(count == 1);
	check_rv(C_FindObjectsFinal(session));

	attr[0].type = CKA_MODULUS;
	attr[0].pValue = NULL_PTR;
	attr[0].ulValueLen = 0;

	attr[1].type = CKA_PUBLIC_EXPONENT;
	attr[1].pValue = NULL_PTR;
	attr[1].ulValueLen = 0;

	check_rv(C_GetAttributeValue(session, object, attr, 2));

	verbose_assert(attr[0].ulValueLen == sig_len);

#if HAVE_OPENSSL
	mod = malloc(attr[0].ulValueLen);
	mod[0] = 0xde; mod[1] = 0xad; mod[2] = 0xbe; mod[3] = 0xef;
	exp = malloc(attr[1].ulValueLen);
	exp[0] = 0xde; exp[1] = 0xad; exp[2] = 0xbe; exp[3] = 0xef;

	attr[0].pValue = mod;
	attr[1].pValue = exp;

	check_rv(C_GetAttributeValue(session, object, attr, 2));

	printf("Received key modulus with length %lu:\n", attr[0].ulValueLen);
	hex_dump(mod, attr[0].ulValueLen);

	printf("Received public exponent of key with length %lu:\n", attr[1].ulValueLen);
	hex_dump(exp, attr[1].ulValueLen);

	if((ret = verify_sig(sig, sig_len, mod, attr[0].ulValueLen, exp, attr[1].ulValueLen)) != TEST_RV_OK) {
		return ret;
	}
#endif

	check_rv(C_Finalize(NULL_PTR));

	return TEST_RV_OK;
}
