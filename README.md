# ImGui Knobs
This is a port/adaptation of [imgui-rs-knobs](https://github.com/DGriffin91/imgui-rs-knobs), for C++ with slightly changed API.

## Usage
Include `imgui-knobs.cpp` and `imgui-knobs.h` in your project and include `imgui-knobs.h` in some source file, then:

```cpp
static float value = 0;

if (ImGuiKnobs::WiperKnob("Volume", &value, -6.0f, 6.0f, "%.1fdB")) {
    // value was changed
}
```

Available knob variants are: `TickKnob`, `DotKnob`, `WiperKnob`, `WiperOnlyKnob`, `WiperDotKnob`, `SteppedKnob`, `SpaceKnob`. They differ only in their visual appearance.

