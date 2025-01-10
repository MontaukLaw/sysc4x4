
#include "app_flash.h"

static void APP_FlashErase(void);
static void APP_FlashProgram(void);
static void APP_FlashBlank(void);
static void APP_FlashVerify(void);

// void APP_ErrorHandler(void);

float f_flash_buffer[FF_FLASH_BUF_LEN];
// 页大小是128字节. 6个float数据, 24字节
float init_coe_buffer[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

// uint32_t DATA[64] = {

// };
#if 1
void test_flash(void)
{

    /* Unlock Flash */
    HAL_FLASH_Unlock();

    /* Erase Flash */
    APP_FlashErase();

    /* Check Flash */
    APP_FlashBlank();

    /* Program Flash */
    APP_FlashProgram();

    /* Lock Flash */
    HAL_FLASH_Lock();

    /* Verify Flash */
    // APP_FlashVerify();
}
#endif

void app_read_data(void)
{
    Flash_Read(FLASH_USER_START_ADDR, sizeof(uint32_t) * 6, (uint32_t *)f_flash_buffer);
}

/**
 * @brief  Flash Erase
 * @param  None
 * @retval None
 */
static void APP_FlashErase(void)
{
    uint32_t PAGEError = 0;
    FLASH_EraseInitTypeDef EraseInitStruct;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGEERASE;         /* Page Erase */
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;           /* Erase Start Address */
    EraseInitStruct.NbPages = 1;                                   /* Erase Page Number */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) /* Page Erase */
    {
        APP_ErrorHandler();
    }
}

/**
 * @brief  Flash Program
 * @param  None
 * @retval None
 */
static void APP_FlashProgram(void)
{
    uint32_t flash_program_start = FLASH_USER_START_ADDR;                           /* flash program start address */
    uint32_t flash_program_end = (FLASH_USER_START_ADDR + sizeof(init_coe_buffer)); /* flash program end address */
    uint32_t *src = (uint32_t *)init_coe_buffer;                                    /* Program data */

    while (flash_program_start < flash_program_end)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_PAGE, flash_program_start, src) == HAL_OK) /* Program */
        {
            flash_program_start += FLASH_PAGE_SIZE; /* flash Start point first page */
            src += FLASH_PAGE_SIZE / 4;             /* Update data point */
        }
    }
}

/**
 * @brief  Flash Check
 * @param  None
 * @retval None
 */
static void APP_FlashBlank(void)
{
    uint32_t addr = 0;

    while (addr < sizeof(init_coe_buffer))
    {
        if (0xFFFFFFFF != HW32_REG(FLASH_USER_START_ADDR + addr))
        {
            APP_ErrorHandler();
        }
        addr += 4;
    }
}

/**
 * @brief  Flash Verify
 * @param  None
 * @retval None
 */
static void APP_FlashVerify(void)
{
    uint32_t addr = 0;

    while (addr < sizeof(init_coe_buffer))
    {
        if (init_coe_buffer[addr / 4] != HW32_REG(FLASH_USER_START_ADDR + addr))
        {
            APP_ErrorHandler();
        }
        addr += 4;
    }
}
