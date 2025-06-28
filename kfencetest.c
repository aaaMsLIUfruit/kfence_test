#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>

#include <nuttx/fs/procfs.h>
#include <nuttx/mm/mm.h>
#include <nuttx/mm/kfence.h>

#ifdef CONFIG_TESTING_KFENCE_JULIET_CWE
#include "cwe416/cwe416_tests.h"
#include "cwe415/cwe415_tests.h"
#include "cwe122/cwe122_tests.h"
#endif

/****************************************************************************
 * 结构体声明
 ****************************************************************************/
typedef struct testcase_s
{
  bool (*func)(FAR struct mm_heap_s *heap, size_t size);
  bool is_auto;
  FAR const char *name;
} testcase_t;

typedef struct run_s
{
  char argv[32];                      // 参数字符串
  FAR const testcase_t *testcase;     // 测试用例
  FAR struct mm_heap_s *heap;         // 堆指针
  size_t size;                        // 分配大小
} run_t;

#define RETRY_COUNT 100    //最大尝试次数
/****************************************************************************
 * 测试用例声明
 ****************************************************************************/
//左越界
static bool test_heap_underflow(FAR struct mm_heap_s *heap, size_t size);
//右越界
static bool test_heap_overflow(FAR struct mm_heap_s *heap, size_t size);
//释放后使用
static bool test_heap_use_after_free(FAR struct mm_heap_s *heap, size_t size);
//双重释放
static bool test_heap_double_free(FAR struct mm_heap_s *heap, size_t size);
//memcpy目标地址非法或越界
static bool test_heap_illegal_memcpy(FAR struct mm_heap_s *heap, size_t size);
//memset目标地址非法或越界
static bool test_heap_illegal_memset(FAR struct mm_heap_s *heap, size_t size);
//strcpy目的缓冲区溢出
static bool test_heap_illegal_strcpy(FAR struct mm_heap_s *heap, size_t size);
//释放无效地址
static bool test_invalid_free_address(FAR struct mm_heap_s *heap, size_t size);
//realloc到已释放地址
static bool test_invalid_realloc_to_freed_address(FAR struct mm_heap_s *heap, size_t size);

//对照组——验证不报错
static bool test_heap_legal_memcpy(FAR struct mm_heap_s *heap, size_t size);
static bool test_heap_legal_memset(FAR struct mm_heap_s *heap, size_t size);
static bool test_heap_legal_strcpy(FAR struct mm_heap_s *heap, size_t size);

/****************************************************************************
 * 朱丽叶测试用例
 ****************************************************************************/
#ifdef CONFIG_TESTING_KFENCE_JULIET_CWE
// CWE416 测试适配函数
static bool test_cwe416_use_after_free(FAR struct mm_heap_s *heap, size_t size);

// CWE415 测试适配函数
static bool test_cwe415_double_free(FAR struct mm_heap_s *heap, size_t size);

// CWE122 测试适配函数
static bool test_cwe122_heap_overflow(FAR struct mm_heap_s *heap, size_t size);
#endif

/****************************************************************************
 * 测试用例数组
 ****************************************************************************/
const static testcase_t g_kfence_test[] =
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
  {test_invalid_realloc_to_freed_address, true, "invalid realloc to freed address"},
#ifdef CONFIG_TESTING_KFENCE_JULIET_CWE
  {test_cwe416_use_after_free, true, "CWE416 use after free"},
  {test_cwe415_double_free, true, "CWE415 double free"},
  {test_cwe122_heap_overflow, true, "CWE122 heap overflow"}
#endif
};

//测试专用内存池
static char g_kfence_heap[512] aligned_data(8);
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
static bool test_heap_underflow(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap underflow test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
    /* 如果重试后仍未能分配内存 */
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  *(mem - 1) = 0x12;
  return false;
}

static bool test_heap_overflow(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap overflow test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  size = kfence_get_size(mem);

  mem[size + 1] = 0x11;
  return false;
}

static bool test_heap_use_after_free(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap use after free test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  kfence_free(mem);
  mem[0] = 0x10;
  return false;
}


static bool test_heap_double_free(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap double free test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  kfence_free(mem);
  kfence_free(mem);
  return false;
}

