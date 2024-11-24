## 静态分配和动态分配

### 宏定义

`configSUPPORT_STATIC_ALLOCATION`：

① config表示这是在 `FreeRTOSConfig.h` 文件中定义，控制 FreeRTOS 的功能启用与否

② SUPPORT表明该配置宏是用来**决定是否支持某种功能**，而不是直接实现功能

③ STATIC_ALLOCATION强调此选项是控制是否支持静态内存分配的开关

**静态分配方式**：用户需要自己提供任务堆栈和任务控制块（TCB）等所需的内存块，FreeRTOS 不从堆中分配内存

宏定义`configSUPPORT_STATIC_ALLOCATION`，在 FreeRTOS 中，`configSUPPORT_STATIC_ALLOCATION` 是一个配置选项，用于启用或禁用静态任务和队列的创建方式。静态分配是 FreeRTOS 提供的内存分配方式之一，与动态分配方式（依赖堆分配）相对。

**取值：0 或 1**
0：禁用静态分配，所有任务、队列等对象必须通过动态分配方式创建。
1：启用静态分配，允许使用静态分配方式创建任务、队列等对象。

与 `configSUPPORT_DYNAMIC_ALLOCATION` 的关系：

`configSUPPORT_STATIC_ALLOCATION` 和 `configSUPPORT_DYNAMIC_ALLOCATION` 可以同时设置为 `1`，以支持静态和动态分配。

如果两者均为 `0`，任务或对象无法创建，系统将无法运行。

**静态分配方式**

```c
#include "FreeRTOS.h"
#include "task.h"

// 静态分配的任务堆栈和任务控制块
static StackType_t xTaskStack[configMINIMAL_STACK_SIZE];//#define configMINIMAL_STACK_SIZE  ((uint16_t)128)
static StaticTask_t xTaskBuffer;

void vTaskFunction(void *pvParameters)
{
    for (;;)
    {
        // 任务逻辑
    }
}

void main(void)
{
    // 创建任务（静态分配）
    TaskHandle_t xTaskHandle = xTaskCreateStatic(
        vTaskFunction,             // 任务函数
        "TaskName",                // 任务名称
        configMINIMAL_STACK_SIZE,  // 堆栈大小
        NULL,                      // 任务参数
        tskIDLE_PRIORITY,          // 优先级
        xTaskStack,                // 用户提供的堆栈
        &xTaskBuffer               // 用户提供的控制块
    );

    // 启动调度器
    vTaskStartScheduler();

}
```

**动态分配方式**

内存由 FreeRTOS 管理：
使用 FreeRTOS 的堆内存分配器（通常是 pvPortMalloc）。
TCB 和任务堆栈都从堆中动态分配。

用户无需手动管理内存分配，调用 xTaskCreate() 时 FreeRTOS 自动分配和释放内存

存在风险：
如果堆内存不足，任务创建会失败。
长时间运行可能导致内存碎片问题，影响系统可靠性。

```c
#include "FreeRTOS.h"
#include "task.h"

void vTaskFunction(void *pvParameters)
{
    for (;;)
    {
        // 任务逻辑
    }
}

void main(void)
{
    // 创建任务（动态分配）
    TaskHandle_t xTaskHandle = NULL;
    xTaskCreate(
        vTaskFunction,             // 任务函数
        "TaskName",                // 任务名称
        configMINIMAL_STACK_SIZE,  // 堆栈大小
        NULL,                      // 任务参数
        tskIDLE_PRIORITY,          // 优先级
        &xTaskHandle               // 任务句柄
    );

    // 启动调度器
    vTaskStartScheduler();

}
```

