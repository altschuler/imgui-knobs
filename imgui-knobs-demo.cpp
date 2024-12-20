#include <imgui.h>

#include "imgui-knobs.h"

namespace ImGuiKnobs {
    constexpr char kFormat[] = "%.2f";
    void ShowDemoWindow(bool *p_open) {
        if (ImGui::Begin("imgui-knobs-demo", p_open)) {
            if (ImGui::CollapsingHeader("Linear")) {
                static float val1 = 0;
                if (ImGuiKnobs::Knob("Gain", &val1, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Tick)) {
                    // value was changed
                }

                ImGui::SameLine();

                static float val2 = 0;
                if (ImGuiKnobs::Knob("Mix", &val2, -1.0f, 1.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped)) {
                    // value was changed
                }

                // Double click to reset
                if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
                    val2 = 0;
                }

                ImGui::SameLine();

                static float val3 = 0;

                // Custom colors
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(255.f, 0, 0, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.f, 0, 0, 1));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255.f, 0, 1));
                // Push/PopStyleColor() for each colors used (namely ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered for primary and ImGuiCol_Framebg for Track)
                if (ImGuiKnobs::Knob("Pitch", &val3, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_WiperOnly)) {
                    // value was changed
                }

                ImGui::PopStyleColor(3);


                ImGui::SameLine();

                static float val4 = 0;

                // Custom min/max angle
                if (ImGuiKnobs::Knob("Dry", &val4, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped, 0, 0, 10, 1.570796f, 3.141592f)) {
                    // value was changed
                }

                ImGui::SameLine();

                static int val5 = 0;

                // Int value
                if (ImGuiKnobs::KnobInt("Wet", &val5, 1, 10, 0.1f, "%i", ImGuiKnobVariant_Stepped, 0, 0, 10)) {
                    // value was changed
                }

                ImGui::SameLine();

                static float val6 = 0;

                // Vertical drag only
                if (ImGuiKnobs::Knob("Vertical", &val6, 0.f, 10.f, 0.1f, "%.1f", ImGuiKnobVariant_Space, 0, ImGuiKnobFlags_DragVertical)) {
                    // value was changed
                }
            }
            if (ImGui::CollapsingHeader("Logarithmic")) {

                static float val7 = 0;
                if (ImGuiKnobs::Knob("f#0", &val7, 20, 20000, 20.0f, "%.1fHz", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic)) {
                    // value was changed
                }

                ImGui::SameLine();
                static float val8 = 0;
                if (ImGuiKnobs::Knob("f#1", &val8, 0, 20000, 20.0f, "%.1fHz", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic)) {
                    // value was changed
                }

                ImGui::SameLine();
                static float val9 = 0;
                if (ImGuiKnobs::Knob("f#2", &val9, -20000, 20000, 20.0f, "%.1fHz", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic)) {
                    // value was changed
                }

                ImGui::SameLine();
                static float val10 = 0;
                if (ImGuiKnobs::Knob("f#3", &val10, -20000, -20, 20.0f, "%.1fHz", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic)) {
                    // value was changed
                }

                ImGui::SameLine();
                static float val11 = 0;
                if (ImGuiKnobs::Knob("f#4", &val11, 0, 0, 0, "%.1fHz", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic)) {
                    // value was changed
                }

            }
            ImGui::End();
        }
    }

}// namespace ImGuiKnobs
