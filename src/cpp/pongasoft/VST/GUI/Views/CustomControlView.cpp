/*
 * Copyright (c) 2019 pongasoft
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

#include "CustomControlView.h"

namespace pongasoft::VST::GUI::Views {

//------------------------------------------------------------------------
// CustomDiscreteControlView::setControlTag
//------------------------------------------------------------------------
void CustomDiscreteControlView::registerParameters()
{
  registerOptionalDiscreteParam(getControlTag(), fControlParameter, getStepCount());
}

//------------------------------------------------------------------------
// CustomDiscreteControlView::computeStepCount
//------------------------------------------------------------------------
int32 CustomDiscreteControlView::computeStepCount() const
{
  // TODO: function not needed
  if(fControlParameter.getStepCount() > 0)
    return fControlParameter.getStepCount();

  return std::max(0, getStepCount());
}

}