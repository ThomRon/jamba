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
#include "GUIParamCxMgr.h"

namespace pongasoft::VST::GUI::Params {

//------------------------------------------------------------------------
// GUIParamCxMgr::invokeAll
//------------------------------------------------------------------------
void GUIParamCxMgr::invokeAll()
{
  for(auto &it: fParamCxs)
  {
    it.second->onTargetChange();
  }
}

//------------------------------------------------------------------------
// GUIParamCxMgr::unregisterParam
//------------------------------------------------------------------------
bool GUIParamCxMgr::unregisterParam(TagID iParamID)
{
  if(iParamID >= 0)
    return fParamCxs.erase(static_cast<ParamID>(iParamID)) == 1;
  return false;
}

//------------------------------------------------------------------------
// GUIParamCxMgr::unregisterAll
//------------------------------------------------------------------------
void GUIParamCxMgr::unregisterAll()
{
  fParamCxs.clear();
}

//------------------------------------------------------------------------
// GUIParamCxMgr::registerOptionalDiscreteParam
//------------------------------------------------------------------------
bool GUIParamCxMgr::registerOptionalDiscreteParam(TagID iParamID,
                                                  GUIOptionalParam<int32> &oParam,
                                                  int32 iStepCount,
                                                  Parameters::IChangeListener *iChangeListener)
{
  auto previousTagID = oParam.getTagID();

  if(previousTagID != iParamID)
    unregisterParam(previousTagID);

  bool paramChanged = false;

  auto param = fGUIState->findParam(iParamID);

  if(param)
  {
    auto discreteParam = param->asDiscreteParameter(iStepCount);

    if(discreteParam)
    {
      oParam.assign(std::move(discreteParam));
      paramChanged = true;
    }
    else
    {
      DLOG_F(WARNING, "param [%d] is not of the requested type", iParamID);
    }
  }

  // no vst or jmb parameter match => using default
  if(!paramChanged)
  {
    oParam.clearAssignment(iParamID);

    if(iParamID == UNDEFINED_PARAM_ID)
      paramChanged = true;

#ifndef NDEBUG
    if(iParamID != UNDEFINED_PARAM_ID)
      DLOG_F(WARNING, "could not find any parameter (vst or jmb) with id [%d]... reverting to default", iParamID);
#endif
  }

  if(iChangeListener)
  {
    fParamCxs[iParamID] = oParam.connect(iChangeListener);
  }
  else
  {
    unregisterParam(iParamID);
  }

  return paramChanged;
}



}