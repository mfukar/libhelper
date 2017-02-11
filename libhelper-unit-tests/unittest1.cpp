#include "stdafx.h"
#include "CppUnitTest.h"

#include "../libhelper/aho-corasick.h"
#include "../libhelper/overflow.h"
#include "../libhelper/kmp.h"
#include "../libhelper/queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libhelperunittests {
	TEST_CLASS (UnitTest1) {
	public:
		TEST_METHOD(test_32bit_signed_addition) {
			int32_t a = INT32_MIN, b = INT32_MIN;
			int32_t c = 0;
			Assert::IsTrue (checked_add_32 (a, b, &c));
		}

		TEST_METHOD (test_kmp_search) {
			const char *barn = "Loremipsumdolorsitamet,consecteturadipisicingelit,seddoeiusmodtemporincididuntutlaboreetdoloremagnaaliqua.Utenimadminimveniam,quisnostrudexercitationullamcolaborisnisiutaliquipexeacommodoconsequat.Duisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur.Excepteursintoccaecatcupidatatnonproident,suntinculpaquiofficiadeseruntmollitanimidestlaborum.";
			const char test_success[] = "tempor";
			const char test_fail[] = "participate in parachute";

			size_t ff1[sizeof test_success] = { 0 };

			kmp_ff (test_success, ff1, sizeof ff1 / sizeof *ff1);
			Assert::IsTrue (kmp (barn, ::strlen (barn), test_success, ::strlen (test_success), ff1) == ::strstr (barn, test_success) - barn);

			size_t ff2[sizeof test_fail] = { 0 };

			kmp_ff (test_fail, ff2, sizeof ff2 / sizeof *ff2);
			Assert::IsTrue (kmp (barn, ::strlen (barn), test_fail, ::strlen (test_fail), ff2) == -1);
		}

		TEST_METHOD (test_dequeue) {
			struct ac_state s0, s1, s2;
			s0.id = 0; s1.id = 1; s2.id = 2;

			dequeue (q1);
			ff_queue_element n0, n1, n2;
			n0.s = &s0; n1.s = &s1; n2.s = &s2;
			
			dequeue_enqueue (&q1, &n0.list);
			dequeue_enqueue (&q1, &n1.list);
			dequeue_enqueue (&q1, &n2.list);

			for (; !dequeue_is_empty (&q1); ) {
				struct ff_queue_element * t = dequeue_data (dequeue_pop_front (&q1), struct ff_queue_element, list);
				printf ("%zu\n", t->s->id);
			}
		}
	};
}
