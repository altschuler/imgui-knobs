#pragma once

#include <imgui.h>
#include <utility>

typedef int ImGuiKnobFlags;

#define KNOB_WIDGET_DEF(name, ...) bool name(const char *title, float *p_value, float v_min, float v_max, const char *format, float size = 0, ImGuiKnobFlags flags = 0, ##__VA_ARGS__)
#define KNOB_WIDGET(name, ...) bool name(const char *title, float *p_value, float v_min, float v_max, const char *format, float size, ImGuiKnobFlags flags, ##__VA_ARGS__)

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

    namespace detail {
        std::pair<ImVec2, ImVec2> bezier_arc(ImVec2 center, ImVec2 start, ImVec2 end);

        void draw_arc1(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImColor color, int num_segments);
        void draw_arc(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImColor color, int num_segments, int bezier_count);

        struct knob {
            const char *label;
            float *p_value;
            float v_min;
            float v_max;
            float v_default;
            float radius;
            bool value_changed;
            ImVec2 center;
            bool is_active;
            bool is_hovered;
            float angle_min;
            float angle_max;
            float t;
            float angle;
            float angle_cos;
            float angle_sin;

            knob(const char *_label, float *_p_value, float _v_min, float _v_max, float _v_default, float _radius);

            void draw_dot(float size, float radius, float angle, color_set color, bool filled, int segments);
            void draw_tick(float start, float end, float width, float angle, color_set color);
            void draw_circle(float size, color_set color, bool filled, int segments);
            void draw_arc(float arc_radius, float size, float start_angle, float end_angle, color_set color, int segments, int bezier_count);
        };

        bool knob_control(const char *id, float *p_value, float v_min, float v_max, float v_default, float radius, ImGuiKnobFlags flags);
        knob knob_with_drag(const char *title, float *p_value, float v_min, float v_max, float v_default, const char *format, float size = 0, ImGuiKnobFlags flags = 0);

        color_set GetPrimaryColorSet();
        color_set GetSecondaryColorSet();
        color_set GetTrackColorSet();

    }// namespace detail

    KNOB_WIDGET_DEF(TickKnob);
    KNOB_WIDGET_DEF(DotKnob);
    KNOB_WIDGET_DEF(WiperKnob);
    KNOB_WIDGET_DEF(WiperOnlyKnob);
    KNOB_WIDGET_DEF(WiperDotKnob);
    KNOB_WIDGET_DEF(SteppedKnob, int steps = 10);
    KNOB_WIDGET_DEF(SpaceKnob);
}// namespace ImGuiKnobs
