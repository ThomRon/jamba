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

#include <vstgui/lib/cview.h>

#include <pongasoft/VST/Parameters.h>

#include <utility>
#include "GUIRawVstParameter.h"
#include "GUIJmbParameter.h"
#include "GUIVstParameter.h"
#include "GUIOptionalParam.h"

namespace pongasoft::VST::GUI {

class GUIState;

namespace Params {

class GUIParamCxMgr;

/**
 * This class is inherited by classes who want to be aware of parameters and be notified when they change.
 *
 * The typical use case is to have a custom view (or a custom controller) inherits from this class and implements
 * the `#registerParameters()` method which in turn calls the various `registerXXXParam` flavors and
 * then implements `onParameterChange()` to handle the callback:
 *
 * ```
 * // Example
 * void MyView::registerParameters() {
 *   fMyParam = registerVstParam(myParamID);
 *   registerBaseCallback(getTitleTag(),
 *                        [this](IGUIParam &iParam) {
 *                          setTitle(iParam.toUTF8String(fPrecisionOverride));
 *                        }, true);
 * }
 *
 * void MyView::onParameterChange(ParamID iParamID) {
 *  // called when fMyParam changes
 * }
 * ```
 *
 * @note For convenience, the classes `Views::CustomView`, `Views::CustomViewAdapter` and `Views::CustomController`
 *       already inherit from this class.
 */
class ParamAware : public Parameters::IChangeListener
{
public:
  virtual ~ParamAware();

  //------------------------------------------------------------------------
  // Base Param
  //------------------------------------------------------------------------

  /**
   * Registers the "base" param for the most generic use case but as a result is fairly limited and mainly gives access
   * to the string representation of the param.
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return empty param if not found
   */
  IGUIParam registerBaseParam(ParamID iParamID, bool iSubscribeToChanges = true);

  /**
   * Registers a callback for the "base" param for the most generic use case. As a result it is fairly limited and
   * mainly gives access to the string representation of the param.
   *
   * @return empty param if not found
   */
  IGUIParam registerBaseCallback(ParamID iParamID,
                                 Parameters::ChangeCallback iChangeCallback,
                                 bool iInvokeCallback);

  /**
   * Registers a callback for the "base" param for the most generic use case. As a result it is fairly limited and
   * mainly gives access to the string representation of the param.
   *
   * @return empty param if not found
   */
  IGUIParam registerBaseCallback(ParamID iParamID,
                                 Parameters::ChangeCallback1<IGUIParam> iChangeCallback,
                                 bool iInvokeCallback);

  //------------------------------------------------------------------------
  // Optional Param
  //------------------------------------------------------------------------

  /**
   * Registers an optional parameter which handles Vst, Jmb or no parameter at all. This parameter is particularly
   * useful to write very generic views that can accept "any" kind of parameter (Vst or Jmb) while still being
   * usable if no parameter is assigned at all.
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return param (never empty!)
   */
  template<typename T>
  GUIOptionalParam<T> registerOptionalParam(ParamID iParamID,
                                            bool iSubscribeToChanges = true);

  /**
   * Registers an optional parameter which handles Vst, Jmb or no parameter at all. This parameter is particularly
   * useful to write very generic views that can accept "any" kind of parameter (Vst or Jmb) while still being
   * usable if no parameter is assigned at all.
   * The callback will be invoked when the parameter changes.
   *
   * @return param (never empty!)
   */
  template<typename T>
  GUIOptionalParam<T> registerOptionalCallback(ParamID iParamID,
                                               Parameters::ChangeCallback iChangeCallback,
                                               bool iInvokeCallback = false);

  /**
   * Registers an optional parameter which handles Vst, Jmb or no parameter at all. This parameter is particularly
   * useful to write very generic views that can accept "any" kind of parameter (Vst or Jmb) while still being
   * usable if no parameter is assigned at all.
   * The callback will be invoked when the parameter changes.
   *
   * @return param (never empty!)
   */
  template<typename T>
  GUIOptionalParam<T> registerOptionalCallback(ParamID iParamID,
                                               Parameters::ChangeCallback1<GUIOptionalParam<T>> iChangeCallback,
                                               bool iInvokeCallback = false);

  //------------------------------------------------------------------------
  // Optional Discrete Param
  //------------------------------------------------------------------------

