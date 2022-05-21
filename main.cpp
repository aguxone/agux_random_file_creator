// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read dbocumentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
// Agregamos lo del ImGUI builder
#include "imgui_gui_propia.cpp"

// Librerías de la api de win32 sólo para minimizar a tray
// aportan peso despreciable igual
#include <windows.h>
#include <winuser.h>
#include <shellapi.h>

// Librería necesaria para minimizar a tray. Es parte de SDL, Lo que hace es sacar
// info de las ventanas en objeto tipo "info",
// y un field (atributo) de los info es de clase HWND que
// son los tipos que maneja la win32 api por lo que nos da un medio de interacción
// con windows esto!
#include <SDL_syswm.h>


#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif


// Main code
int main(int, char**)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    // SDL_WINDOW_ALLOW_HIGHDPI | 
    // | SDL_WINDOW_RESIZABLE | Saqué esta flag no quiero que se resizee porque el tamaño además va a ser bien adaptive.
    // SDL_WINDOW_HIDDEN HACE que la app NO aparezca en ningún lado, solo task  manager
    // la podríamos ver. Hay que habilitar funcionalidad específica para que aparezca
    // en la system tray.

    // PASITO IMPORTANTE PARA LA VENTANA,
    // Creamos una variable derivado de struct que es SDL_DisplayMode
    // Con la función SDL_GetCurrentDisplayMode extraemos
    // la resolución de la pantalla y la pointeamos a la variable anterior. El 1er parámetro es el nro de display.
    SDL_DisplayMode res;
    SDL_GetCurrentDisplayMode(0, &res);
    SDL_DisplayMode res_dynamic;

    //Creamos la ventana y el 3er y 4to parámetro es width y height con dos métodos .w y .h sacamos un int que es la reoslución en pixeles y bueno yo la hago relativa dividiendo.
    SDL_Window* window = SDL_CreateWindow("Agux's Random File Creator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, res.w/3, res.h/6, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Código para minimziar a tray en WINDOWS ////////////////////////////////////////////////////////////
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    // SDL_GetWMInfo(&info);

    NOTIFYICONDATA icon;
    if (SDL_GetWindowWMInfo(window, &info))
    {
        icon.uCallbackMessage = WM_USER + 1;
        icon.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
        // icon.hIcon = LoadIcon(NULL, IDI_INFORMATION);
        // Cambié LoadIcon por LoadImage, pero fue imposible encontrar la forma
        // de cargar realmente cargar una imagen custom, el tema es que loadimage
        // carga un HANDLE y necesitamos un HICON, con el truco de static_cast hacemos
        // la conversión, además el segundo parámetro es un LPCSTR que también horas
        // para que me tome lo que se supone que es, se arregla con la función TEXT.
        icon.hIcon = static_cast<HICON>(LoadImage(NULL, TEXT("favi.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
        icon.cbSize = sizeof(icon);
        icon.hWnd = info.info.win.window;
        strcpy_s(icon.szTip, "Agux's Random File Creator");

        // bool success =  Shell_NotifyIcon(NIM_ADD, &icon);
        // Lo guardaba en una variable esta función no sé por qué
        // yo lo ejecuto y listo la función.
        Shell_NotifyIcon(NIM_ADD, &icon);
    }

    // AGREGAR ESTA LÍNEA que permite utilizar la librería Swyswm
    // para sacar la info y eso para poder minimizar a tray dps.
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

    // FIN DE PRUEBAAA ////////////////////////////////////

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
////////////// ACA EMPIEZA LA GUI posta ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

    // bool hidear_ventana = true;
    // if(hidear_ventana) SDL_HideWindow(window);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;

            ///////// CODIGO DEL LOOP PARA MINIMIZAR A TRAY //////////////////////
            //////////////////////////////////////////////////////////////////
            switch (event.type)
            {
            case SDL_SYSWMEVENT:
                if (event.syswm.msg->msg.win.msg == WM_USER + 1)
                {
                    if (LOWORD(event.syswm.msg->msg.win.lParam) == WM_LBUTTONDBLCLK)
                    {
                        // SDL_RaiseWindow(window);
                        // SDL_SetWindowInputFocus(window);
                        // ShowWindow(info.window, SW_MAXIMIZE);
                        SDL_ShowWindow(window);
                        //Agrego un maximize primero aunque quede un poquito
                        // bugueado porque el restore solo no trae la ventana
                        // al frente, y el raise tampoco JAJ
                        SDL_MaximizeWindow(window);
                        SDL_RestoreWindow(window);
                    }
                }
                break;
            case SDL_QUIT:
                done = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_MINIMIZED)
                    SDL_HideWindow(window);
                break;
            }
            /////////////////////// FIN CODIGO PARA MINIMIZAR TRAY ///////////////////
            //////////////////////////////////////////////////////////////////

        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////
        /////////// MIS VENTANAS Y COSAS POSTA //////////////////////////////
        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////

        // Todo este código lo podríamos dejar en un archivo aparte y llamar una función nomás.
        // Debemos incluir imgui.h nomás en ese archivo.

        gui_builder0();

        //Este clear_color debe quedar definido no sé por qué
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ////////////////////////////////////////////////////////////
    //CODIGO DE RENDERING! ////////////////////////////////////
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // SIGUE DENTRO DE MI VENTANA!!!//////////////////////////
        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
