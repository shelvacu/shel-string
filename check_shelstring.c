#include <check.h>
#include <stdint.h>
#include "shelstring.h"

#define LEN 8

START_TEST(test_a){
  int tests[LEN] = {0, 1, 5, 127,128,255,16383,16384};
  short res[LEN] = {1, 1, 1,   1,  2,  2,    2,    3};
  for(int i=0;i<LEN;i++){
    ck_assert_int_eq(res[i],shelint_length_from_cint(tests[i]));
  }
}END_TEST;

START_TEST(wat){
  ck_assert_int_eq(1,2);
}END_TEST;

int main(void){
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_a);
  tcase_add_test(tc1_1, wat);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
} 
