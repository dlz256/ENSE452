#define mainBLINKY_TASK_PRIORITY (tskIDLE_PRIORITY +2)
static void vBlinkTask (void* parameters);

#define mainCLI_TASK_PRIORITY (tskIDLE_PRIORITY +1)
static void vCLITask (void* parameters);

