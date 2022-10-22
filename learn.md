### 字节序
1. 大端（存储）模式，是指数据的低位保存在内存的高地址中，而数据的高位保存在内存的低地址中;
2. 小端（存储）模式，是指数据的低位保存在内存的低地址中，而数据的高位保存在内存的高地址中;
    - 例如：Intel x86系列的CPU.

### GDB
1. 查看当前汇编: `disas`
2. 当前汇编下前进几行: `ni + 行数`
3. 查看寄存器：`info r`
4. 查看内存: https://blog.csdn.net/xiaozhiwise/article/details/122732433?