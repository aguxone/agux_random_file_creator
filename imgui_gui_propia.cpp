#include <iostream>
#include <thread> // Para las funciones de los botones necesito threads
// para que no me frene la GUI en paralelo.
#include <ctime> // Aporta la función time() y el tipo de objetos de tiempo time_t, struct tm, ctime(), strftime y otras cosas que dicen time.
using namespace std;
//Agrego mi archivito de funciones locas que quiero que haga
// Le defino la variable stop que usa la función porque es lo menos
// complicado para que ande y quede en este scope
int stop = 0;
int tiempo_pausa = 1;
// La última la va a poder cambiar el usuario.
#include "creador_archivos_random_main.cpp"

// Defino una variable que chekee si ya estaba
// empezada la función de crear archivitos que uso dps.
bool check_empezar = false;

void gui_builder0()
{
    
    // Primero obtenemos la resolución de la pantalla
    // que tiene métodos .x y .y 
    // ImVec2 res_size2 = ImGui::GetWindowSize();

    // Después guardamos el tamaño del viewport que esl relleno
    // DENTRO de la pantalla de SDL, como un browser
    // el viewport es una clase (no struct) así que usa viewport-> , tiene WorkPos, WorkSize
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowSize(true ? viewport->WorkSize : viewport->Size); // El FirstUseEver lo pongo de tradición nomás porque permite resizear, igual lo voy a dejar fijo acá así que da igual.
    ImGui::SetNextWindowPos(true ? viewport->WorkPos : viewport->Pos);
    // | ImGuiCond_FirstUseEver ESTE FLAG SIRVE si queremos resizear a mano con el botoncito abajo a la derecha. SI QUEREMOS que se resizee con la ventana de sdl lo debemos QUITAR porque es un cancer si no y no se adapta nunca a la ventana!!!
    // ImGui::SetNextWindowSize({(viewport->WorkSize.x/3),(viewport->WorkSize.y/6)}, ImGuiCond_FirstUseEver);

    // ImGui::SetNextWindowSize({651.f,273.f},


    // Aún así me voy a guardar los worksize en alguna variable porque si no es una paja volver a escribir todo, y los workpos
    float xsize = viewport->Size.x;
    float ysize = viewport->Size.y;
    float xpos = viewport->Pos.x;
    float ypos = viewport->Pos.y;
    // Los printee para debuguear, igual lo printea mil millones de veces
    // está impráctico.
    // cout << xsize << ysize << xpos << ypos;

    // EL BEGIN CREA LA VENTANA. También Hay BeginTable y otras cosas para crear otro tipo de cositas, pero Begin es lo común. El segundo parámetro es siempre NULL, es para activarlo desactivarlo con alguna variable. Y el tercero son las FLAGS, lo saqué del demo directo viendo un poco el viewport, le sacamos el decoration (la barrita que dice el nombre y para cerrar), hacemos no se mueva, que no se pueda resizear y que no use settings guardados en el ini.
    bool aver = true;
    
    ImGui::Begin("form0", &aver, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);
    // ImGui::SetCursorPos({60.f,60.f});
    // BUENO, para mí la mitad exacta es 0.46 el xsize o ysize. Como para tener una referencia; 0.5 está un poco pasado.
    ImGui::SetCursorPos({xsize * 0.05f, ysize * 0.1f});
    // ImGui::PushItemWidth(105.000000);
    // IImGui::PushItemWidth({xsize * 0.5f, ysize * 0.5f});
    ImGui::Text("Agux's Random File Creator");
    // ImGui::PopItemWidth( );
    // ImGui::SetCursorPos({36.f,78.f});
    ImGui::SetCursorPos({xsize * 0.5f, ysize * 0.3f});
    // jthread threadcito;
    if(ImGui::Button("Start", {xsize * 0.3f, ysize * 0.2f}))
    {
        if (check_empezar==false)
        {
            check_empezar=true;
            // lector_creador_archivos_ejecutar();
            stop = 0;
            thread threadcito(lector_creador_archivos_loop);
            // threadcito = threadcito(lector_creador_archivos_loop, stop);
            threadcito.detach();
            // threadcito.request_stop();
            // ImGui::Text("La App salvaje del agux vieja");
            // ImGui::ShowDemoWindow();
        }
    }

    ImGui::SetCursorPos({xsize * 0.5f, ysize * 0.6f});
    if(ImGui::Button("End", {xsize * 0.3f, ysize * 0.2f}))
    {
        // thread threadcito2([](){return stop=1;});
        // threadcito2.detach();
        stop = 1;
        check_empezar=false;
        cout << stop;
        // threadcito.request_stop();
    }

    // Textito que dice lo que hace la cajita de tiempo
    ImGui::SetCursorPos({xsize * 0.05f, ysize * 0.3f});
    ImGui::Text("Delay between \neach creation\n(seconds)");

    // Cajita para poner el tiempo
    ImGui::SetCursorPos({xsize * 0.05f, ysize * 0.6f});
        ImGui::PushItemWidth(xsize*0.3f);
        if( ImGui::InputInt(" ", &tiempo_pausa) )
        {
        }

    // ImGui::ShowDemoWindow();

    //Pequeño hack para cambiar rapido tamaño de font
    // ImGui::SetWindowFontScale(2.0);

    ImGui::End();
}


