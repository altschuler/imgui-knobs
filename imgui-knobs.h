#pragma once

#include <imgui.h>

typedef int ImGuiKnobFlags;

enum ImGuiKnobFlags_ {
    ImGuiKnobFlags_NoTitle = 1 << 0,
    ImGuiKnobFlags_NoInput = 1 << 1,
    ImGuiKnobFlags_ValueTooltip = 1 << 2
};

namespace ImGuiKnobs {

    struct color_set {
        ImColor base;
        ImColor hovered;
        ImColor active;

        color_set(){};

        color_set(ImColor base, ImColor hovered, ImColor active) : base(base), hovered(hovered), active(active) {}

        color_set(ImColor color) {
            base = color;
            hovered = color;
            active = color;
        }
    };

    bool TickKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
    bool DotKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
    bool WiperKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
    bool WiperOnlyKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
    bool WiperDotKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
    bool SteppedKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0, int steps = 10);
    bool SpaceKnob(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0);
}// namespace ImGuiKnobs
