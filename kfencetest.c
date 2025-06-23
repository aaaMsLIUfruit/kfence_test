#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>

#include <nuttx/fs/procfs.h>
#include <nuttx/mm/mm.h>
#include <nuttx/mm/kfence.h>

#include "cwe416/cwe416_tests.h"
#include "cwe415/cwe415_tests.h"
#include "cwe122/cwe122_tests.h"

/****************************************************************************
 * CWE416 测试用例数组
 ****************************************************************************/

typedef int (*cwe416_test_func)(void);

static cwe416_test_func g_cwe416_tests[] = {
    test_cwe416_malloc_free_char_01, test_cwe416_malloc_free_char_02, test_cwe416_malloc_free_char_03,
    test_cwe416_malloc_free_char_04, test_cwe416_malloc_free_char_05, test_cwe416_malloc_free_char_06,
    test_cwe416_malloc_free_char_07, test_cwe416_malloc_free_char_08, test_cwe416_malloc_free_char_09,
    test_cwe416_malloc_free_char_10, test_cwe416_malloc_free_char_11, test_cwe416_malloc_free_char_12,
    test_cwe416_malloc_free_char_13, test_cwe416_malloc_free_char_14, test_cwe416_malloc_free_char_15,
    test_cwe416_malloc_free_char_16, test_cwe416_malloc_free_char_17, test_cwe416_malloc_free_char_18,
    test_cwe416_malloc_free_struct_01, test_cwe416_malloc_free_struct_02, test_cwe416_malloc_free_struct_03,
    test_cwe416_malloc_free_struct_04, test_cwe416_malloc_free_struct_05, test_cwe416_malloc_free_struct_06,
    test_cwe416_malloc_free_struct_07, test_cwe416_malloc_free_struct_08, test_cwe416_malloc_free_struct_09,
    test_cwe416_malloc_free_struct_10, test_cwe416_malloc_free_struct_11, test_cwe416_malloc_free_struct_12,
    test_cwe416_malloc_free_struct_13, test_cwe416_malloc_free_struct_14, test_cwe416_malloc_free_struct_15,
    test_cwe416_malloc_free_struct_16, test_cwe416_malloc_free_struct_17, test_cwe416_malloc_free_struct_18,
    test_cwe416_malloc_free_long_01, test_cwe416_malloc_free_long_02, test_cwe416_malloc_free_long_03,
    test_cwe416_malloc_free_long_04, test_cwe416_malloc_free_long_05, test_cwe416_malloc_free_long_06,
    test_cwe416_malloc_free_long_07, test_cwe416_malloc_free_long_08, test_cwe416_malloc_free_long_09,
    test_cwe416_malloc_free_long_10, test_cwe416_malloc_free_long_11, test_cwe416_malloc_free_long_12,
    test_cwe416_malloc_free_long_13, test_cwe416_malloc_free_long_14, test_cwe416_malloc_free_long_15,
    test_cwe416_malloc_free_long_16, test_cwe416_malloc_free_long_17, test_cwe416_malloc_free_long_18,
    test_cwe416_malloc_free_int64_t_01, test_cwe416_malloc_free_int64_t_02, test_cwe416_malloc_free_int64_t_03,
    test_cwe416_malloc_free_int64_t_04, test_cwe416_malloc_free_int64_t_05, test_cwe416_malloc_free_int64_t_06,
    test_cwe416_malloc_free_int64_t_07, test_cwe416_malloc_free_int64_t_08, test_cwe416_malloc_free_int64_t_09,
    test_cwe416_malloc_free_int64_t_10, test_cwe416_malloc_free_int64_t_11, test_cwe416_malloc_free_int64_t_12,
    test_cwe416_malloc_free_int64_t_13, test_cwe416_malloc_free_int64_t_14, test_cwe416_malloc_free_int64_t_15,
    test_cwe416_malloc_free_int64_t_16, test_cwe416_malloc_free_int64_t_17, test_cwe416_malloc_free_int64_t_18,
    test_cwe416_return_freed_ptr_01, test_cwe416_return_freed_ptr_02, test_cwe416_return_freed_ptr_03,
    test_cwe416_return_freed_ptr_04, test_cwe416_return_freed_ptr_05, test_cwe416_return_freed_ptr_06,
    test_cwe416_return_freed_ptr_07, test_cwe416_return_freed_ptr_08, test_cwe416_return_freed_ptr_09,
    test_cwe416_return_freed_ptr_10, test_cwe416_return_freed_ptr_11, test_cwe416_return_freed_ptr_12,
    test_cwe416_return_freed_ptr_13, test_cwe416_return_freed_ptr_14, test_cwe416_return_freed_ptr_15,
    test_cwe416_return_freed_ptr_16, test_cwe416_return_freed_ptr_17, test_cwe416_return_freed_ptr_18
};

