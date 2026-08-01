# keil_proj_demo

这是一个给机器人队新人准备的 STM32 入门示例工程，使用 Keil MDK 打开。

工程会循环演示下面这件事：

1. LED1 ~ LED4 依次闪烁 3 次；
2. 蜂鸣器响一声；
3. LED 亮灭速度逐渐变快；
4. 到达较快速度后恢复初始速度，然后重新开始。

工程里刻意安排了一些可以改的小地方，用来练习 Git 的分支、提交、回退等操作。

## 硬件资源

| 外设 | 引脚 | 说明 |
| --- | --- | --- |
| LED1 | PA4 | 高电平点亮 |
| LED2 | PA5 | 高电平点亮 |
| LED3 | PA6 | 高电平点亮 |
| LED4 | PA7 | 高电平点亮 |
| 蜂鸣器 | PA8 | 高电平响 |

引脚初始化代码在 `Core/Src/gpio.c` 的 `MX_GPIO_Init()` 中，由 STM32CubeMX 生成。

## 工程目录结构

```text
keil_proj_demo/
├── Core/
│   ├── Inc/                 # 主程序、GPIO、中断等头文件
│   └── Src/                 # main.c、gpio.c、中断服务程序等源文件
├── Drivers/
│   ├── CMSIS/               # ARM 内核相关文件
│   └── STM32F4xx_HAL_Driver/ # STM32 HAL 库
├── hardware/
│   ├── inc/                 # 自己写的 LED、蜂鸣器头文件
│   └── src/                 # 自己写的 LED、蜂鸣器实现文件
├── MDK-ARM/                 # Keil 工程文件和编译输出
│   └── keil_proj_demo.uvprojx
├── keil_proj_demo.ioc       # CubeMX 工程配置，改引脚时用 CubeMX 打开
├── .gitignore               # Git 忽略编译输出等文件
└── README.md
```

```text
Core/Src/main.c      程序入口，主循环在这里
Core/Src/gpio.c      GPIO 初始化，配置 PA4~PA8 为输出
hardware/inc/led.h   LED 函数声明和引脚宏
hardware/src/led.c   LED 点亮/熄灭实现，里面有 switch 例子
hardware/inc/buzzer.h 蜂鸣器函数声明和引脚宏
hardware/src/buzzer.c 蜂鸣器打开/关闭实现
MDK-ARM/keil_proj_demo.uvprojx  Keil 工程文件，双击或在 Keil 中打开
```

## 打开、编译和下载

1. 安装 Keil MDK 5，并安装 STM32F4 器件支持包；
2. 双击打开 `MDK-ARM/keil_proj_demo.uvprojx`；
3. 按 `F7` 编译工程；
4. 连接 ST-Link 或调试器，按 `F8` 下载；
5. 复位开发板，观察 LED 和蜂鸣器。

## 编译的原理

### 先形象化地理解

可以把 C 源码想象成给芯片写的“菜谱”或“施工图”。芯片本身不认 C 代码，只认识由 0 和 1 组成的机器指令。编译器就像翻译官：把 `main.c`、`led.c` 里人能读懂的代码翻译成芯片能执行的机器码；链接器再把分散翻译好的零件拼成一个完整程序；最后通过调试器把程序“装进”芯片的 Flash。芯片复位后，CPU 从 Flash 里读出机器码并逐条执行。

Keil 里按 `F7` 后出现的 `.o`、`.axf`、`.hex` 等文件，就是这个翻译过程的中间产物和最终产物。

### 专业一点的描述

Keil MDK 的编译下载流程通常分这几步：

1. 预处理：处理 `#include`、`#define` 宏替换、条件编译；
2. 编译：对 C 源码进行词法、语法、语义分析，生成 ARM 汇编或目标文件 `.o`；
3. 汇编：启动文件 `startup_stm32f405xx.s` 等汇编代码也被汇编成目标文件；
4. 链接：把多个 `.o` 文件和库按地址分配链接在一起，生成可执行映像 `.axf` 和烧录文件 `.hex`；
5. 下载：通过 ST-Link 等调试器把 `.hex` 写入芯片 Flash。

