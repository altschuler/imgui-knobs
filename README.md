# ImGui Knobs
This is a port/adaptation of [imgui-rs-knobs](https://github.com/DGriffin91/imgui-rs-knobs), for C++ with slightly changed API.

![image](https://user-images.githubusercontent.com/956928/164050142-96a8dde4-7d2e-43e4-9afe-14ab48eac243.png)

## Usage
Add `imgui-knobs.cpp` and `imgui-knobs.h` to your project and include `imgui-knobs.h` in some source file, then:

```cpp
static float value = 0;

if (ImGuiKnobs::WiperKnob("Volume", &value, -6.0f, 6.0f, "%.1fdB")) {
    // value was changed
}
```

Available knob variants are: `TickKnob`, `DotKnob`, `WiperKnob`, `WiperOnlyKnob`, `WiperDotKnob`, `SteppedKnob`, `SpaceKnob`. They differ only in their visual appearance.

### Size
You can specify a size given as the width of the knob (will be scaled according to ImGui's `FontGlobalScale`). Default (0) will use 4x line height.

```cpp
// Draw a knob that is 80px wide
ImGuiKnobs::WiperKnob("Volume", &value, -6.0f, 6.0f, "%.1fdB", 80);
```

### Flags
There are flags to hide the title, hide the input and to show the current value in a tooltip when the knob is hovered.

```cpp
ImGuiKnobs::WiperKnob("No title", &value, -6.0f, 6.0f, "%.1fdB", 0, ImGuiKnobFlags_NoTitle);

ImGuiKnobs::DotKnob("Pitch", &value, 0, 2, "%.1f", 0, ImGuiKnobFlags_NoInput | ImGuiKnobFlags_NoTitle | ImGuiKnobFlags_ValueTooltip);
```