/****************************************************************************
 * 结构体声明
 ****************************************************************************/
typedef struct kfencetest_s
{
  bool (*func)(FAR struct mm_heap_s *heap, size_t size);  // 测试函数指针
  bool is_auto;                                           // 是否自动运行
  FAR const char *name;  
} kfencetest_t;

typedef struct run_s
{
  char argv[32];                      // 参数字符串
  FAR const testcase_t *testcase;     // 测试用例
  FAR struct mm_heap_s *heap;         // 堆指针
  size_t size;                        // 分配大小
} run_t;


/****************************************************************************
 * 测试用例声明
 ****************************************************************************/
//左越界
static bool test_heap_underflow(size_t size);
//右越界
static bool test_heap_overflow(size_t size);
//释放后使用
static bool test_heap_use_after_free(size_t size);
//双重释放
static bool test_heap_double_free(size_t size);
//memcpy目标地址非法或越界
static bool test_heap_illegal_memcpy(FAR struct mm_heap_s *heap, size_t size);
//memset目标地址非法或越界
static bool test_heap_illegal_memset(size_t size);
//strcpy目的缓冲区溢出
static bool test_heap_illegal_strcpy(size_t size);
//释放无效地址
static bool test_invalid_free_address(size_t size);
//realloc非kfence内存
static bool test_invalid_realloc_non_kfence(size_t size);
//realloc到无效地址
static bool test_invalid_realloc_to_invalid_address(size_t size);
//realloc到已释放地址
static bool test_invalid_realloc_to_freed_address(size_t size);

//对照组——验证不报错
static bool test_heap_legal_memcpy(size_t size);
static bool test_heap_legal_memset(size_t size);
static bool test_heap_legal_strcpy(size_t size);

/****************************************************************************
 * 朱丽叶测试用例
 ****************************************************************************/
// CWE416 测试适配函数
static bool test_cwe416_use_after_free(size_t size);

// CWE415 测试适配函数
static bool test_cwe415_double_free(size_t size);

// CWE122 测试适配函数
static bool test_cwe122_heap_overflow(size_t size);

 /****************************************************************************
 * 测试用例数组
 ****************************************************************************/
const static kfencetest_t g_kfence_test[] =
{
  {test_heap_underflow, true, "heap underflow"},
  {test_heap_overflow, true, "heap overflow"},
  {test_heap_use_after_free, true, "heap use after free"},
  {test_heap_double_free, true, "heap double free"},
  {test_heap_illegal_memcpy, true, "heap illegal memcpy"},
  {test_heap_illegal_memset, true, "heap illegal memset"},
  {test_heap_illegal_strcpy, true, "heap illegal strcpy"},
  {test_heap_legal_memcpy, true, "heap legal memcpy"},
  {test_heap_legal_memset, true, "heap legal memset"},
  {test_heap_legal_strcpy, true, "heap legal strcpy"},
  {test_invalid_free_address, true, "invalid free address"},
  {test_invalid_realloc_non_kfence, true, "invalid realloc non kfence"},
  {test_invalid_realloc_to_invalid_address, true, "invalid realloc to invalid address"},
  {test_invalid_realloc_to_freed_address, true, "invalid realloc to freed address"},
  {test_cwe416_use_after_free, true, "CWE416 use after free"},
  {test_cwe415_double_free, true, "CWE415 double free"},
  {test_cwe122_heap_overflow, true, "CWE122 heap overflow"}
};

