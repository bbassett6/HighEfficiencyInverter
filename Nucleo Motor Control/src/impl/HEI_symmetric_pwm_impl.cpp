#include "interface/symmetric_pwm_interface.hpp"

#if PLATFORM_HEI

HRTIM_HandleTypeDef hhrtim1;

namespace SymmetricPWM
{
    unsigned int _masterTimerPeriodCycles = cpuMHz * 2 * switchingPeriodNanos / 1000;   // master timer is derived from cpu clock PLL'd 2x
    Vec3<float> _phaseDuties = {.a = {0.5, 0.5, 0.5}};

    HRTIM_TimeBaseCfgTypeDef _commonTimeBaseCfg = {0};
    HRTIM_TimerCfgTypeDef _masterTimerCfg = {0};
    HRTIM_TimerCfgTypeDef _subTimerCfg = {0};
    HRTIM_CompareCfgTypeDef _compareUnitCfg = {0};

    bool init()
    {
        HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg = {0};
        HRTIM_OutputCfgTypeDef pOutputCfg = {0};

        // Time base config
        _commonTimeBaseCfg.Period               = _masterTimerPeriodCycles;     // <-- period
        _commonTimeBaseCfg.RepetitionCounter    = 0x00;
        _commonTimeBaseCfg.PrescalerRatio       = HRTIM_PRESCALERRATIO_DIV1;    // <-- prescaler
        _commonTimeBaseCfg.Mode                 = HRTIM_MODE_CONTINUOUS;

        // Init master timer
        hhrtim1.Instance = HRTIM1;
        hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
        hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
        __HAL_RCC_HRTIM1_CONFIG(RCC_HRTIM1CLK_PLLCLK);                              // Use the PLLx2 for 128Mhz resolution
        __HRTIM1_CLK_ENABLE();
        HRTIM1->sCommonRegs.DLLCR = HRTIM_CALIBRATIONRATE_14| HRTIM_DLLCR_CALEN;    // DLL calibration: periodic calibration enabled, period set to 14μs
        while(HRTIM1->sCommonRegs.ISR & HRTIM_IT_DLLRDY == RESET);                  // Wait for DLL calibration to finish
        if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK)
        {
            return false;
        }

