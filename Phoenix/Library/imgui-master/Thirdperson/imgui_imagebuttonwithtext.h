#pragma once

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif


// Declaration (.h file)
namespace ImGui
{
    // label is used as id
    // If direction is true, the characters are placed vertically, if false, the characters are placed horizontally.
    // <0 frame_padding uses default frame padding settings. 0 for no padding
    IMGUI_API bool ImageButtonWithText(ImTextureID texId, const char* label, bool direction, bool* out_hovered, const ImVec2& imageSize = ImVec2(0, 0), const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
} // namespace ImGui
