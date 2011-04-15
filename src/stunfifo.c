/*
Copyright 2011 Cisco. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY CISCO ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of Cisco.
*/



#include "stunfifo.h"
#include <string.h>
#include <stdlib.h>

static StunPassFifo_T  StunPassFifo[STUN_FIFO_MAX_THREAD_CTX];


/* */
void StunFifo_Init(uint32_t threadCtx, uint32_t passLen)
{
    int i;
    StunPassFifo[threadCtx].passLen = passLen;
    for (i=0; i < STUN_FIFO_SIZE; i++)
    {
        StunPassFifo[threadCtx].elem[i].sockPairId = -1;
        StunPassFifo[threadCtx].elem[i].passwd = calloc(1, passLen); 
    }
    StunPassFifo[threadCtx].init = true;
}

/* insert last in ring buffer */
void StunFifo_InsertLast(uint32_t threadCtx, int32_t sockPairId, char  *passwd)
{
    /* check if exists */
    StunFifoElement *p =StunFifo_FindElemBySockPairId(threadCtx, sockPairId);
    if (!p)
    {
        /* not found, point to last in ring buffer */
        p = &StunPassFifo[threadCtx].elem[StunPassFifo[threadCtx].w];
        StunPassFifo[threadCtx].w = (StunPassFifo[threadCtx].w + 1) % STUN_FIFO_SIZE;
    }
    /* copy */
    p->sockPairId = sockPairId;
    strncpy(p->passwd, passwd, StunPassFifo[threadCtx].passLen);
}

/* search whole fifo backwards for SockId */
StunFifoElement *StunFifo_FindElemBySockPairId(uint32_t threadCtx, int32_t sockPairId)
{
    int startIx, i;
    startIx = (StunPassFifo[threadCtx].w == 0 ? STUN_FIFO_SIZE-1 : StunPassFifo[threadCtx].w-1);

    for (i = 0; i < STUN_FIFO_SIZE; i++)
    {
        if (StunPassFifo[threadCtx].elem[startIx].sockPairId ==  sockPairId)
            return &StunPassFifo[threadCtx].elem[startIx];
        startIx = (startIx == 0 ? STUN_FIFO_SIZE-1 : startIx-1);
    }
    return NULL;
}

char *StunFifo_FindPasswdBySockPairId(uint32_t threadCtx, int32_t sockPairId)
{
    StunFifoElement *p = StunFifo_FindElemBySockPairId(threadCtx, sockPairId);
    return p ? p->passwd : NULL;
}