static bool test_heap_illegal_memcpy(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *src = NULL;
  FAR uint8_t *dst = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap illegal memcpy test ---\n");
  while (retry_count-- > 0){
    src = kfence_alloc(heap,size);
    if(src != NULL){
      break;
    }
    usleep(50000);
  }

  size = size / 2;
  size = kfence_get_size(src);
  while (retry_count-- > 0){
    dst = kfence_alloc(heap,size);
    if(dst != NULL){
      break;
    }
  }
  if (dst == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  memcpy(dst, src, size);
  memcpy(dst, src, size + 4); // 越界访问
  return false;
}
static bool test_heap_illegal_memset(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap illegal memset test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  size = kfence_get_size(mem);
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  memset(mem, 0x11, size + 1); // 越界访问
  return false;
}

static bool test_heap_illegal_strcpy(FAR struct mm_heap_s *heap, size_t size)
{
  FAR char *dst = NULL;
  FAR char *src = NULL;
  int i;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap illegal strcpy test ---\n");
  while (retry_count-- > 0){
    dst = kfence_alloc(heap,16);
    if(dst != NULL){
      break;
    }
    usleep(50000);
  }
  if (dst == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false;
  }
  size = kfence_get_size(dst);
  retry_count = RETRY_COUNT; // 重新赋值
  while (retry_count-- > 0){
    src = kfence_alloc(heap,size + 16);
    if(src != NULL){
      break;
    }
    usleep(50000);
  }
  if (src == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false;
  }
  for (i = 0; i < size + 16; i++) {
    src[i] = 'A';
  }
  strcpy(dst, src); // 越界
  return true; // 检测到越界应返回true
}

static bool test_heap_legal_memcpy(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *des = NULL;
  FAR uint8_t *src = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap legal memcpy test ---\n");
  while (retry_count-- > 0){
    des = kfence_alloc(heap,size / 2);
    if(des != NULL){
      break;
    }
    usleep(50000);
  }
  if (des == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  while (retry_count-- > 0){
    src = kfence_alloc(heap,size / 2);
    if(src != NULL){
      break;
    }
    usleep(50000);
  }
  if (src == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  size_t des_size = kfence_get_size(des);
  size_t src_size = kfence_get_size(src);

  return memcpy(des, src, des_size > src_size ? src_size : des_size) != NULL;
}

static bool test_heap_legal_memset(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *des = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running heap legal memset test ---\n");
  while (retry_count-- > 0){
    des = kfence_alloc(heap,size / 2);
    if(des != NULL){
      break;
    }
    usleep(50000);
  }
  if (des == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  return memset(des, 0xef, size) != NULL;
}

static bool test_heap_legal_strcpy(FAR struct mm_heap_s *heap, size_t size)
{
  FAR char *mem = NULL;
  FAR char *str = "hello world";
  int retry_count = RETRY_COUNT;
  printf("--- Running heap legal strcpy test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  size = kfence_get_size(mem);
  return strcpy(mem, str) != NULL;
}

static bool test_invalid_free_address(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running invalid free address test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  // 非法释放（不是起始地址）
  FAR uint8_t *invalid_ptr = mem + 1;
  kfence_free(invalid_ptr); 
  return false; 
}


static bool test_invalid_realloc_to_freed_address(FAR struct mm_heap_s *heap, size_t size)
{
  FAR uint8_t *mem = NULL;
  FAR uint8_t *invalid_ptr = NULL;
  int retry_count = RETRY_COUNT;
  printf("--- Running invalid realloc to freed address test ---\n");
  while (retry_count-- > 0){
    mem = kfence_alloc(heap,size);
    if(mem != NULL){
      break;
    }
    usleep(50000);
  }
  if (mem == NULL) {
    printf("ERROR: Failed to allocate memory after 1000 attempts. Test aborted.\n");
    return false; // 无法执行测试
  }
  kfence_free(mem);
  invalid_ptr = mem;
  void *new_ptr = kfence_realloc(heap, invalid_ptr, size);

  // 如果 realloc 未报错，说明 kfence 没有正确检测
  return (new_ptr == NULL);
}


/****************************************************************************
 * CWE416 测试适配函数
 ****************************************************************************/
#ifdef CONFIG_TESTING_KFENCE_JULIET_CWE

#include "cwe416/cwe416_declarations.h"

typedef void (*cwe416_test_func)(void);
static cwe416_test_func g_cwe416_tests[] = {
#include "cwe416/cwe416_test_cases.h"
};

static bool test_cwe416_use_after_free(FAR struct mm_heap_s *heap, size_t size)
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

#include "cwe415/cwe415_declarations.h"

typedef void (*cwe415_test_func)(void);
static cwe415_test_func g_cwe415_tests[] = {
#include "cwe415/cwe415_test_cases.h"
};

static bool test_cwe415_double_free(FAR struct mm_heap_s *heap, size_t size)
{
  for (int i = 0; i < nitems(g_cwe415_tests); i++)
    {
      g_cwe415_tests[i]();
    }
  return true;
}

/****************************************************************************
 * CWE122 测试适配函数
 ****************************************************************************/

typedef void (*cwe122_test_func)(void);
typedef int (*cwe122_multi_test_func)(void);
typedef int (*cwe122_single_test_func)(void);
static cwe122_multi_test_func g_cwe122_multi_file_tests[] = {
    test_CWE122_snprintf_66,
    test_CWE122_memcpy_66,
    test_CWE122_loop_66
};
static cwe122_single_test_func g_cwe122_single_file_tests[] = {
#include "cwe122/cwe122_test_cases.h"
};
static bool test_cwe122_heap_overflow(FAR struct mm_heap_s *heap, size_t size)
{
  printf("--- Running CWE122 multi-file tests ---\n");
  for (int i = 0; i < nitems(g_cwe122_multi_file_tests); i++)
    g_cwe122_multi_file_tests[i]();
  printf("--- Multi-file tests complete ---\n\n");

  printf("--- Running CWE122 single-file tests ---\n");
  for (int i = 0; i < nitems(g_cwe122_single_file_tests); i++)
    g_cwe122_single_file_tests[i]();
  printf("--- All %zu CWE122 single-file tests complete ---\n", nitems(g_cwe122_single_file_tests));
  return true;
}

#endif /* CONFIG_TESTING_KFENCE_JULIET_CWE */

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
  printf("=== kfencetest starting ===\n");
  printf("argc = %d\n", argc);
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