//测试专用内存池
static char g_kfence_heap[10240] aligned_data(8);
/****************************************************************************
 * 辅助函数
 ****************************************************************************/
//命令行输入错误后引导
 static void error_handler(void)
{
  int i;
  printf("Usage: kfence_test [-h] [case_number]\n");
  printf("options:\n-h: show this help message\n");
  printf("case_number:\n");
  for (i = 0; i < nitems(g_kfence_test); i++) {
    printf("%d: %s\n", i + 1, g_kfence_test[i].name);
  }
}
//计算时间差
static void timespec_sub(struct timespec *dest,
                         struct timespec *ts1,
                         struct timespec *ts2)
{
  dest->tv_sec = ts1->tv_sec - ts2->tv_sec;
  dest->tv_nsec = ts1->tv_nsec - ts2->tv_nsec;

  if (dest->tv_nsec < 0)
    {
      dest->tv_nsec += 1000000000;
      dest->tv_sec -= 1;
    }
}
/****************************************************************************
 * 各种测试函数
 ****************************************************************************/
static bool test_heap_underflow(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  *(mem - 1) = 0x12;
  return false;
}

static bool test_heap_overflow(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  size = kfence_alloc_size(mem);

  mem[size + 1] = 0x11;
  return false;
}

static bool test_heap_use_after_free(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);

  kfence_free(mem);
  mem[0] = 0x10;
  return false;
}


static bool test_heap_double_free(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  kfence_free(mem);
  kfence_free(mem);
  return false;
}

static bool test_heap_illegal_memcpy(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *src;
  FAR uint8_t *dst;

  size = size / 2;
  src = kfence_alloc(size);
  size = kfence_alloc_size(src);
  dst = kfence_alloc(size);

  memcpy(dst, src, size);
  memcpy(dst, src, size + 4); // 越界访问
  return false;
}
static bool test_heap_illegal_memset(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  size = kfence_alloc_size(mem);

  memset(mem, 0x11, size + 1); // 越界访问
  return false;
}

static bool test_heap_illegal_strcpy(size_t size)
{
  FAR char *dst = kfence_alloc(16);
  FAR char *src;
  int i;
  size = kfence_alloc_size(dst);
  src = kfence_alloc(size + 16);

  for (i = 0; i < size + 16; i++)
    {
      src[i] = 'A'; // 填充源缓冲区
    }
  
  strcpy(dst, src); // 越界访问
  return false;
}

static bool test_heap_legal_memcpy(size_t size)
{
  FAR uint8_t *des = kfence_alloc(size / 2);
  FAR uint8_t *src = kfence_alloc(size / 2);
  size_t des_size = kfence_get_size(des);
  size_t src_size = kfence_alloc_size(src);

  return memcpy(des, src, des_size > src_size ? src_size : des_size) != NULL;
}

static bool test_heap_legal_memset(size_t size)
{
  FAR uint8_t *des = kfence_alloc(size / 2);
  size = kfence_alloc_size(des);

  return memset(des, 0xef, size) != NULL;
}

static bool test_heap_legal_strcpy(size_t size)
{
  FAR char *mem = kfence_alloc(size);
  FAR char *str = "hello world";

  size = kfence_alloc_size(mem);
  return strcpy(mem, str) != NULL;
}

static bool test_invalid_free_address(size_t size)
{
  FAR uint8_t *mem = (FAR uint8_t *)kfence_alloc(size);
  if (!mem)
    {
      printf("test_invalid_free_address: alloc failed\n");
      return false;
    }

  // 非法释放（不是起始地址）
  FAR uint8_t *invalid_ptr = mem + 1;

  printf("Running test_invalid_free_address: free(%p), expected base=%p\n",
         invalid_ptr, mem);

  kfence_free(invalid_ptr); 

  return false; 
}

