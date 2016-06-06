#include <assert.h>

#include "stdafx.h"
#include "overflow.h"
#include "kmp.h"
#include "aho-corasick.h"

struct num {
	size_t val;
	dequeue list;
};

int _tmain (int argc, TCHAR *argv[]) {
	int32_t a = INT32_MIN, b = INT32_MIN;
	int32_t c = 0;
	assert (true == checked_add_32 (a, b, &c));

	/*====================*/
	const char *barn = "Loremipsumdolorsitamet,consecteturadipisicingelit,seddoeiusmodtemporincididuntutlaboreetdoloremagnaaliqua.Utenimadminimveniam,quisnostrudexercitationullamcolaborisnisiutaliquipexeacommodoconsequat.Duisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur.Excepteursintoccaecatcupidatatnonproident,suntinculpaquiofficiadeseruntmollitanimidestlaborum.";
	const char test_success[] = "tempor";
	const char test_fail[] = "participate in parachute";

	int ff1[sizeof test_success] = {0};

	kmp_ff (test_success, ff1, sizeof ff1 / sizeof *ff1);
	assert (kmp (barn, test_success, ff1) == strstr(barn, test_success) - barn);

	int ff2[sizeof test_fail] = {0};

	kmp_ff (test_fail, ff2, sizeof ff2 / sizeof *ff2);
	assert (kmp (barn, test_fail, ff2) == -1);

	/*======================*/
	dequeue (q1);
	struct num n0, n1, n2;
	n0.val = 0; n1.val = 1; n2.val = 2;
	dequeue_enqueue (&q1, &n0.list);
	dequeue_enqueue (&q1, &n1.list);
	dequeue_enqueue (&q1, &n2.list);

	for (; !dequeue_is_empty (&q1); ) {
		struct num *t = dequeue_data (dequeue_pop_front (&q1), struct num, list);
		printf ("%zu\n", t->val);
	}

	/*======================*/

	struct ac_trie trie;
	ac_trie_init (&trie);
	ac_trie_insert (&trie, "she");
	ac_trie_insert (&trie, "hers");
	ac_trie_insert (&trie, "he");
	ac_build_failure_function (&trie);
	ac_result res = ac_search (&trie, "head of hers, OK?",
							   sizeof "head of hers, OK?", 0);
	printf ("%zu %zu %zu", res.id, res.start, res.end);
	for (size_t i = res.start; i < res.end; ++i) {
		printf ("%c", "head of hers, OK?"[i]);
	}
	return 0;
}
