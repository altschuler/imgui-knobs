#include "knobs.h"

#include <cmath>
#include <cstdlib>
#include <imgui.h>
#include <imgui_internal.h>
#include <utility>

namespace ImGuiKnobs {
    namespace detail {
        std::pair<ImVec2, ImVec2> bezier_arc(ImVec2 center, ImVec2 start, ImVec2 end) {
            auto ax = start[0] - center[0];
            auto ay = start[1] - center[1];
            auto bx = end[0] - center[0];
            auto by = end[1] - center[1];
            auto q1 = ax * ax + ay * ay;
            auto q2 = q1 + ax * bx + ay * by;
            auto k2 = (4.0f / 3.0f) * (sqrtf((2.0f * q1 * q2)) - q2) / (ax * by - ay * bx);

            return {
                    {center[0] + ax - k2 * ay, center[1] + ay + k2 * ax},
                    {center[0] + bx + k2 * by, center[1] + by - k2 * bx},
            };
        }

        void draw_arc1(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImColor color, int num_segments) {
            ImVec2 start = {
                    center[0] + cosf(start_angle) * radius,
                    center[1] + sinf(start_angle) * radius,
            };

            ImVec2 end = {
                    center[0] + cosf(end_angle) * radius,
                    center[1] + sinf(end_angle) * radius,
            };

            auto arc = bezier_arc(center, start, end);

            auto *draw_list = ImGui::GetWindowDrawList();

            draw_list->AddBezierCurve(start, arc.first, arc.second, end, color, thickness, num_segments);
        }

        void draw_arc(ImVec2 center, float radius, float start_angle, float end_angle, float thickness, ImColor color, int num_segments, int bezier_count) {
            // Overlap and angle of ends of bezier curves needs work, only looks good when not transperant
            auto overlap = thickness * radius * 0.00001f * M_PI;
            auto delta = end_angle - start_angle;
            auto bez_step = 1.0f / bezier_count;
            auto mid_angle = start_angle + overlap;

            for (auto i = 0; i < bezier_count - 1; i++) {
                auto mid_angle2 = delta * bez_step + mid_angle;
                draw_arc1(
                        center,
                        radius,
                        mid_angle - overlap,
                        mid_angle2 + overlap,
                        thickness,
                        color,
                        num_segments);
                mid_angle = mid_angle2;
            }

            draw_arc1(
                    center,
                    radius,
                    mid_angle - overlap,
                    end_angle,
                    thickness,
                    color,
                    num_segments);
        }

        bool knob_control(const char *id, float *p_value, float v_min, float v_max, float v_default, float radius) {
            ImGui::InvisibleButton(id, {radius * 2.0f, radius * 2.0f});

            auto value_changed = false;

            auto is_active = ImGui::IsItemActive();
            auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.0001f);

            const auto &io = ImGui::GetIO();

            // Maybe this should be configurable
            auto speed = io.KeyShift ? 2000.0f : 200.0f;

            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && is_active) {
                *p_value = v_default;
                value_changed = true;
            } else if (is_active && delta[1] != 0.0) {
                auto step = (v_max - v_min) / speed;
                *p_value -= delta[1] * step;
                if (*p_value < v_min) {
                    *p_value = v_min;
                }
                if (*p_value > v_max) {
                    *p_value = v_max;
                }
                value_changed = true;

                // There may be a way to do this without using this
                ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
            }