所以一次完整的“编译”不只是按 F7，而是“预处理 -> 编译 -> 汇编 -> 链接 -> 下载 -> 复位运行”这条工具链在协同工作。

### 为什么改完 C 要重新编译下载

芯片真正运行的是 Flash 里的机器码，不是 `.c` 源码。`main.c` 里改了一个数字，源码变了，但 Flash 里还是旧的机器码，MCU 不会自动知道。

- 重新编译：把改动后的源码重新翻译成新的机器码，并重新链接出新的 `.hex`；
- 重新下载：把新的 `.hex` 覆盖写入 Flash；
- 复位：CPU 重新从 Flash 第一条指令开始执行新程序。

如果只编译不下载，编译产物只是硬盘上新固件；如果只下载不编译，下载的仍是旧固件。新人练习时最容易漏掉的是：源码改完 -> 保存 -> 编译 -> 下载 -> 复位，这五步都要走完。

## 程序运行流程

程序上电后大致按以下顺序运行：

```text
复位
  -> startup_stm32f405xx.s（启动文件，初始化栈和中断向量表）
  -> SystemInit（配置时钟树基础部分）
  -> main
      -> HAL_Init（初始化 HAL 库和 SysTick）
      -> SystemClock_Config（配置系统时钟，本工程为 168 MHz）
      -> MX_GPIO_Init（配置 PA4~PA8 为推挽输出）
      -> while (1)（无限循环，执行 LED/蜂鸣器演示）
```

`main.c` 中有 `USER CODE BEGIN ... END` 注释。CubeMX 再次生成代码时只会保留这些区域里的用户代码，所以自己的代码应该写在用户区域内。

## C 文件与头文件的作用

`.h` 文件放声明，`.c` 文件放实现。

例如 `led.h` 只告诉别人“有这个函数”，不写函数体：

```c
#ifndef LED_H
#define LED_H

void led_on(uint8_t led_num);
void led_off(uint8_t led_num);

#endif
```

`led.c` 通过 `#include "led.h"` 引入声明，然后写出真正的函数体：

```c
#include "led.h"

void led_on(uint8_t led_num)
{
    /* 实现代码 */
}
```

这样写的好处：

- 谁要调用 `led_on`，只需要包含 `led.h`，不需要关心内部实现；
- `led.c` 内部怎么改，只要函数名和参数不变，调用方不用改；
- 头文件保护 `#ifndef / #define / #endif` 可以防止同一个头文件被重复包含。

## 本工程覆盖的 C 语法

### 变量类型

`main.c` 里使用了：

```c
uint8_t  current_led = 1U;   /* 8 位无符号整数 */
uint16_t blink_times = 3U;   /* 16 位无符号整数 */
uint32_t delay_ms = 250U;    /* 32 位无符号整数 */
const uint8_t led_count = 4U; /* const 表示值不能被修改 */
```

常用类型对照：

| 类型 | 说明 | 常见范围 |
| --- | --- | --- |
| `uint8_t` | 8 位无符号整数 | 0 ~ 255 |
| `uint16_t` | 16 位无符号整数 | 0 ~ 65535 |
| `uint32_t` | 32 位无符号整数 | 0 ~ 4294967295 |
| `int32_t` | 32 位有符号整数 | -2147483648 ~ 2147483647 |
| `float` | 单精度小数 | 约 6~7 位有效数字 |

数字后面的 `U` 表示无符号数，例如 `250U`。

### `#define` 宏定义

`main.c` 顶部用 `#define` 给常量起名字：

```c
#define DEMO_LED_COUNT   4U
#define DEMO_BLINK_TIMES 3U
#define DEMO_DELAY_MS    250U
```

宏在编译前会被替换成后面的内容。宏定义结尾不需要分号。

### `for` 循环

`demo_blink_led()` 中用 `for` 控制闪烁次数：