        // Config time bases
        if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER, &_commonTimeBaseCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &_commonTimeBaseCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, &_commonTimeBaseCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &_commonTimeBaseCfg) != HAL_OK)
        {
            return false;
        }

        // Config master timer
        _masterTimerCfg.InterruptRequests   = HRTIM_MASTER_IT_NONE;
        _masterTimerCfg.DMARequests         = HRTIM_MASTER_DMA_NONE;
        _masterTimerCfg.DMASrcAddress       = 0x0000;
        _masterTimerCfg.DMADstAddress       = 0x0000;
        _masterTimerCfg.DMASize             = 0x1;
        _masterTimerCfg.HalfModeEnable      = HRTIM_HALFMODE_DISABLED;
        _masterTimerCfg.StartOnSync         = HRTIM_SYNCSTART_DISABLED;
        _masterTimerCfg.ResetOnSync         = HRTIM_SYNCRESET_DISABLED;
        _masterTimerCfg.DACSynchro          = HRTIM_DACSYNC_NONE;
        _masterTimerCfg.PreloadEnable       = HRTIM_PRELOAD_DISABLED;
        _masterTimerCfg.UpdateGating        = HRTIM_UPDATEGATING_INDEPENDENT;
        _masterTimerCfg.BurstMode           = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
        _masterTimerCfg.RepetitionUpdate    = HRTIM_UPDATEONREPETITION_DISABLED;
        if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER, &_masterTimerCfg) != HAL_OK)
        {
            return false;
        }
        
        // Config sub-timers
        _subTimerCfg = _masterTimerCfg;
        _subTimerCfg.InterruptRequests      = HRTIM_TIM_IT_NONE;
        _subTimerCfg.DMARequests            = HRTIM_TIM_DMA_NONE;
        _subTimerCfg.PushPull               = HRTIM_TIMPUSHPULLMODE_DISABLED;
        _subTimerCfg.FaultEnable            = HRTIM_TIMFAULTENABLE_NONE;
        _subTimerCfg.FaultLock              = HRTIM_TIMFAULTLOCK_READWRITE;
        _subTimerCfg.DeadTimeInsertion      = HRTIM_TIMDEADTIMEINSERTION_ENABLED;               // <-- Enable deadtime insertion
        _subTimerCfg.DelayedProtectionMode  = HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
        _subTimerCfg.DelayedProtectionMode  = HRTIM_TIMER_D_E_DELAYEDPROTECTION_DISABLED;       // NOTE: if delayed protection mode is used, Timers A-C and D-E need separate config structs
        _subTimerCfg.UpdateTrigger          = HRTIM_TIMUPDATETRIGGER_NONE;
        _subTimerCfg.ResetTrigger           = HRTIM_TIMRESETTRIGGER_MASTER_PER;                 // <-- Reset sub-timers when master timer rolls over
        _subTimerCfg.ResetUpdate            = HRTIM_TIMUPDATEONRESET_DISABLED;
        if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &_subTimerCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, &_subTimerCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &_subTimerCfg) != HAL_OK)
        {
            return false;
        }

        // Config timer compare units
        _compareUnitCfg.AutoDelayedMode     = HRTIM_AUTODELAYEDMODE_REGULAR;
        _compareUnitCfg.AutoDelayedTimeout  = 0x0000;
        setPhaseDuties(_phaseDuties);

        // Config deadtime
        pDeadTimeCfg.Prescaler          = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;    // <-- Enables deadtime resolution 8x the cpu clock
        pDeadTimeCfg.RisingValue        = 6;                                        // <-- Corresponds to ~10ns deadtime
        pDeadTimeCfg.RisingSign         = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
        pDeadTimeCfg.RisingLock         = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
        pDeadTimeCfg.RisingSignLock     = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
        pDeadTimeCfg.FallingValue       = 6;
        pDeadTimeCfg.FallingSign        = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
        pDeadTimeCfg.FallingLock        = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
        pDeadTimeCfg.FallingSignLock    = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;

        if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &pDeadTimeCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, &pDeadTimeCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pDeadTimeCfg) != HAL_OK)
        {
            return false;
        }

        // Config timer outputs
        pOutputCfg.Polarity                 = HRTIM_OUTPUTPOLARITY_HIGH;
        pOutputCfg.SetSource                = HRTIM_OUTPUTSET_TIMCMP1;                                  // <-- Set the output when compare unit 1 fires
        pOutputCfg.ResetSource              = HRTIM_OUTPUTRESET_TIMCMP2|HRTIM_OUTPUTRESET_MASTERPER;    // <-- Reset the output low when compare unit 2 fires or the master timer rolls over
        pOutputCfg.IdleMode                 = HRTIM_OUTPUTIDLEMODE_NONE;
        pOutputCfg.IdleLevel                = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
        pOutputCfg.FaultLevel               = HRTIM_OUTPUTFAULTLEVEL_NONE;
        pOutputCfg.ChopperModeEnable        = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
        pOutputCfg.BurstModeEntryDelayed    = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &pOutputCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &pOutputCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &pOutputCfg) != HAL_OK)
        {
            return false;
        }
        pOutputCfg.SetSource    = HRTIM_OUTPUTSET_NONE;     // <-- Deadtime insertion is enabled so no need to set output sources for Tx2 pins
        pOutputCfg.ResetSource  = HRTIM_OUTPUTRESET_NONE;
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &pOutputCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &pOutputCfg) != HAL_OK)
        {
            return false;
        }
        if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2, &pOutputCfg) != HAL_OK)
        {
            return false;
        }

        // Post init routine so timers can drive their GPIO pins
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin         = GPIO_PIN_14|GPIO_PIN_15;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate   = GPIO_AF13_HRTIM1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin         = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

        // Enable timer outputs
        HAL_HRTIM_WaveformOutputStart(&hhrtim1, 
              HRTIM_OUTPUT_TA1
            | HRTIM_OUTPUT_TA2
            | HRTIM_OUTPUT_TB1
            | HRTIM_OUTPUT_TB2
            | HRTIM_OUTPUT_TD1
            | HRTIM_OUTPUT_TD2
        );

        // Enable timer counters
        HAL_HRTIM_WaveformCounterStart(&hhrtim1,
              HRTIM_TIMERID_TIMER_A
            | HRTIM_TIMERID_TIMER_B
            | HRTIM_TIMERID_TIMER_D
        );

        return true;
    }

    void setPeriodNanos(unsigned int periodNanos)
    {
        _masterTimerPeriodCycles = cpuMHz * 2 * periodNanos / 1000;
        _commonTimeBaseCfg.Period = _masterTimerPeriodCycles;

        HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER, &_commonTimeBaseCfg);
        HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, &_commonTimeBaseCfg);
        HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, &_commonTimeBaseCfg);
        HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &_commonTimeBaseCfg);

        setPhaseDuties(_phaseDuties);
    }

    void setPhaseDuties(Vec3<float> phaseDuties)
    {
        _phaseDuties = phaseDuties;

        Vec3<unsigned int> riseTimes =
        {.u = {
            .A = (_masterTimerPeriodCycles * (1 - phaseDuties.u.A)) / 2,
            .B = (_masterTimerPeriodCycles * (1 - phaseDuties.u.B)) / 2,
            .C = (_masterTimerPeriodCycles * (1 - phaseDuties.u.C)) / 2
        }};

        Vec3<unsigned int> fallTimes =
        {.u = {
            .A = _masterTimerPeriodCycles - riseTimes.u.A,
            .B = _masterTimerPeriodCycles - riseTimes.u.B,
            .C = _masterTimerPeriodCycles - riseTimes.u.C
        }};

        // Set compare units
        _compareUnitCfg.CompareValue = riseTimes.u.A;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &_compareUnitCfg);
        _compareUnitCfg.CompareValue = riseTimes.u.B;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, &_compareUnitCfg);
        _compareUnitCfg.CompareValue = riseTimes.u.C;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_1, &_compareUnitCfg);
        _compareUnitCfg.CompareValue = fallTimes.u.A;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, &_compareUnitCfg);
        _compareUnitCfg.CompareValue = fallTimes.u.B;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, &_compareUnitCfg);
        _compareUnitCfg.CompareValue = fallTimes.u.C;
        HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_COMPAREUNIT_2, &_compareUnitCfg);
    }
}

void HRTIM1_Master_IRQHandler(void)
{
    HAL_HRTIM_IRQHandler(&hhrtim1,HRTIM_TIMERINDEX_MASTER);
}

#endif