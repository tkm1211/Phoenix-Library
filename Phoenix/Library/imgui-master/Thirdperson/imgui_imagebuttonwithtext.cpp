#include "imgui_imagebuttonwithtext.h"


// Definition (.cpp file. Not sure if it needs "imgui_internal.h" or not)
namespace ImGui
{
    bool ImageButtonWithText(ImTextureID texId, const char* label, bool direction, bool* out_hovered, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImVec2 size = imageSize;
        if (size.x <= 0 && size.y <= 0) { size.x = size.y = ImGui::GetTextLineHeightWithSpacing(); }
        else {
            if (size.x <= 0)          size.x = size.y;
            else if (size.y <= 0)     size.y = size.x;
            size *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
        }

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(label);
        const ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);

        if (direction)
        {
            const bool hasText = textSize.y > 0;

            const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.y)) : 0.f;
            const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
            const ImVec2 totalSizeWithoutPadding(size.x, size.y + innerSpacing + textSize.y);
            const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);

            ImVec2 start(0, 0);
            start = window->DC.CursorPos + padding;
            if (size.y < textSize.y)
            {
                start.y += (textSize.y - size.y) * .5f;
            }

            const ImRect image_bb(start, start + size);
            start = window->DC.CursorPos + padding;
            start.y += size.y + innerSpacing;

            size_t labelNum = strlen(label);
            float textSizeX = textSize.x / labelNum;
            size_t num = 0;
            if (size.x < textSize.x)
            {

                const char* s = label;
                for (size_t i = 0; i < labelNum; i++)
                {
                    if (size.x < textSizeX * i)
                    {
                        num = i;
                        break;
                    }
                    s += 1;
                }
            }

            const char* label_end = 0 < num ? label + (num - 3) : NULL;

            ItemSize(bb);
            if (!ItemAdd(bb, id))
                return false;

            bool hovered = false, held = false;
            bool pressed = ButtonBehavior(bb, id, &hovered, &held);
            (*out_hovered) = hovered;

            // Render
            const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
            RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
            if (bg_col.w > 0.0f)
                window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

            window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

            if (textSize.x > 0)
            {
                ImGui::RenderText(start, label, label_end);
                if (0 < num)
                {
                    const ImVec2 newTextSize = ImGui::CalcTextSize(label, label_end, true);
                    start.x += newTextSize.x;
                    ImGui::RenderText(start, "...");
                }
            }
            return pressed;
        }
        else
        {
            const bool hasText = textSize.x > 0;

            const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
            const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
            const ImVec2 totalSizeWithoutPadding(280.5f, size.y > textSize.y ? size.y : textSize.y);
            const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
            ImVec2 start(0, 0);
            start = window->DC.CursorPos + padding; if (size.y < textSize.y) start.y += (textSize.y - size.y) * .5f;
            const ImRect image_bb(start, start + size);
            start = window->DC.CursorPos + padding; start.x += size.x + innerSpacing; if (size.y > textSize.y) start.y += (size.y - textSize.y) * .5f;
            ItemSize(bb);
            if (!ItemAdd(bb, id))
                return false;

            bool hovered = false, held = false;
            bool pressed = ButtonBehavior(bb, id, &hovered, &held);
            (*out_hovered) = hovered;

            // Render
            const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
            RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
            if (bg_col.w > 0.0f)
                window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

            window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

            if (textSize.x > 0) ImGui::RenderText(start, label);
            return pressed;
        }
    }
} // namespace ImGui
