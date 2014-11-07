#ifndef EIDMW_TESTLIB_H
#define EIDMW_TESTLIB_H

#include <assert.h>
#include <stdio.h>

#define TEST_RV_SKIP 77 // defined by automake
#define TEST_RV_FAIL 1
#define TEST_RV_OK 0

extern int va_counter;
extern int fc_counter;

#ifndef TEST_NO_ABORT
#define my_assert(a) assert(a)
#else
#define my_assert(a) do { if((!a)) { printf("failed!\n"); return TEST_RV_FAIL; }} while(0)
#endif

#define verbose_assert(a) { printf("assertion %d: \"%s\": ", va_counter++, #a); my_assert((a)); printf("ok\n"); }

#ifdef __GNUC__
#define EIDT_LIKELY(expr) __builtin_expect((expr), 1)
#define EIDT_UNLIKELY(expr) __builtin_expect((expr), 0)
#else
#define EIDT_LIKELY(expr) (expr)
#define EIDT_UNLIKELY(expr) (expr)
#endif

typedef struct {
	CK_RV rv;
	int retval;
} ckrv_mod;

const static ckrv_mod m_p11_noinit[] = {
	{ CKR_OK, TEST_RV_FAIL },
	{ CKR_CRYPTOKI_NOT_INITIALIZED, TEST_RV_OK },
};

#define check_rv_late(func) { int retval = ckrv_decode(rv, func, 0, NULL); if(EIDT_UNLIKELY(retval != TEST_RV_OK)) { printf("not ok\n"); C_Finalize(NULL_PTR); my_assert(retval != TEST_RV_FAIL); return retval; }}
#define check_rv(call) check_rv_action(call, 0, NULL)
#define check_rv_action(call, count, mods) { CK_RV rv = call; int retval = ckrv_decode(rv, #call, count, mods); if(EIDT_UNLIKELY(retval != TEST_RV_OK)) { printf("not ok\n"); C_Finalize(NULL_PTR); my_assert(retval != TEST_RV_FAIL); return retval; }}
#define check_rv_long(call, mods) { int c = sizeof(mods) / sizeof(ckrv_mod); check_rv_action(call, c, mods); }

int ckrv_decode(CK_RV rv, char* fc, int count, const ckrv_mod*);

char* ckm_to_charp(CK_MECHANISM_TYPE);

#ifdef HAVE_CONFIG_H
#define TEST_FUNC(a) int main(void)
#else
#define TEST_FUNC(a) int a(void)
#endif

/* Verifies that a string does not contain a NULL character */
void verify_null(CK_UTF8CHAR* string, size_t length, int nulls_expected, char* msg);

/* Functions to work with card moving robots */
CK_BBOOL have_robot();
CK_BBOOL is_manual_robot();
CK_BBOOL can_confirm();
CK_BBOOL have_pin();
CK_BBOOL can_enter_pin();
void robot_remove_card();
void robot_remove_card_delayed();
void robot_insert_card();
void robot_insert_card_delayed();
void robot_remove_reader();
void robot_insert_reader();
void hex_dump(char* data, CK_ULONG length);

/* Helper functions to not have to repeat common operations all the time */
int find_slot(CK_BBOOL with_token, CK_SLOT_ID_PTR slot);

/* function definitions for tests that exist */
int init_finalize(void);
int getinfo();
int funclist();
int slotlist();
int slotinfo();
int tkinfo();
int double_init();
int fork_init();
int slotevent();
int mechlist();
int mechinfo();
int sessions();
int sessions_nocard();
int sessioninfo();
int login();
int nonsensible();
int objects();
int readdata();
int digest();
int threads();
int sign();
int decode_photo();
int ordering();

#endif
