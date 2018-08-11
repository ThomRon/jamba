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
#ifndef __PONGASOFT_VST_GUI_PARAMETER_H__
#define __PONGASOFT_VST_GUI_PARAMETER_H__

#include "GUIRawVstParameter.h"
#include <pongasoft/VST/ParamConverters.h>
#include <pongasoft/VST/ParamDef.h>

namespace pongasoft {
namespace VST {
namespace GUI {
namespace Params {

/**
 * This class wraps a GUIRawVstParameter to deal with any type T
 */
template<typename T>
class GUIVstParameter
{
public:
  using ParamType = T;

public:
  /**
   * Wrapper to edit a single parameter. Usage:
   *
   * // from a CView::onMouseDown callback
   * fMyParamEditor = fParameter.edit(myParamID);
   * fParamEditor->setValue(myValue);
   *
   * // from a CView::onMouseMoved callback
   * fParamEditor->setValue(myValue);
   *
   * // from a CView::onMouseUp/onMouseCancelled callback
   * fMyParamEditor->commit();
   */
  class Editor
  {
  public:
    inline explicit Editor(std::unique_ptr<GUIRawVstParameter::Editor> iRawEditor,
                           std::shared_ptr<VstParamDef<T>> iVstParamDef) :
      fRawEditor{std::move(iRawEditor)},
      fVstParamDef{std::move(iVstParamDef)}
    {
    }

    // disabling copy
    Editor(Editor const &) = delete;
    Editor& operator=(Editor const &) = delete;

    /**
     * Change the value of the parameter. Note that nothing happens if you have called commit or rollback already
     */
    inline tresult setValue(ParamType const &iValue)
    {
      return fRawEditor->setValue(fVstParamDef->normalize(iValue));
    }

    /*
     * Call when you are done with the modifications.
     * This has no effect if rollback() has already been called
     */
    inline tresult commit()
    {
      return fRawEditor->commit();
    }

    /*
     * Shortcut to set the value prior to commit
     * Call when you are done with the modifications.
     * This has no effect if rollback() has already been called
     */
    inline tresult commit(ParamType const &iValue)
    {
      setValue(iValue);
      return commit();
    }

    /**
     * Call this if you want to revert to the original value of the parameter (when the editor is created).
     * This has no effect if commit() has already been called
     */
    inline tresult rollback()
    {
      return fRawEditor->rollback();
    }

  private:
    std::unique_ptr<GUIRawVstParameter::Editor> fRawEditor;
    std::shared_ptr<VstParamDef<T>> fVstParamDef;
  };

public:
  // Constructor
  GUIVstParameter(std::shared_ptr<GUIRawVstParameter> iRawParameter,
                  std::shared_ptr<VstParamDef<T>> iVstParamDef) :
    fRawParameter{std::move(iRawParameter)},
    fVstParamDef{std::move(iVstParamDef)}
  {
    DCHECK_NOTNULL_F(fRawParameter.get());
    // DLOG_F(INFO, "VSTParameter::VSTParameter(%d)", fRawParameter->getParamID());
  }

  // Destructor
  ~GUIVstParameter()
  {
    // DLOG_F(INFO, "VSTParameter::~VSTParameter(%d)", fRawParameter->getParamID());
  }

  // getParamID
  ParamID getParamID() const
  {
    return fRawParameter->getParamID();
  }

  /**
   * @return the current value of the parameter as a T (using the Denormalizer)
   */
  ParamType getValue() const
  {
    return fVstParamDef->denormalize(fRawParameter->getValue());
  }

  /**
   * @return the current value of the parameter as a normalized value
   */
  ParamValue getNormalizedValue() const
  {
    return fRawParameter->getValue();
  }

  /**
   * Sets the value of this parameter. Note that this is "transactional" and if you want to make
   * further changes that spans multiple calls (ex: onMouseDown / onMouseMoved / onMouseUp) you should use an editor
   */
  tresult setValue(ParamType const &iValue)
  {
    return fRawParameter->setValue(fVstParamDef->normalize(iValue));
  }

  /**
   * Populates the oString with a string representation of this parameter
   */
  void toString(String128 oString)
  {
    fRawParameter->toString(oString);
  }

  /**
   * Returns a string representation of this parameter
   */
  String toString()
  {
    return fRawParameter->toString();
  }

