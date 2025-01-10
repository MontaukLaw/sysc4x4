#include "flash.h"

float cap_buffer[16] = {0};
#if 0
float coe_buffer[16] = {1.4, 1.0, 1.4, 1.0, 1.0, 0.7, 0.93, 1.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; //#14
#endif
// float coe_buffer[16] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // #9
// uint32_t flash_buffer[16] = {0};

uint8_t MCU_Status = 0x00;

// 寄生电容擦除Flash
void CAP_FlashErase(uint32_t flash_addr, uint32_t buffer_size)
{
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;              /* 页擦除 */
    EraseInitStruct.PageAddress = flash_addr;                             /* 擦除起始地址 */
    EraseInitStruct.NbPages  = (buffer_size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;      // 为了正确计算擦除页数，需要向上取整
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)        /* 页擦除 */
    {
       //  printf("\r\nFlash Erase Error\r\n");
        APP_ErrorHandler();
    }
}

// 检查Flash是否为空
void CAP_FlashBlank(uint32_t flash_addr, uint32_t buffer_size)
{
    uint32_t addr = 0;

    while (addr < sizeof(buffer_size))
    {
        if (0xFFFFFFFF != HW32_REG(flash_addr + addr))
        {
            // printf("\r\nFlash is not blank\r\n");
            APP_ErrorHandler();
        }
        addr += 4;
    }
}

// 将浮点数组存储到Flash
void Flash_Write(uint32_t flash_addr, uint32_t buffer_size, uint32_t* data_src) 
{
    // 临时禁用看门狗
    IWDG_HandleTypeDef *pIwdg = &IwdgHandle;
    uint32_t reload = pIwdg->Init.Reload;
    pIwdg->Init.Reload = 0xFFF;
    HAL_IWDG_Init(pIwdg);
    
    uint8_t i = 0;
    uint32_t offset = 0;

    if(flash_addr == FLASH_CAP_ADDR)
    {
        // 将寄生电容数组都存入 cap_buffer 中
        for (i = 0; i < sizeof(cxxx0) / sizeof(float); i++, offset++)
        {
            cap_buffer[offset] = cxxx0[i];
        }
        for (i = 0; i < sizeof(cxxx1) / sizeof(float); i++, offset++)
        {
            cap_buffer[offset] = cxxx1[i];
        }
        for (i = 0; i < sizeof(cxxx2) / sizeof(float); i++, offset++)
        {
            cap_buffer[offset] = cxxx2[i];
        }
        for (i = 0; i < sizeof(cxxx3) / sizeof(float); i++, offset++)
        {
            cap_buffer[offset] = cxxx3[i];
        }
    }
    // else if(flash_addr == FLASH_COE_ADDR)
    // {
        
    // }
    
    uint32_t flash_start_addr = flash_addr;
    uint32_t flash_end_addr = flash_addr + buffer_size;
    uint32_t *src = data_src;

    HAL_FLASH_Unlock(); // 解锁Flash
    CAP_FlashErase(flash_addr, buffer_size);   // 擦除Flash
    CAP_FlashBlank(flash_addr, buffer_size);   // 检查Flash是否为空
    // 写入flash
    while(flash_start_addr < flash_end_addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_start_addr, src) == HAL_OK)
        {
            flash_start_addr += FLASH_PAGE_SIZE;                                           
            src += FLASH_PAGE_SIZE / 4;                                                       
        }
    }
    HAL_FLASH_Lock(); // 上锁

    memset(data_src, 0, buffer_size);

    // 恢复看门狗设置
    pIwdg->Init.Reload = reload;
    HAL_IWDG_Init(pIwdg);
}


// 从Flash读取浮点数组
void Flash_Read(uint32_t flash_addr, uint32_t buffer_size, uint32_t* data_src)
{
    uint32_t addr = 0;
    uint32_t index = 0; // 数组索引计数器
    while(addr < buffer_size)
    {
        uint32_t value = HW32_REG(flash_addr + addr); // 从flash中读取4字节数据
        addr += 4;

        // 将 uint32_t 数据转换为 float，并存入数据缓冲区
        memcpy(&data_src[index], &value, sizeof(float));
        index++;
    }

    // if(flash_addr == FLASH_CAP_ADDR)
    // {
    //     printf("cap read:\r\n");
    //     printf("cxxx0:%f, %f, %f, %f\r\n", cap_buffer[0], cap_buffer[1], cap_buffer[2], cap_buffer[3]);
    //     printf("cxxx1:%f, %f, %f, %f\r\n", cap_buffer[4], cap_buffer[5], cap_buffer[6], cap_buffer[7]);
    //     printf("cxxx2:%f, %f, %f, %f\r\n", cap_buffer[8], cap_buffer[9], cap_buffer[10], cap_buffer[11]);
    //     printf("cxxx3:%f, %f, %f, %f\r\n", cap_buffer[12], cap_buffer[13], cap_buffer[14], cap_buffer[15]);
    // }
    // else if(flash_addr == FLASH_COE_ADDR)
    // {
    //     printf("coe read:\r\n");
    //     printf("coe0:%f, %f, %f, %f\r\n", coe_buffer[0], coe_buffer[1], coe_buffer[2], coe_buffer[3]);
    //     printf("coe1:%f, %f, %f, %f\r\n", coe_buffer[4], coe_buffer[5], coe_buffer[6], coe_buffer[7]);
    //     printf("coe2:%f, %f, %f, %f\r\n", coe_buffer[8], coe_buffer[9], coe_buffer[10], coe_buffer[11]);
    //     printf("coe3:%f, %f, %f, %f\r\n", coe_buffer[12], coe_buffer[13], coe_buffer[14], coe_buffer[15]);
    // }
}

// 写入单个状态值到Flash
void Flash_Write_Status(uint32_t flash_addr, uint8_t status)
{
    uint32_t status_buffer[16] = {0};  // 创建一个页大小的缓冲区
    status_buffer[0] = (uint32_t)status;  // 将状态值放在缓冲区第一个位置
    
    HAL_FLASH_Unlock(); // 解锁Flash
    
    // 擦除状态值所在的页
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError = 0;
    
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGEERASE;
    EraseInitStruct.PageAddress = flash_addr;
    EraseInitStruct.NbPages     = 1;  // 只需要一页
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        printf("\r\nStatus Flash Erase Error\r\n");
        APP_ErrorHandler();
    }
    
    // 写入状态值
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_addr, (uint32_t *)status_buffer) != HAL_OK)
    {
        printf("\r\nStatus Flash Write Error\r\n");
        APP_ErrorHandler();
    }
    
    HAL_FLASH_Lock(); // 上锁
}

// 从Flash读取状态值
uint8_t Flash_Read_Status(uint32_t flash_addr)
{
    uint32_t status_word = HW32_REG(flash_addr); // 读取32位数据
    return (uint8_t)status_word; // 转换回uint8_t
}


