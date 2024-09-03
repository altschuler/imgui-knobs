#include "../imgui-knobs.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>

SDL_GLContext gl_context;
void draw_knobs();
SDL_Window *init();
bool begin_frame(SDL_Window *window);
void end_frame(SDL_Window *window);
void teardown(SDL_Window *window);

int main(int, char **) {
    SDL_Window *window = init();

    ImGuiIO &io = ImGui::GetIO();

    // quick fix for hdpi screens
    io.FontGlobalScale = 2;

    while (begin_frame(window)) {
        {
            ImGui::Begin("Knob knob");

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

            // Custom min/max angle
            static float val4 = 0;
            if (ImGuiKnobs::Knob("Dry", &val4, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped, 0, 0, 10, 1.570796f, 3.141592f)) {
                // value was changed
            }

            ImGui::SameLine();

            // Int value
            static int val5 = 1;
            if (ImGuiKnobs::KnobInt("Wet", &val5, 1, 10, 0.1f, "%i", ImGuiKnobVariant_Stepped, 0, 0, 10)) {
                // value was changed
            }

            ImGui::SameLine();

            // Vertical drag only
            static float val6 = 1;
            if (ImGuiKnobs::Knob("Vertical", &val6, 0.f, 10.f, 0.1f, "%.1f", ImGuiKnobVariant_Space, 0, ImGuiKnobFlags_DragVertical)) {
                // value was changed
            }


            ImGui::End();
        }

        end_frame(window);
    }

    teardown(window);

    return 0;
}

/******************
 * ImGui handling
 ******************/

SDL_Window *init() {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
        0) {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags =
            (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                               SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window =
            SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr) {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return NULL;
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);// Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
    io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad;// Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    return window;
}

bool begin_frame(SDL_Window *window) {
    bool done = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT &&
            event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return !done;
}

void end_frame(SDL_Window *window) {
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO &io = ImGui::GetIO();
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    // glUseProgram(0); // You may want this if using this code in an OpenGL 3+
    // context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void teardown(SDL_Window *window) {

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