```c
for (i = 0U; i < times; i++)
{
    led_on(led_num);
    HAL_Delay(delay_ms);
    led_off(led_num);
    HAL_Delay(delay_ms);
}
```

执行顺序是：初始化 `i = 0U` -> 判断 `i < times` -> 执行循环体 -> 执行 `i++` -> 再次判断。

### `while` 循环

`main()` 中最外层是嵌入式常见的死循环：

```c
while (1)
{
    /* 程序一直在这里循环 */
}
```

`main()` 里还有一个带条件的 `while`，用来依次控制 4 颗 LED：

```c
while (current_led <= led_count)
{
    demo_blink_led(current_led, blink_times, delay_ms);
    current_led++;
}
```

### `if` 判断

`demo_blink_led()` 用 `if` 检查 LED 编号是否合法：

```c
if (led_num > DEMO_LED_COUNT)
{
    return;
}
```

`main()` 里还用 `if / else` 控制延时变化。

### `switch` 语句

`hardware/src/led.c` 的 `led_on()` 和 `led_off()` 用 `switch` 根据 LED 编号选择引脚：

```c
switch (led_num)
{
    case 1:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
        break;
    case 2:
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
        break;
    default:
        break;
}
```

`case` 匹配成功后，如果没有 `break`，会继续执行下一个 `case` 的代码。`default` 处理没有匹配到的情况。

### 函数声明与定义

`main.c` 顶部先写函数声明，也就是函数原型：

```c
void demo_blink_led(uint8_t led_num, uint16_t times, uint32_t delay_ms);
```

文件后面的 `USER CODE 4` 区域写函数定义：

```c
void demo_blink_led(uint8_t led_num, uint16_t times, uint32_t delay_ms)
{
    /* 函数体 */
}
```

函数声明告诉编译器“这个函数存在”，函数定义告诉编译器“这个函数具体做什么”。

## Git 练习点

本目录默认还没有 Git 仓库，可以按下面的步骤练习。

### 第一次提交

```bash
git init
git add .
git commit -m "first commit"
```

`git add .` 会把当前目录加入暂存区。`.gitignore` 已经帮我们排除了 Keil 编译输出和用户界面文件。

### 练习 1：改闪烁次数

打开 `Core/Src/main.c`，找到：

```c
#define DEMO_BLINK_TIMES 3U
```

把 `3U` 改成 `5U`，保存后重新编译下载，然后提交：

```bash
git add Core/Src/main.c
git commit -m "change blink times to 5"
```

### 练习 2：改亮灭速度

把 `DEMO_DELAY_MS` 从 `250U` 改成 `500U`，观察现象并提交。

### 练习 3：创建分支

创建一个新分支，在分支里修改演示效果：

```bash
git switch -c feature-blink
```

修改 `demo_blink_led()` 的循环次数或延时，然后提交。切回主分支：

```bash
git checkout main
```

查看历史：

```bash
git log --oneline
git diff main feature-blink
```

确认没问题后可以把分支合并回来：
<small>**注意：** 如果在主分支上也改了同一段代码，合并时可能会有冲突，需要手动解决。</small>
<small>如果不想保留分支，可以在合并后删除它：`git branch -d feature-blink`。</small>
<small>**对象关系：** 先 `git checkout main` 再 `git merge feature-blink`，表示把 `feature-blink` 合并到当前分支 `main`；当前分支是接收方，命令行里的参数分支是来源。</small>

```bash
git checkout main
git merge feature-blink
```

### 练习 4：体验回退

先提交一个改动，再回退到上一个版本：

```bash
git log --oneline
git reset --hard HEAD~1
```

回退会丢掉最后一次提交，建议先在练习分支上操作。

## 暂时不展开的内容

这个入门版本先不展开 `struct`、`enum`、指针、数组、宏参数等进阶内容，后续可以单独开一个训练任务。

源码文件已保存为 UTF-8（带 BOM）编码，Keil 可以直接识别中文注释。如果仍然乱码，请用 VS Code 查看和编辑源码，用 Keil 负责编译下载，或者查阅资料调整keil的文件编码格式。