static bool test_invalid_realloc_non_kfence(size_t size)
{
  uint8_t dummy; // 栈上的对象，非分配得到
  FAR uint8_t *invalid_ptr = &dummy;

  printf("Running test_invalid_realloc_non_kfence: realloc(%p, %zu)\n",
         invalid_ptr, size);

  void *new_ptr = kfence_realloc(invalid_ptr, size); 

  // 如果 realloc 未报错，说明 kfence 没有正确检测
  return (new_ptr == NULL);
}

static bool test_invalid_realloc_to_invalid_address(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  FAR uint8_t *invalid_ptr = (FAR uint8_t *)0x12345678;

  printf("Running test_invalid_realloc_to_invalid_address: realloc(%p, %zu)\n",
         invalid_ptr, size);

  void *new_ptr = kfence_realloc(mem, size);

  // 如果 realloc 未报错，说明 kfence 没有正确检测
  return (new_ptr == NULL);
}

static bool test_invalid_realloc_to_freed_address(size_t size)
{
  FAR uint8_t *mem = kfence_alloc(size);
  kfence_free(mem);
  FAR uint8_t *invalid_ptr = mem;

  printf("Running test_invalid_realloc_to_freed_address: realloc(%p, %zu)\n",
         invalid_ptr, size);

  void *new_ptr = kfence_realloc(invalid_ptr, size);

  // 如果 realloc 未报错，说明 kfence 没有正确检测
  return (new_ptr == NULL);
}


/****************************************************************************
 * CWE416 测试适配函数
 ****************************************************************************/

static bool test_cwe416_use_after_free(size_t size)
{
  for (int i = 0; i < nitems(g_cwe416_tests); i++)
    {
      g_cwe416_tests[i]();
    }
  return true; 
}

/****************************************************************************
 * CWE415 测试适配函数
 ****************************************************************************/

static bool test_cwe415_double_free(size_t size)
{
  /* 定义并调用所有 CWE415 测试 */
  int (*tests[])(void) = {
    test_CWE415_Double_Free__malloc_free_int_01,
    test_CWE415_Double_Free__malloc_free_int_02,
    test_CWE415_Double_Free__malloc_free_int_03,
    test_CWE415_Double_Free__malloc_free_int_04,
    test_CWE415_Double_Free__malloc_free_int_05,
    test_CWE415_Double_Free__malloc_free_int_06,
    test_CWE415_Double_Free__malloc_free_int_07,
    test_CWE415_Double_Free__malloc_free_int_08,
    test_CWE415_Double_Free__malloc_free_int_09,
    test_CWE415_Double_Free__malloc_free_int_10,
    test_CWE415_Double_Free__malloc_free_int_11,
    test_CWE415_Double_Free__malloc_free_int_12,
    test_CWE415_Double_Free__malloc_free_int_13,
    test_CWE415_Double_Free__malloc_free_int_14,
    test_CWE415_Double_Free__malloc_free_int_15,
    test_CWE415_Double_Free__malloc_free_int_16,
    test_CWE415_Double_Free__malloc_free_int_17,
    test_CWE415_Double_Free__malloc_free_int_18,
    test_CWE415_Double_Free__malloc_free_int64_t_01,
    test_CWE415_Double_Free__malloc_free_long_01,
    test_CWE415_Double_Free__malloc_free_struct_01,
    test_CWE415_Double_Free__malloc_free_wchar_t_01
  };

  for (int i = 0; i < nitems(tests); i++)
    {
      tests[i]();
    }
  return true;
}

/****************************************************************************
 * CWE122 测试适配函数
 ****************************************************************************/