  /**
   * Registers an optional discrete parameter which handles Vst, Jmb or no parameter at all. The underlying parameter
   * will be converted to a discrete parameter if possible.
   * This parameter is particularly useful to write very generic views that can accept "any" kind of parameter
   * (Vst or Jmb) while still being usable if no parameter is assigned at all.
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return param (never empty!)
   */
  GUIOptionalParam<int32> registerOptionalDiscreteParam(ParamID iParamID,
                                                        int32 iStepCount,
                                                        bool iSubscribeToChanges = true);

  /**
   * Registers an optional discrete parameter which handles Vst, Jmb or no parameter at all. The underlying parameter
   * will be converted to a discrete parameter if possible.
   * This parameter is particularly useful to write very generic views that can accept "any" kind of parameter
   * (Vst or Jmb) while still being usable if no parameter is assigned at all.
   * The callback will be invoked when the parameter changes.
   *
   * @return param (never empty!)
   */
  GUIOptionalParam<int32> registerOptionalDiscreteCallback(ParamID iParamID,
                                                           int32 iStepCount,
                                                           Parameters::ChangeCallback iChangeCallback,
                                                           bool iInvokeCallback = false);

  /**
   * Registers an optional discrete parameter which handles Vst, Jmb or no parameter at all. The underlying parameter
   * will be converted to a discrete parameter if possible.
   * This parameter is particularly useful to write very generic views that can accept "any" kind of parameter
   * (Vst or Jmb) while still being usable if no parameter is assigned at all.
   * The callback will be invoked when the parameter changes.
   *
   * @return param (never empty!)
   */
  GUIOptionalParam<int32> registerOptionalDiscreteCallback(ParamID iParamID,
                                                           int32 iStepCount,
                                                           Parameters::ChangeCallback1<GUIOptionalParam<int32>> iChangeCallback,
                                                           bool iInvokeCallback = false);

  //------------------------------------------------------------------------
  // Raw Vst Param / ParamID
  //------------------------------------------------------------------------

  /**
   * Registers a raw parameter (no conversion)
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return empty param if not found
   */
  GUIRawVstParam registerRawVstParam(ParamID iParamID, bool iSubscribeToChanges = true);

  /**
   * Registers a raw parameter (no conversion)
   * The callback will be invoked when the parameter changes.
   *
   * @return empty param if not found
   */
  GUIRawVstParam registerRawVstCallback(ParamID iParamID,
                                        Parameters::ChangeCallback iChangeCallback,
                                        bool iInvokeCallback = false);

  /**
   * Registers a raw parameter (no conversion)
   * The callback will be invoked when the parameter changes.
   *
   * @return empty param if not found
   */
  GUIRawVstParam registerRawVstCallback(ParamID iParamID,
                                        Parameters::ChangeCallback1<GUIRawVstParam> iChangeCallback,
                                        bool iInvokeCallback = false);


  //------------------------------------------------------------------------
  // Raw Vst Param / param definition (RawVstParam)
  //------------------------------------------------------------------------

  /**
   * Registers a raw parameter (no conversion). Convenient call using the param def
   * (accessible in state-aware views via `fParams->fMyParam`).
   */
  inline GUIRawVstParam registerParam(RawVstParam const &iParamDef, bool iSubscribeToChanges = true) {
    return registerRawVstParam(iParamDef->fParamID, iSubscribeToChanges);
  }

  /**
   * Registers a raw parameter (no conversion). Convenient call using the param def
   * (accessible in state-aware views via `fParams->fMyParam`).
   */
  inline GUIRawVstParam registerCallback(RawVstParam const &iParamDef,
                                         Parameters::ChangeCallback iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerRawVstCallback(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);
  }

  /**
   * Registers a raw parameter (no conversion). Convenient call using the param def
   * (accessible in state-aware views via `fParams->fMyParam`).
   */
  inline GUIRawVstParam registerCallback(RawVstParam const &iParamDef,
                                         Parameters::ChangeCallback1<GUIRawVstParam> iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerRawVstCallback(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);
  }

  //------------------------------------------------------------------------
  // Shortcut Vst Param bool/Percent
  //------------------------------------------------------------------------

  // shortcut for BooleanParameter (will be removed => not very useful)
  [[deprecated("Since 4.0.0 -  use registerVstParam<bool> directly")]]
  GUIVstParam<bool> registerVstBooleanParam(ParamID iParamID, bool iSubscribeToChanges = true);