            return value_changed;
        }

        knob::knob(const char *_label, float *_p_value, float _v_min, float _v_max, float _v_default, float _radius) {
            auto _angle_min = M_PI * 0.75;
            auto _angle_max = M_PI * 2.25;
            auto _t = (*_p_value - _v_min) / (_v_max - _v_min);
            auto _angle = _angle_min + (_angle_max - _angle_min) * _t;
            auto _value_changed = false;
            auto screen_pos = ImGui::GetCursorScreenPos();

            // Make an invisble button that handles drag behaviour
            _value_changed = knob_control(_label, _p_value, _v_min, _v_max, _v_default, _radius);

            label = _label;
            p_value = _p_value;
            v_min = _v_min;
            v_max = _v_max;
            v_default = _v_default;
            radius = _radius;
            value_changed = _value_changed;
            center = {screen_pos[0] + radius, screen_pos[1] + radius};
            is_active = ImGui::IsItemActive();
            is_hovered = ImGui::IsItemHovered();
            angle_min = _angle_min;
            angle_max = _angle_max;
            t = _t;
            angle = _angle;
            angle_cos = cosf(angle);
            angle_sin = sinf(angle);
        }

        void knob::draw_dot(float size, float radius, float angle, color_set color, bool filled, int segments) {
            auto dot_size = size * this->radius;
            auto dot_radius = radius * this->radius;

            ImGui::GetWindowDrawList()->AddCircleFilled(
                    {
                            center[0] + cosf(angle) * dot_radius,
                            center[1] + sinf(angle) * dot_radius,
                    },
                    dot_size,
                    is_active ? color.active : (is_hovered ? color.hovered : color.base),
                    segments);
        }

        void knob::draw_tick(float start, float end, float width, float angle, color_set color) {
            auto tick_start = start * radius;
            auto tick_end = end * radius;
            auto angle_cos = cosf(angle);
            auto angle_sin = sinf(angle);

            ImGui::GetWindowDrawList()->AddLine(
                    {
                            center[0] + angle_cos * tick_end,
                            center[1] + angle_sin * tick_end,
                    },
                    {
                            center[0] + angle_cos * tick_start,
                            center[1] + angle_sin * tick_start,
                    },
                    is_active ? color.active : (is_hovered ? color.hovered : color.base),
                    width * radius);
        }

        void knob::draw_circle(float size, color_set color, bool filled, int segments) {
            auto circle_radius = size * radius;

            ImGui::GetWindowDrawList()->AddCircleFilled(
                    center,
                    circle_radius,
                    is_active ? color.active : (is_hovered ? color.hovered : color.base));
        }

        void knob::draw_arc(float radius, float size, float start_angle, float end_angle, color_set color, int segments, int bezier_count) {
            auto track_radius = radius * this->radius;
            auto track_size = size * this->radius * 0.5f + 0.0001f;

            detail::draw_arc(
                    center,
                    track_radius,
                    start_angle,
                    end_angle,
                    track_size,
                    is_active ? color.active : (is_hovered ? color.hovered : color.base),
                    segments,
                    bezier_count);
        }

        knob knob_with_drag(const char *label, float *p_value, float v_min, float v_max, float v_default, const char *format, float size) {
            ImGui::PushID(label);
            auto width = size == 0 ? ImGui::GetTextLineHeight() * 4.0f : size * ImGui::GetIO().FontGlobalScale;
            ImGui::PushItemWidth(width);

            ImGui::BeginGroup();

            // There's an issue with `SameLine` and Groups, see https://github.com/ocornut/imgui/issues/4190.
            // This is probably not the best solution, but seems to work for now
            ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0;

            // Draw title
            auto title_size = ImGui::CalcTextSize(label, NULL, false, width);

            // Center title
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (width - title_size[0]) * 0.5f);

            ImGui::Text("%s", label);

            knob k(label, p_value, v_min, v_max, v_default, width * 0.5f);

            ImGui::DragScalar("###knob_drag", ImGuiDataType_Float, p_value, (v_max - v_min) / 1000.f, &v_min, &v_max, format);

            ImGui::EndGroup();
            ImGui::PopItemWidth();
            ImGui::PopID();
            return k;
        }

        color_set GetPrimaryColorSet() {
            auto *colors = ImGui::GetStyle().Colors;

            return {colors[ImGuiCol_ButtonActive], colors[ImGuiCol_ButtonHovered], colors[ImGuiCol_ButtonHovered]};
        }

        color_set GetSecondaryColorSet() {
            auto *colors = ImGui::GetStyle().Colors;
            auto active = colors[ImGuiCol_ButtonActive] * ImVec4(0.5f, 0.5f, 0.5f, 1.f);
            auto hovered = colors[ImGuiCol_ButtonHovered] * ImVec4(0.5f, 0.5f, 0.5f, 1.f);
            return {active, hovered, hovered};
        }

        color_set GetTrackColorSet() {
            auto *colors = ImGui::GetStyle().Colors;

            return {colors[ImGuiCol_FrameBg], colors[ImGuiCol_FrameBg], colors[ImGuiCol_FrameBg]};
        }
    }// namespace detail


    // Knob implementations
    KNOB_WIDGET(WiperKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_circle(0.7, detail::GetSecondaryColorSet(), true, 32);
        knob.draw_arc(0.8, 0.41, knob.angle_min, knob.angle_max, detail::GetTrackColorSet(), 16, 2);

        if (knob.t > 0.01) {
            knob.draw_arc(0.8, 0.43, knob.angle_min, knob.angle, detail::GetPrimaryColorSet(), 16, 2);
        }
        return knob.value_changed;
    }

    KNOB_WIDGET(WiperOnlyKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_arc(0.8, 0.41, knob.angle_min, knob.angle_max, detail::GetTrackColorSet(), 32, 2);

        if (knob.t > 0.01) {
            knob.draw_arc(0.8, 0.43, knob.angle_min, knob.angle, detail::GetPrimaryColorSet(), 16, 2);
        }
        return knob.value_changed;
    }

    KNOB_WIDGET(WiperDotKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_circle(0.6, detail::GetSecondaryColorSet(), true, 32);
        knob.draw_arc(0.85, 0.41, knob.angle_min, knob.angle_max, detail::GetTrackColorSet(), 16, 2);
        knob.draw_dot(0.1, 0.85, knob.angle, detail::GetPrimaryColorSet(), true, 12);
        return knob.value_changed;
    }


    KNOB_WIDGET(TickKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_circle(0.7, detail::GetSecondaryColorSet(), true, 32);
        knob.draw_tick(0.4, 0.7, 0.08, knob.angle, detail::GetPrimaryColorSet());
        return knob.value_changed;
    }

    KNOB_WIDGET(DotKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_circle(0.85, detail::GetSecondaryColorSet(), true, 32);
        knob.draw_dot(0.12, 0.6, knob.angle, detail::GetPrimaryColorSet(), true, 12);
        return knob.value_changed;
    }

    KNOB_WIDGET(SpaceKnob) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        knob.draw_circle(0.3 - knob.t * 0.1, detail::GetSecondaryColorSet(), true, 16);

        if (knob.t > 0.01) {
            knob.draw_arc(0.4, 0.15, knob.angle_min - 1.0, knob.angle - 1.0, detail::GetPrimaryColorSet(), 16, 2);
            knob.draw_arc(0.6, 0.15, knob.angle_min + 1.0, knob.angle + 1.0, detail::GetPrimaryColorSet(), 16, 2);
            knob.draw_arc(0.8, 0.15, knob.angle_min + 3.0, knob.angle + 3.0, detail::GetPrimaryColorSet(), 16, 2);
        }

        return knob.value_changed;
    }

    KNOB_WIDGET(SteppedKnob, int steps) {
        auto knob = detail::knob_with_drag(title, p_value, v_min, v_max, 0, format, size);
        for (auto n = 0.f; n < steps; n++) {
            auto a = n / (steps - 1);
            auto angle = knob.angle_min + (knob.angle_max - knob.angle_min) * a;
            knob.draw_tick(0.7, 0.9, 0.04, angle, detail::GetPrimaryColorSet());
        }
        knob.draw_circle(0.6, detail::GetSecondaryColorSet(), true, 32);
        knob.draw_dot(0.12, 0.4, knob.angle, detail::GetPrimaryColorSet(), true, 12);
        return knob.value_changed;
    }
}// namespace ImGuiKnobs
