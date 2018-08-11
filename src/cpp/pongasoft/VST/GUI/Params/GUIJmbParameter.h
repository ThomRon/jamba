/*
 * Copyright (c) 2018 pongasoft
 *
 * Licensed under the General Public License (GPL) Version 3; you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * https://www.gnu.org/licenses/gpl-3.0.html
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

#include <base/source/fobject.h>
#include <pongasoft/VST/ParamConverters.h>
#include <pongasoft/VST/ParamDef.h>
#include <pongasoft/VST/Parameters.h>
#include <pongasoft/VST/Messaging.h>
#include <pongasoft/VST/MessageHandler.h>
#include "GUIParamCx.h"

namespace pongasoft {
namespace VST {
namespace GUI {
namespace Params {

/**
 * Base class for a Jamba (Jmb) GUI parameter. This type of parameter is used when it cannot be mapped to a
 * Vst parameter whose internal representation must be a value in the range [0.0, 1.0]. For example a string (like
 * a user input label to name a component) does not fit in the Vst parameter category. By implementating
 * the serializable api (readFromStream/writeToStream), any type can be part of the state.
 */
class IGUIJmbParameter : public IMessageHandler
{
public:
  // Constructor
  explicit IGUIJmbParameter(std::shared_ptr<IJmbParamDef> iParamDef) : fParamDef{std::move(iParamDef)} {}

  // getParamDef
  std::shared_ptr<IJmbParamDef> const &getParamDef() const { return fParamDef; }

  // getParamID
  ParamID getParamID() const { return fParamDef->fParamID; }

  // readFromStream
  virtual tresult readFromStream(IBStreamer &iStreamer) = 0;

  // writeToStream
  virtual tresult writeToStream(IBStreamer &oStreamer) const = 0;

  // readFromMessage
  virtual tresult readFromMessage(Message const &iMessage) = 0;

  // writeToMessage
  virtual tresult writeToMessage(Message &oMessage) const = 0;

  // handleMessage
  tresult handleMessage(Message const &iMessage) override { return readFromMessage(iMessage); }

protected:
  std::shared_ptr<IJmbParamDef> fParamDef;
};

/**
 * This is the templated version providing serializer methods, very similar to the GUIVstParameter concept.
 *
 * @tparam T the underlying type of the param */
template<typename T>
class GUIJmbParameter : public IGUIJmbParameter, public FObject
{
public:
  using ParamType = T;

  // Constructor
  explicit GUIJmbParameter(std::shared_ptr<JmbParamDef<T>> iParamDef) :
    IGUIJmbParameter(iParamDef),
    FObject(),
    fJmbParamDef{std::move(iParamDef)},
    fValue{fJmbParamDef->fDefaultValue}
  {}

  // getJmbParamDef (correct type for this subclass)
  inline std::shared_ptr<JmbParamDef<T>> const &getJmbParamDef() const
  {
    return fJmbParamDef;
  }

  /**
   * Update the parameter with a value.
   *
   * @return true if the value was actually updated, false if it is the same
   */
  bool update(ParamType const &iValue)
  {
    if(fValue != iValue)
    {
      fValue = iValue;
      changed();
      return true;
    }
    return false;
  }

  /**
   * Sets the value. The difference with update is that it does not check for equality (case when ParamType is
   * not comparable)
   */
  void setValue(ParamType const &iValue)
  {
    fValue = iValue;
    changed();
  }

  // readFromStream
  tresult readFromStream(IBStreamer &iStreamer) override
  {
    tresult res = fJmbParamDef->readFromStream(iStreamer, fValue);
    if(res == kResultOk)
      changed();
    return res;
  }

  // writeToStream
  tresult writeToStream(IBStreamer &oStreamer) const override
  {
    return fJmbParamDef->writeToStream(fValue, oStreamer);
  }

  // readFromMessage
  tresult readFromMessage(Message const &iMessage) override
  {
    tresult res = fJmbParamDef->readFromMessage(iMessage, fValue);
    if(res == kResultOk)
      changed();
    return res;
  }

  // writeToMessage
  tresult writeToMessage(Message &oMessage) const override
  {
    return fJmbParamDef->writeToMessage(fValue, oMessage);
  }

  // getValue
  inline ParamType const &getValue() { return fValue; }

  /**
   * @return a connection that will listen to parameter changes (see GUIParamCx)
   */
  std::unique_ptr<GUIParamCx> connect(Parameters::IChangeListener *iChangeListener)
  {
    return std::make_unique<GUIParamCx>(getParamID(), this, iChangeListener);
  }

protected:
  std::shared_ptr<JmbParamDef<T>> fJmbParamDef;
  ParamType fValue;
};


//------------------------------------------------------------------------
// GUIJmbParam - wrapper to make writing the code much simpler and natural
//------------------------------------------------------------------------
/**
 * This is the main class that the plugin should use as it exposes only the necessary methods of the param
 * as well as redefine a couple of operators which helps in writing simpler and natural code (the param
 * behaves like T in many ways).
 *
 * @tparam T the underlying type of the param */
template<typename T>
class GUIJmbParam
{
public:
  GUIJmbParam(GUIJmbParameter<T> *iPtr = nullptr) : // NOLINT (not marked explicit on purpose)
    fPtr{iPtr}
  {}

  // exists
  inline bool exists() const { return fPtr != nullptr; }

  // getParamID
  inline ParamID getParamID() const { return fPtr->getParamID(); }

  /**
   * This method is typically called by a view to change the value of the parameter. Listeners will be notified
   * of the changes.
   */
  inline bool update(T const &iNewValue) { return fPtr->update(iNewValue); }

  /**
   * The difference with update is that it does not check for equality (case when SerParamType is not comparable)
   */
  inline void setValue(T const &iNewValue) { fPtr->setValue(iNewValue); }

  // getValue
  inline T const &getValue() const { return fPtr->getValue(); }

  // allow to use the param as the underlying ParamType (ex: "if(param)" in the case ParamType is bool))
  inline operator T const &() const { return fPtr->getValue(); } // NOLINT

  // allow writing param->xxx to access the underlying type directly (if not a primitive)
  inline T const *operator->() const { return &fPtr->getValue(); }

  // connect
  inline std::unique_ptr<GUIParamCx> connect(Parameters::IChangeListener *iChangeListener) { return fPtr->connect(iChangeListener); }

private:
  GUIJmbParameter<T> *fPtr;
};

}
}
}
}