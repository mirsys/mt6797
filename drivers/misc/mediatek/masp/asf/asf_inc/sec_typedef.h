/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef SEC_PAL_H
#define SEC_PAL_H

#include "sec_osal_light.h"
#include "sec_osal.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/******************************************************************************
 *  DEBUG
 ******************************************************************************/
/* Debug message event */
#define DBG_EVT_NONE        (0)	/* No event */
#define DBG_EVT_CMD         (1 << 0)	/* SEC CMD related event */
#define DBG_EVT_FUNC        (1 << 1)	/* SEC function event */
#define DBG_EVT_INFO        (1 << 2)	/* SEC information event */
#define DBG_EVT_WRN         (1 << 30)	/* Warning event */
#define DBG_EVT_ERR         (1 << 31)	/* Error event */
#define DBG_EVT_ALL         (0xffffffff)

#define DBG_EVT_MASK        (DBG_EVT_ALL)

#ifdef SEC_DEBUG
#define MSG(evt, fmt, args...) do { \
if ((DBG_EVT_##evt) & DBG_EVT_MASK)
pr_err(fmt, ##args);
} while (0)
#else
#define MSG(evt, fmt, args...)  do {} while (0)
#endif

#define MSG_FUNC(mod) MSG(FUNC, "[%s] %s\n", mod, __func__)

/******************************************************************************
 *  EXPORT FUNCTION
 ******************************************************************************/
extern void *mcpy(void *dest, const void *src, int count);
extern int mcmp(const void *cs, const void *ct, int count);
extern void dump_buf(unsigned char *buf, unsigned int len);

#endif				/* end of SEC_LIB_H */
