/*
 * Copyright (c) 2019-2020 pongasoft
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

#include <pongasoft/VST/GUI/GUIController.h>
#include "../Plugin.h"

namespace pongasoft::test::jamba::GUI {

using namespace pongasoft::VST::GUI;

//------------------------------------------------------------------------
// JambaTestPluginController - Main GUI Controller
//------------------------------------------------------------------------
class JambaTestPluginController : public GUIController
{
public:
  static inline ::Steinberg::FUID UUID() { return JambaTestPluginControllerUID; };

public:
  //------------------------------------------------------------------------
  // Factory method used in JambaTestPlugin_VST3.cpp to create the controller
  //------------------------------------------------------------------------
  static FUnknown *createInstance(void * /*iContext*/) { return (IEditController *) new JambaTestPluginController(); }

public:
  // Constructor
  explicit JambaTestPluginController();

  // Destructor -- overridden for debugging purposes only
  ~JambaTestPluginController() override;

  // getGUIState
  GUIState *getGUIState() override { return &fState; }

  // createCustomController
  IController *createCustomController(UTF8StringPtr iName, IUIDescription const *iDescription,
                                      IController *iBaseController) override;

protected:
  // initialize
  tresult PLUGIN_API initialize(FUnknown *context) override;

private:
  // The controller gets a copy of the parameters (defined in Plugin.h)
  JambaTestPluginParameters fParams;

  // The state accessible in the controller and views
  JambaTestPluginGUIState fState;
};

}

