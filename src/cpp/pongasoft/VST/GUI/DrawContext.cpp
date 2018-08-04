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
#include "DrawContext.h"
#include <vstgui4/vstgui/lib/controls/ccontrol.h>

namespace pongasoft {
namespace VST {
namespace GUI {

using namespace VSTGUI;

void RelativeDrawContext::drawString(UTF8String const &fText, RelativeRect const &fSize, StringDrawContext &sdc)
{
  if(!(sdc.fStyle & kNoTextStyle))
  {
    fDrawContext->saveGlobalState();
    CRect textRect = toAbsoluteRect(fSize);
    textRect.inset(sdc.fTextInset.x, sdc.fTextInset.y);

    CRect oldClip;
    fDrawContext->getClipRect(oldClip);
    CRect newClip(textRect);
    newClip.bound(oldClip);
    fDrawContext->setClipRect(newClip);

    if(sdc.fAntialias)
      fDrawContext->setDrawMode(kAntiAliasing);
    fDrawContext->setFont(sdc.fFont);

    // draw darker text (as shadow)
    if(sdc.fStyle & kShadowText)
    {
      CRect newSize(textRect);
      newSize.offset(sdc.fShadowTextOffset);
      fDrawContext->setFontColor(sdc.fShadowColor);
      fDrawContext->drawString(fText.getPlatformString(), newSize, sdc.fHorizTxtAlign, sdc.fAntialias);
    }
    fDrawContext->setFontColor(sdc.fFontColor);
    fDrawContext->drawString(fText.getPlatformString(), textRect, sdc.fHorizTxtAlign, sdc.fAntialias);
    fDrawContext->restoreGlobalState();
  }

}
}
}
}