  // shortcut for PercentParameter (will be removed => not very useful)
  [[deprecated("Since 4.0.0 -  use registerVstParam<Percent> directly")]]
  GUIVstParam<Percent> registerVstPercentParam(ParamID iParamID, bool iSubscribeToChanges = true);

  //------------------------------------------------------------------------
  // Vst Param (Typed) / ParamID
  //------------------------------------------------------------------------

  /**
   * Register a Vst parameter simply given its id
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  GUIVstParam<T> registerVstParam(ParamID iParamID, bool iSubscribeToChanges = true);

  /**
   * Register a callback for a Vst parameter simply given its id
   * The callback will be invoked when the parameter changes.
   *
   * @return empty param if not found or not proper type
   */
  template<typename T>
  GUIVstParam<T> registerVstCallback(ParamID iParamID,
                                     Parameters::ChangeCallback iChangeCallback,
                                     bool iInvokeCallback = false);

  /**
   * Register a callback for a Vst parameter simply given its id
   * The callback will be invoked when the parameter changes.
   *
   * @return empty param if not found or not proper type
   */
  template<typename T>
  GUIVstParam<T> registerVstCallback(ParamID iParamID,
                                     Parameters::ChangeCallback1<GUIVstParam<T>> iChangeCallback,
                                     bool iInvokeCallback = false);

  //------------------------------------------------------------------------
  // Vst Param (Typed) / param definition (VstParam<T>)
  //------------------------------------------------------------------------

  /**
   * Convenient call to register a Vst param simply by using its description (accessible in state-aware views
   * via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIVstParam<T> registerParam(VstParam<T> const &iParamDef, bool iSubscribeToChanges = true) {
    return registerVstParam<T>(iParamDef->fParamID, iSubscribeToChanges);
  }

  /**
   * Convenient call to register a callback for the Vst param simply by using its description (accessible in
   * state-aware views via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIVstParam<T> registerCallback(VstParam<T> const &iParamDef,
                                         Parameters::ChangeCallback iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerVstCallback<T>(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);
  }

  /**
   * Convenient call to register a callback for the Vst param simply by using its description (accessible in
   * state-aware views via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * Example:
   *
   * ```
   * registerCallback<int>(fParams->fMyParam,
   *                       [] (GUIVstParam<int> &iParam) { iParam.getValue()...; });
   * ```
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIVstParam<T> registerCallback(VstParam<T> const &iParamDef,
                                         Parameters::ChangeCallback1<GUIVstParam<T>> iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerVstCallback<T>(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);
  }

  //------------------------------------------------------------------------
  // Jmb Param (Typed) / ParamID
  //------------------------------------------------------------------------

  /**
   * Registers the Jmb param by its id and return the wrapper to the param.
   * `onParameterChange()` will be called on changes (if `iSubscribeToChanges` is set to `true`).
   *
   * @return the wrapper which may be empty if the param does not exists or is of wrong type (use GUIJmbParam::exists())
   */
  template<typename T>
  GUIJmbParam<T> registerJmbParam(ParamID iParamID, bool iSubscribeToChanges = true);

  /**
   * Registers a callback for the Jmb param by its id and return the wrapper to the param.
   * The callback will be invoked when the parameter changes.
   *
   * @return the wrapper which may be empty if the param does not exists or is of wrong type (use GUIJmbParam::exists())
   */
  template<typename T>
  GUIJmbParam<T> registerJmbCallback(ParamID iParamID,
                                     Parameters::ChangeCallback iChangeCallback,
                                     bool iInvokeCallback = false);

  /**
   * Registers a callback for the Jmb param by its id and return the wrapper to the param.
   * The callback will be invoked when the parameter changes.
   *
   * @return the wrapper which may be empty if the param does not exists or is of wrong type (use GUIJmbParam::exists())
   */
  template<typename T>
  GUIJmbParam<T> registerJmbCallback(ParamID iParamID,
                                     Parameters::ChangeCallback1<GUIJmbParam<T>> iChangeCallback,
                                     bool iInvokeCallback = false);

  //------------------------------------------------------------------------
  // Jmb Param (Typed) / Param definition (JmbParam<T>)
  //------------------------------------------------------------------------

