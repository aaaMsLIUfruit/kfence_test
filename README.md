# kfence_test
## 测试内容
- juliet-testing-suite-c 朱丽叶测试套件
  - cwe122 Heap Based Buffer Overflow
    **检测问题：**
    1. 越界访问：`Out-of-bounds access at %p, size %zu (object at %p)`  
    2. 警戒区被污染：`Red zone byte at %p is corrupted: expected 0x%02x, got 0x%02x`
    3. invalid access: `Access to non-allocated KFENCE object at %p`  
  - cwe415 Double Free
    **检测问题：**
    1. double free：`Double free detected: object already freed at %p`
  - cwe416 Use After Free
    **检测问题：**
    1. Use after free：`Use after free detected at %p`
    2. invalid access: `Access to non-allocated KFENCE object at %p`
- 自建测试
  完善Invalid free addr (p+1)、Invalid free: 非 heap 区、Invalid realloc测试
    
    