  /**
   * @return an editor to modify the parameter (see Editor)
   */
  std::unique_ptr<Editor> edit()
  {
    return std::make_unique<Editor>(fRawParameter->edit(), fVstParamDef);
  }

  /**
   * Shortcut to create an editor and set the value to it
   *
   * @return an editor to modify the parameter (see Editor)
   */
  std::unique_ptr<Editor> edit(ParamType iValue)
  {
    auto editor = edit();
    editor->setValue(iValue);
    return editor;
  }

private:
  std::shared_ptr<GUIRawVstParameter> fRawParameter;
  std::shared_ptr<VstParamDef<T>> fVstParamDef;
};

//------------------------------------------------------------------------
// GUIVstParam - wrapper to make writing the code much simpler and natural
//------------------------------------------------------------------------
/**
 * This is the main class that the plugin should use as it exposes only the necessary methods of the param
 * as well as redefine a couple of operators which helps in writing simpler and natural code (the param
 * behaves like T in many ways).
 *
 * @tparam T the underlying type of the param */
template<typename T>
class GUIVstParam
{
public:
  GUIVstParam() : fPtr{nullptr} {}

  // move constructor
  explicit GUIVstParam(std::unique_ptr<GUIVstParameter<T>> &&iPtr) : fPtr{std::move(iPtr)} {}

  // delete copy constructor
  GUIVstParam(GUIVstParam<T> &iPtr) = delete;

  // move copy constructor
  GUIVstParam(GUIVstParam<T> &&iPtr) noexcept : fPtr{std::move(iPtr.fPtr)} {}

  // move assignment constructor
  GUIVstParam<T> &operator=(GUIVstParam<T> &&iPtr) { fPtr = std::move(iPtr.fPtr); return *this; }

  // exists
  inline bool exists() const { return (bool) fPtr; }

  // getParamID
  inline ParamID getParamID() const { return fPtr->getParamID(); }

  /**
   * @return the current value of the parameter as a T (using the Denormalizer)
   */
  inline T getValue() const { return fPtr->getValue(); }

  /**
   * @return the current value of the parameter as a normalized value
   */
  inline ParamValue getNormalizedValue() const { return fPtr->getNormalizedValue(); }

  /**
   * Sets the value of this parameter. Note that this is "transactional" and if you want to make
   * further changes that spans multiple calls (ex: onMouseDown / onMouseMoved / onMouseUp) you should use an editor
   */
  tresult setValue(T const &iValue) { return fPtr->setValue(iValue); }

  /**
   * Populates the oString with a string representation of this parameter
   */
  void toString(String128 oString) { fPtr->toString(oString); }

  /**
   * Returns a string representation of this parameter
   */
  String toString() { return fPtr->toString(); }

  /**
   * @return an editor to modify the parameter (see Editor)
   */
  std::unique_ptr<typename GUIVstParameter<T>::Editor> edit() { return fPtr->edit(); }

  /**
   * Shortcut to create an editor and set the value to it
   *
   * @return an editor to modify the parameter (see Editor)
   */
  std::unique_ptr<typename GUIVstParameter<T>::Editor> edit(T const &iValue) { return fPtr->edit(iValue); }

  // allow to use the param as the underlying ParamType (ex: "if(param)" in the case ParamType is bool))
  inline operator T() const { return fPtr->getValue(); } // NOLINT

  // allow to write param = 3 instead of param.setValue(3)
  inline void operator=(T const &iValue) { fPtr->setValue(iValue); }

  // allow to write param1 == param2
  inline bool operator==(const GUIVstParam<T> &rhs) const { return fPtr->getNormalizedValue() == rhs.fPtr->getNormalizedValue(); }

  // allow to write param1 != param2
  inline bool operator!=(const GUIVstParam &rhs) const { return fPtr->getNormalizedValue() != rhs.fPtr->getNormalizedValue(); }

private:
  std::unique_ptr<GUIVstParameter<T>> fPtr;
};

//------------------------------------------------------------------------
// shortcut notations
//------------------------------------------------------------------------
template<typename T>
using GUIVstParamEditor = std::unique_ptr<typename GUIVstParameter<T>::Editor>;

using GUIVstBooleanParam = GUIVstParam<bool>;
using GUIVstPercentParam = GUIVstParam<Percent>;

}
}
}
}

#endif // __PONGASOFT_VST_GUI_PARAMETER_H__