  /**
   * Convenient call to register a Jmb param simply by using its description (accessible in state-aware views
   * via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerParam(JmbParam<T> const &iParamDef, bool iSubscribeToChanges = true) {
    return registerJmbParam<T>(iParamDef->fParamID, iSubscribeToChanges);
  }

  /**
   * Convenient call to register a callback for the Jmb param simply by using its description (accessible in
   * state-aware views via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerCallback(JmbParam<T> const &iParamDef,
                                         Parameters::ChangeCallback iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerJmbCallback<T>(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);

  }

  /**
   * Convenient call to register a callback for the Jmb param simply by using its description (accessible in
   * state-aware views via `fParams->fMyParam`). Takes care of the type due to method API.
   *
   * Example:
   *
   * ```
   * registerCallback<int>(fParams->fMyParam,
   *                       [] (GUIJmbParam<int> &iParam) { iParam.getValue()...; });
   * ```
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerCallback(JmbParam<T> const &iParamDef,
                                         Parameters::ChangeCallback1<GUIJmbParam<T>> iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerJmbCallback<T>(iParamDef->fParamID, std::move(iChangeCallback), iInvokeCallback);
  }

  //------------------------------------------------------------------------
  // Jmb Param (Typed) / Param (GUIJmbParam<T>)
  //------------------------------------------------------------------------

  /**
   * Convenient call to register a Jmb param simply by using the param from the state (accessible in state-aware views
   * via `fState->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerParam(GUIJmbParam<T> &iParam, bool iSubscribeToChanges = true) {
    return registerJmbParam<T>(iParam.getParamID(), iSubscribeToChanges);
  }

  /**
   * Convenient call to register a callback for the Jmb param simply by using the param from the state
   * (accessible in state-aware views via `fState->fMyParam`). Takes care of the type due to method API.
   *
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerCallback(GUIJmbParam<T> &iParam,
                                         Parameters::ChangeCallback iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerJmbCallback<T>(iParam.getParamID(), std::move(iChangeCallback), iInvokeCallback);
  }

  /**
   * Convenient call to register a callback for the Jmb param simply by using the param from the state
   * (accessible in state-aware views via `fState->fMyParam`). Takes care of the type due to method API.
   *
   * Example:
   *
   * ```
   * registerCallback<int>(fParams->fMyParam,
   *                       [] (GUIJmbParam<int> &iParam) { iParam.getValue()...; });
   * ```
   * @return empty param  if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerCallback(GUIJmbParam<T> &iParam,
                                         Parameters::ChangeCallback1<GUIJmbParam<T>> iChangeCallback,
                                         bool iInvokeCallback = false) {
    return registerJmbCallback<T>(iParam.getParamID(), std::move(iChangeCallback), iInvokeCallback);
  }

  /**
   * Called during initialization. Jamba takes care of properly initializing instances of this class with the
   * gui state when they get instantiated.
   *
   * @note Subclasses are allowed to extend this behavior (if you want to do some extra setup when the view
   *       is initialized for example), but care should be taken in making sure that this method ends up being
   *       called, otherwise the view will most likely not behave as expected. */
  virtual void initState(GUIState *iGUIState);

  /**
   * Subclasses should override this method to register each parameter. Jamba automatically calls this method and you
   * should not have to invoke it yourself.
   *
   * \note Jamba automatically calls `unregisterAll()` prior to calling this method so it is ok to assume that there
   *       is no parameter registered prior to this call.
   *
   * \note This method may be called multiple times during the life of the class inheriting from `ParamAware`, which
   *       is generally the case while using the %VSTGUI editor
   */
  virtual void registerParameters()
  {
    // subclasses implements this method
  }

  /**
   * Unregisters all parameters */
  void unregisterAll();

  /**
   * Removes the registration of the provided param (closing the connection/stopping to listen)
   *
   * @return `true` if the param was present, `false` otherwise
   */
   [[deprecated("Since 4.0.0 - If you need this kind of granularity, maintain your own connections")]]
  bool unregisterParam(ParamID iParamID) { return false; }

  /**
   * Removes the registration of the provided param (closing the connection/stopping to listen)
   *
   * @return an empty param so that you can write `param = unregisterParam(param)`
   */
  [[deprecated("Since 4.0.0 - If you need this kind of granularity, maintain your own connections")]]
  GUIRawVstParam unregisterParam(GUIRawVstParam const &iParam) { return iParam; }

  /**
   * Removes the registration of the provided param (closing the connection/stopping to listen)
   *
   * @return an empty param so that you can write `param = unregisterParam(param)`
   */
  template<typename T>
  [[deprecated("Since 4.0.0 - If you need this kind of granularity, maintain your own connections")]]
  GUIVstParam<T> unregisterParam(GUIVstParam<T> const &iParam) { return iParam; }

  /**
 * Removes the registration of the provided param (closing the connection/stopping to listen)
 *
 * @return an empty param so that you can write `param = unregisterParam(param)`
 */
  template<typename T>
  [[deprecated("Since 4.0.0 - If you need this kind of granularity, maintain your own connections")]]
  GUIJmbParam<T> unregisterParam(GUIJmbParam<T> const &iParam) { return iParam; }

  /**
   * This is the callback that should be implemented to handle parameter changes. This callback is only invoked
   * if any `registerXXParam` method is called. */
  void onParameterChange(ParamID iParamID) override {}

  /**
   * Invoke all (currently) registered callbacks and `onParameterChange()` (if registered). */
  void invokeAll();

protected:
  // Access to parameters
  std::unique_ptr<GUIParamCxMgr> fParamCxMgr{};
};

/**
 * This subclass allows for registering callbacks to any kind of view without having to inherit from it.
 *
 * @tparam TView should be a subclass of `VSTGUI::CView`
 * @see StateAwareCustomController::makeParamAware()
 */
template<typename TView>
class ParamAwareView : public ParamAware
{
  // ensures that TView is a subclass of CView
  static_assert(std::is_convertible<TView *, VSTGUI::CView*>::value, "TView must be a subclass of CView");

public:
  // required because we implement other registerCallback methods
  using ParamAware::registerCallback;

  // defining the change listener callback
  using ChangeListener = std::function<void(TView *, ParamID)>;

  // Constructor
  explicit ParamAwareView(TView *iView) : fView(iView), fListener{} {}

  /**
   * Registers a listener invoked when a parameter changes.
   *
   * Since this class is intended to be used without inheriting from a view, you cannot implement the
   * `onParameterChange()` method. As a result, you can use this method to set a listener that will be invoked
   * instead.
   */
  void registerListener(ChangeListener iListener)
  {
    fListener = std::move(iListener);
  }

  /**
   * Register a callback for the Vst parameter. This version is specific to this class and allows for
   * the view to be passed back (since the view is not inheriting from this class).
   *
   * ```
   * // Example
   * registerCallback<bool>(fParams->fMyParam,
   *                        [] (CTextLabel *iView, GUIVstParam<bool> &iParam) {
   *                             iParam.getValue()...;
   *                        });
   * ```
   *
   * @return empty param if not found or not proper type
   */
  template<typename T>
  inline GUIVstParam<T> registerCallback(VstParam<T> const &iParamDef,
                                         Parameters::ChangeCallback2<TView, GUIVstParam<T>> iChangeCallback,
                                         bool iInvokeCallback = false)
  {
    auto callback = [view = this->fView, cb2 = std::move(iChangeCallback)] (GUIVstParam<T> &iParam) {
      cb2(view, iParam);
    };

    return ParamAware::registerCallback<T>(iParamDef, std::move(callback), iInvokeCallback);
  }

  /**
   * Register a callback for the Jmb parameter. This version is specific to this class and allows for
   * the view to be passed back (since the view is not inheriting from this class).
   *
   * ```
   * // Example
   * registerCallback<Range>(fState->fMyParam,
   *                         [] (CTextLabel *iView, GUIJmbParam<Range> &iParam) {
   *                               iParam.getValue()...;
   *                          });
   * ```
   *
   * @return empty param if not found or not proper type
   */
  template<typename T>
  inline GUIJmbParam<T> registerCallback(GUIJmbParam<T> &iParam,
                                         Parameters::ChangeCallback2<TView, GUIJmbParam<T>> iChangeCallback,
                                         bool iInvokeCallback = false)
  {
    auto callback = [view = this->fView, cb2 = std::move(iChangeCallback)] (GUIJmbParam<T> &iParam) {
      cb2(view, iParam);
    };

    return ParamAware::registerCallback<T>(iParam, std::move(callback), iInvokeCallback);
  }

  /**
   * This implementation simply delegates to the listener registered by `registerListener()`.
   */
  void onParameterChange(ParamID iParamID) override
  {
    if(fListener)
      fListener(fView, iParamID);
  }

protected:
  TView *fView;
  ChangeListener fListener;
};

}
}
