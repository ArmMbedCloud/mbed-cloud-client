// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

#if defined(TARGET_LIKE_MBED)

#include "update-client-pal-flashiap/arm_uc_pal_flashiap_platform.h"
#include "mbed.h"

static FlashIAP flash;

int32_t arm_uc_flashiap_init()
{
    /* Workaround for https://github.com/ARMmbed/mbed-os/issues/4967
     * pal_init calls flash.init() before here. Second call to flash.init() will
     * return -1 error state. Hence we ignore the result of flash.init here.
     */
    flash.init();
    return 0;
}

int32_t arm_uc_flashiap_erase(uint32_t address, uint32_t size)
{
    return flash.erase(address, size);
}

int32_t arm_uc_flashiap_program(const uint8_t* buffer, uint32_t address, uint32_t size)
{
    return flash.program(buffer, address, size);
}

int32_t arm_uc_flashiap_read(uint8_t* buffer, uint32_t address, uint32_t size)
{
    return flash.read(buffer, address, size);
}

uint32_t arm_uc_flashiap_get_page_size(void)
{
    return flash.get_page_size();
}

uint32_t arm_uc_flashiap_get_sector_size(uint32_t address)
{
    return flash.get_sector_size(address);
}

#endif
