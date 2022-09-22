//
//  Gui.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 03.06.19.
//

#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "GLExample.h"
namespace cgCourse
{
    bool inputAccept;
    void connectVar(std::string _varName, void * _varAddress){
        std::map<std::string,void *>::iterator it = vars.find(_varName);
        if (it != vars.end())
            it->second = _varAddress;
        else
            vars.insert(std::pair<std::string,void *>(_varName,_varAddress));
    }

    
    void *getVar(std::string _varName){
        auto ret = vars.find(_varName);
        if (ret != vars.end()){
            return ret->second;
        }else{
            std::cout<<"WARNING: "<<_varName<<" is not connected to the GUI"<<std::endl;
            return nullptr;
        }
    }

    void initGui(GLFWwindow* _window){
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 150");
        
        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'misc/fonts/README.txt' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("/Users/amiraghdam/Downloads/imgui-master/misc/fonts/Roboto-Medium.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF((GLApp::getPathToExecutable() + "../../res/Cousine-Regular.ttf").c_str(), 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("/Users/amiraghdam/Downloads/imgui-master/misc/fonts/Roboto-Medium.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);
        
        
        
        
        
    }
    
    void updateGui(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        bool showd,show_demo_window,show_another_window;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        // Menu Bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                // ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            /*
             if (ImGui::BeginMenu("Examples"))
             {
             ImGui::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
             ImGui::MenuItem("Console", NULL, &show_app_console);
             ImGui::MenuItem("Log", NULL, &show_app_log);
             ImGui::MenuItem("Simple layout", NULL, &show_app_layout);
             ImGui::MenuItem("Property editor", NULL, &show_app_property_editor);
             ImGui::MenuItem("Long text display", NULL, &show_app_long_text);
             ImGui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
             ImGui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
             ImGui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
             ImGui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
             ImGui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
             ImGui::MenuItem("Documents", NULL, &show_app_documents);
             ImGui::EndMenu();
             }
             */
            if (ImGui::BeginMenu("Help"))
            {
                ImGui::MenuItem("Metrics", NULL, &showd);
                ImGui::MenuItem("Style Editor", NULL, &showd);
                ImGui::MenuItem("About Dear ImGui", NULL, &showd);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        
        
       // ImGui::ShowDemoWindow(&showd);
        {
            static int counter = 0;
            ImGui::SetNextWindowPos({0,20});
            if (ImGui::GetFrameCount() ==1)
                ImGui::SetNextWindowSize({300,1000});
           
            ImGui::Begin("Menu",NULL,ImGuiWindowFlags_NoMove);                          // Create a window called "Menu" and append into it.
            
            ImGui::Text("Light Color:");               // Display some text (you can use a format strings too)
            ImGui::ColorEdit3("", (float *)getVar("lightDiffuse")); // Edit 3 floats representing a color
            
            
            ImGui::Separator();
            ImGui::Text("Camera Type:");
            ImGui::RadioButton("Arcball", (int *)getVar("cameraType"), 0); ImGui::SameLine();
            ImGui::RadioButton("Fly over", (int *)getVar("cameraType"), 1); ImGui::SameLine();
            ImGui::Separator();
            ImGui::Text("Shading Algorithm:");
            ImGui::RadioButton("Phong", (int *)getVar("shadingAlgorithm"), 0); ImGui::SameLine();
            ImGui::RadioButton("PBR", (int *)getVar("shadingAlgorithm"), 1); ImGui::SameLine();
            ImGui::Separator();
            ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("FPS: %.1f FPS", ImGui::GetIO().Framerate);
            
            inputAccept = !ImGui::GetIO().WantCaptureMouse;
            InputManager::consumed = !inputAccept;
            ImGui::End();
        }
        
        
        ImGui::Render();

    }
    void renderGui(){
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


}
