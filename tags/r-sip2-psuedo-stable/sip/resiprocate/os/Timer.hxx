#if !defined(TIMER_HXX)
#define TIMER_HXX
     
#include <util/Data.hxx>
#include <iostream>

#if defined( WIN32 )
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif


namespace Vocal2
{

// Note: Timers are not thread safe; if the application needs to create timers
// do something else
class Timer
{
   public:
      typedef unsigned long Id;
      typedef enum 
      {
         TimerA, // doubling
         TimerB,
         TimerC,
         TimerD,
         TimerE1,// doubling
         TimerE2,// doubling
         TimerF,
         TimerG, // doubling
         TimerH,
         TimerI,
         TimerJ,
         TimerK,
         TimerTrying,
         TimerStale
      } Type;
      
      Timer(unsigned long ms, Type type, const Data& transactionId);
      Timer(unsigned long ms); // for TimerQueue only - don't use
      Timer(const Timer& t);
      Timer& operator=(const Timer& t);
      
      // returns the unique identifier
      Id getId() const { return mId; }
      Type getType() const { return mType; }

      static void setupTimeOffsets(); // initialize
      static UInt64 getTimeMicroSec(); // get a 64 bit time
      static UInt64 getTimeMs(); // in ms

      static const unsigned long T1;
      static const unsigned long T2;
      static const unsigned long T4;
      static const unsigned long T100;
      static const unsigned long TC;
      static const unsigned long TD;
      static const unsigned long TS;
      
   private:
      static UInt64 getSystemTime();
      static UInt64 getSystemTicks();

      UInt64 mWhen;
      Id mId;
      Type mType;
      Data mTransactionId;
      unsigned long mDuration;

      static unsigned long mCpuSpeedMHz;
      static UInt64 mBootTime;
      static unsigned long mTimerCount;

      friend bool operator<(const Timer& t1, const Timer& t2);
      friend std::ostream& operator<<(std::ostream&, const Timer&);
      friend class TimerQueue;
};
 
std::ostream& operator<<(std::ostream&, const Timer&);

}

#endif
/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */