/*
 * Copyright (c) 2018 pongasoft
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

#include <vstgui4/vstgui/lib/cdrawcontext.h>
#include <vstgui4/vstgui/lib/cview.h>
#include <pongasoft/Utils/Misc.h>
#include <base/source/fstring.h>

#include "Types.h"

namespace pongasoft {
namespace VST {
namespace GUI {

using namespace VSTGUI;

/**
 * Implementation note: The `CControlEnum` enumeration has been removed from VSTGUI. Since some of the values
 * were being used by Jamba, this (deprecated) enumeration serves 2 purposes:
 * - keep the same values that were present in the old enum
 * - trigger deprecation warning when compiling
 *
 * @deprecated Since 5.0.0 - Use `StringDrawContext::Style` enum instead
 */
enum [[deprecated("Since 5.0.0 - Use StringDrawContext::Style enum instead")]] DeprecatedCControlEnum
{
  kShadowText			= 1 << 2,
  kNoTextStyle		= 1 << 11,
};

/**
 * The context which contains the details on how the string should be drawn. */
struct StringDrawContext
{
  enum class Style : int32_t
  {
    kShadowText			= 1 << 2,
    kNoTextStyle		= 1 << 11
  };

  CHoriTxtAlign fHorizTxtAlign{kCenterText};
  //! Should be a value provided by `StringDrawContext::Style`
  int32_t fStyle{0};
  FontPtr fFont{nullptr};
  CColor fFontColor{kWhiteCColor};
  CColor fShadowColor{kBlackCColor};
  CPoint fTextInset{0, 0};
  CPoint fShadowTextOffset{1., 1.};
  bool fAntialias{true};

  constexpr bool hasStyle(Style iStyle) const { return fStyle & static_cast<int32_t>(iStyle); }
  inline void setStyle(Style iStyle) { fStyle = static_cast<int32_t>(iStyle); }
  inline void addStyle(Style iStyle) { fStyle |= static_cast<int32_t>(iStyle); }
};

using RelativeCoord = CCoord;
using AbsoluteCoord = CCoord;
using RelativePoint = CPoint;
using AbsolutePoint = CPoint;
using RelativeRect = CRect;
using AbsoluteRect = CRect;

class RelativeView
{
public:
  explicit RelativeView(CView const *iView) : fRect{iView->getViewSize()}
  {
  }

  explicit RelativeView(AbsoluteRect const &iRect) : fRect{iRect}
  {
  }

  inline RelativeRect getViewSize() const
  {
    return RelativeRect(0, 0, fRect.getWidth(), fRect.getHeight());
  }

  inline CCoord getWidth() const
  {
    return fRect.getWidth();
  }

  inline Range getHorizontalRange() const { return Range{0, getWidth()}; }

  inline Range getVerticalRange() const { return Range{0, getHeight()}; }

  inline CCoord getHeight() const
  {
    return fRect.getHeight();
  }

  inline AbsoluteCoord toAbsoluteX(RelativeCoord x) const
  {
    return x + fRect.left;
  }

  inline AbsoluteCoord toAbsoluteY(RelativeCoord y) const
  {
    return y + fRect.top;
  }

  inline RelativeCoord fromAbsoluteX(AbsoluteCoord x) const
  {
    return x - fRect.left;
  }

  inline RelativeCoord fromAbsoluteY(AbsoluteCoord y) const
  {
    return y - fRect.top;
  }

  inline AbsolutePoint toAbsolutePoint(RelativePoint const &iPoint) const
  {
    return AbsolutePoint{toAbsoluteX(iPoint.x), toAbsoluteY(iPoint.y)};
  }

  inline RelativePoint fromAbsolutePoint(AbsolutePoint const &iPoint) const
  {
    return RelativePoint{fromAbsoluteX(iPoint.x), fromAbsoluteY(iPoint.y)};
  }

  /**
   * Convert the absolute point to a relative point while making sure it is clamped (constrained)
   * within this relative view.
   */
  inline RelativePoint clampAbsolutePoint(AbsolutePoint const &iPoint) const
  {
    return RelativePoint{
      Utils::clamp(fromAbsoluteX(iPoint.x), 0.0, fRect.getWidth()),
      Utils::clamp(fromAbsoluteY(iPoint.y), 0.0, fRect.getHeight())
    };
  }

  inline AbsolutePoint toAbsolutePoint(RelativeCoord x, RelativeCoord y) const
  {
    return CPoint{x + fRect.left, y + fRect.top};
  }

  inline AbsoluteRect toAbsoluteRect(RelativeRect const &iRect) const
  {
    return CRect(toAbsolutePoint(iRect.getTopLeft()), iRect.getSize());
  }

protected:
  AbsoluteRect const &fRect;
};

/**
 * Encapsulates the draw context provided by VSTGUI to reason in relative coordinates (0,0) is top,left
 */
class RelativeDrawContext : public RelativeView
{
public:
  RelativeDrawContext(CView *iView, CDrawContext *iDrawContext) : RelativeView{iView}, fDrawContext{iDrawContext}
  {
  }

  void drawLine(RelativeCoord x1, RelativeCoord y1, RelativeCoord x2, RelativeCoord y2, CColor const &color)
  {
    fDrawContext->setFrameColor(color);
    fDrawContext->drawLine(toAbsolutePoint(x1, y1), toAbsolutePoint(x2, y2));
  }

  void drawRect(RelativeCoord x1, RelativeCoord y1, RelativeCoord x2, RelativeCoord y2, CColor const &iStrokeColor)
  {
    drawRect(RelativeRect{x1, y1, x2, y2}, iStrokeColor);
  }

  void drawRect(RelativeRect const &iRect, CColor const &iStrokeColor)
  {
    fDrawContext->setFrameColor(iStrokeColor);
    fDrawContext->drawRect(toAbsoluteRect(iRect), kDrawStroked);
  }

  void fillRect(RelativeCoord x1, RelativeCoord y1, RelativeCoord x2, RelativeCoord y2, CColor const &iColor)
  {
    fillRect(RelativeRect{x1, y1, x2, y2}, iColor);
  }

  void fillRect(RelativeRect const &iRect, CColor const &iColor)
  {
    fDrawContext->setFillColor(iColor);
    fDrawContext->drawRect(toAbsoluteRect(iRect), kDrawFilled);
  }

  void fillAndStrokeRect(RelativeRect const &iRect, CColor const &iFillColor, CColor const &iStrokeColor)
  {
    fDrawContext->setFillColor(iFillColor);
    fDrawContext->setFrameColor(iStrokeColor);
    fDrawContext->drawRect(toAbsoluteRect(iRect), kDrawFilledAndStroked);
  }

  void drawEllipse(RelativeCoord x1, RelativeCoord y1, RelativeCoord x2, RelativeCoord y2, CColor const &iStrokeColor)
  {
    drawEllipse(RelativeRect{x1, y1, x2, y2}, iStrokeColor);
  }

  void drawEllipse(RelativeRect const &iRect, CColor const &iStrokeColor)
  {
    fDrawContext->setFrameColor(iStrokeColor);
    fDrawContext->drawEllipse(toAbsoluteRect(iRect), kDrawStroked);
  }

  void fillEllipse(RelativeCoord x1, RelativeCoord y1, RelativeCoord x2, RelativeCoord y2, CColor const &iColor)
  {
    fillEllipse(RelativeRect{x1, y1, x2, y2}, iColor);
  }

  void fillEllipse(RelativeRect const &iRect, CColor const &iColor)
  {
    fDrawContext->setFillColor(iColor);
    fDrawContext->drawEllipse(toAbsoluteRect(iRect), kDrawFilled);
  }

  void fillAndStrokeEllipse(RelativeRect const &iRect, CColor const &iFillColor, CColor const &iStrokeColor)
  {
    fDrawContext->setFillColor(iFillColor);
    fDrawContext->setFrameColor(iStrokeColor);
    fDrawContext->drawEllipse(toAbsoluteRect(iRect), kDrawFilledAndStroked);
  }

  void drawString(UTF8String const &iText, RelativeCoord x, RelativeCoord y, RelativeCoord iHeight, StringDrawContext &iSdc)
  {
    CRect size{x, y, fDrawContext->getStringWidth(iText.getPlatformString()), iHeight};
    drawString(iText, size, iSdc);
  }

  void drawString(UTF8String const &iText, RelativeRect const &fSize, StringDrawContext &iSdc);

  void drawString(UTF8String const &iText, StringDrawContext &iSdc)
  {
    drawString(iText, getViewSize(), iSdc);
  }

#if EDITOR_MODE
  /**
   * Draw a string containing the formatted string (printf style) at (x,y)
   *
   * @param x if -1 means right
   * @param y if -1 means bottom
   */
  template<typename... Args>
  inline void debugXY(RelativeCoord x, RelativeCoord y, char const *iFormat, Args&& ...iArgs)
  {
    // handles strings up to 4k characters so should be fine for debug...
    auto s = Steinberg::String().printf(iFormat, std::forward<Args>(iArgs)...);
    debugText(x, y, s.text8());
  }

  // debugTopLeft
  template<typename... Args>
  inline void debugTopLeft(char const *iFormat, Args&& ...iArgs) { debugXY(0, 0, iFormat, std::forward<Args>(iArgs)...); }

  // debugTopRight
  template<typename... Args>
  inline void debugTopRight(char const *iFormat, Args&& ...iArgs) { debugXY(-1, 0, iFormat, std::forward<Args>(iArgs)...); }

  // debugBottomLeft
  template<typename... Args>
  inline void debugBottomLeft(char const *iFormat, Args&& ...iArgs) { debugXY(0, -1, iFormat, std::forward<Args>(iArgs)...); }

  // debugBottomRight
  template<typename... Args>
  inline void debugBottomRight(char const *iFormat, Args&& ...iArgs) { debugXY(-1, -1, iFormat, std::forward<Args>(iArgs)...); }

  /**
   * Draw a string containing the formatted string (printf style) at the top of the view (0,0)
   */
  template<typename... Args>
  inline void debug(char const *iFormat, Args&& ...iArgs) { debugTopLeft(iFormat, std::forward<Args>(iArgs)...); }

  /**
   * Draw the text at the top of the view (0,0)
   */
  inline void debugText(char const *iText) { debugText(0, 0, iText); }

  /**
   * Draw the text at the top of the view (x,y)
   */
  void debugText(RelativeCoord x, RelativeCoord y, char const *iText);

#endif

protected:
  CDrawContext *fDrawContext;

#if EDITOR_MODE
public:
  CColor fDebugStringColor{kGreenCColor};
  FontPtr fDebugStringFont{kNormalFontVerySmall};
#endif
};

}
}
}