static bool test_cwe122_heap_overflow(size_t size)
{
  /* First, run the multi-file tests */
  printf("--- Running CWE122 multi-file tests ---\\n");
  test_CWE122_snprintf_66();
  test_CWE122_memcpy_66();
  test_CWE122_loop_66();
  printf("--- Multi-file tests complete ---\\n\\n");

  /* Now, run all single-file tests via function pointer array */
  printf("--- Running CWE122 single-file tests ---\\n");

  static int (* const cwe122_single_file_tests[])(void) =
  {
#include "cwe122/cwe122_test_cases.h"
  };

  for (int i = 0; i < sizeof(cwe122_single_file_tests) / sizeof(cwe122_single_file_tests[0]); i++)
    {
      cwe122_single_file_tests[i]();
    }

  printf("--- All %d CWE122 single-file tests complete ---\\n", i);
  return true;
}

/****************************************************************************
 * 测试运行函数
 ****************************************************************************/
static int run_test(FAR const testcase_t *test)
{
  size_t heap_size = sizeof(g_kfence_heap) - sizeof(run_t);
  FAR char *argv[3];
  FAR run_t *run;
  int status;
  pid_t pid;

  /* There is a memory leak here because we cannot guarantee that
   * it can be released correctly.
   */

  run = (run_t *)g_kfence_heap;
  if (!run)
    {
      return ERROR;
    }

  snprintf(run->argv, sizeof(run->argv), "%p", run);
  run->testcase = test;
  run->size = rand() % (heap_size / 2) + 1;
  run->heap = mm_initialize("kfence", (struct mm_heap_s *)&run[1], heap_size);
  if (!run->heap)
    {
      free(run);
      return ERROR;
    }

  argv[0] = "kfencetest";
  argv[1] = run->argv;
  argv[2] = NULL;

  posix_spawn(&pid, "kfencetest", NULL, NULL, argv, NULL);
  waitpid(pid, &status, 0);
  mm_uninitialize(run->heap);
  return status;
}

static int run_testcase(int argc, FAR char *argv[])
{
  uintptr_t index = strtoul(argv[1], NULL, 0);
  struct timespec result;
  struct timespec start;
  struct timespec end;
  FAR run_t *run;
  int ret;

  /* Pass in the number to run the specified case,
   * and the string of the number will not be very long
   */

  if (strlen(argv[1]) <= 3)
    {
      if (memcmp(argv[1], "-h", 2) == 0
          || index <= 0 || index > nitems(g_kfence_test))
        {
          error_handler();
        }
      else
        {
          run_test(&g_kfence_test[index - 1]);
        }

      return EXIT_SUCCESS;
    }

  run = (FAR run_t *)(uintptr_t)strtoul(argv[1], NULL, 16);
  clock_gettime(CLOCK_MONOTONIC, &start);
  ret = run->testcase->func(run->heap, run->size);
  clock_gettime(CLOCK_MONOTONIC, &end);

  timespec_sub(&result, &end, &start);
  printf("%s spending %ld.%lds\n", run->testcase->name,
                                   result.tv_sec,
                                   result.tv_nsec);

  return ret;
}
/****************************************************************************
 * main函数
 ****************************************************************************/
int main(int argc, FAR char *argv[])
{
  int status[nitems(g_kfence_test)];
  size_t i;

  if (argc < 2)
    {
      for (i = 0; i < nitems(g_kfence_test); i++)
        {
          if (g_kfence_test[i].is_auto)
            {
              printf("Kfence test: %s\n", g_kfence_test[i].name);
              status[i] = run_test(&g_kfence_test[i]);
            }
        }

      for (i = 0; i < nitems(status); i++)
        {
          if (g_kfence_test[i].is_auto)
            {
              printf("Kfence Test: %s -> %s\n",
                      g_kfence_test[i].name,
                      status[i]? "\033[32mPASS\033[0m" :
                                 "\033[31mFAIL\033[0m");
            }
        }
    }
  else
    {
      return run_testcase(argc, argv);
    }

  return EXIT_SUCCESS;
}