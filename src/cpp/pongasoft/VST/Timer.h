/*
 * Copyright (c) 2018-2019 pongasoft
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * @author Yan Pujante
 */

#pragma once

#include <base/source/timer.h>
#include <memory>

namespace pongasoft::VST {

/**
 * Simple wrapper class around Steinberg::Timer which will automatically release the timer on delete
 */
class AutoReleaseTimer
{
public:
  // Constructor
  explicit AutoReleaseTimer(Steinberg::Timer *iTimer) : fTimer{iTimer} {}

  // Destructor => auto release
  ~AutoReleaseTimer()
  {
    stop();

    if(fTimer)
      fTimer->release();
  }

  // stops the timer
  inline void stop()
  {
    if(fTimer)
      fTimer->stop();
  }

  /**
   * Creates and return an auto release timer
   */
  static std::unique_ptr<AutoReleaseTimer> create(Steinberg::ITimerCallback* iCallback,
                                                  Steinberg::uint32 iIntervalMilliseconds)
  {
    return std::make_unique<AutoReleaseTimer>(Steinberg::Timer::create(iCallback, iIntervalMilliseconds));
  }

  // disabling copy
  AutoReleaseTimer(AutoReleaseTimer const &) = delete;
  AutoReleaseTimer& operator=(AutoReleaseTimer const &) = delete;

private:
  Steinberg::Timer *fTimer;
};

}