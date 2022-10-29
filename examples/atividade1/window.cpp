#include "window.hpp"

void Window::onCreate() {
  auto const &windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void Window::onPaint() {
  // Set the clear color
  abcg::glClearColor(m_clearColor.at(0), m_clearColor.at(1), m_clearColor.at(2),
                     m_clearColor.at(3));
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::onPaintUI();
  
  //ImGui::SetNextWindowPos(ImVec2(75,300));

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Começando!");
    ImGui::Text("Digite um Texto");
    static char str0[128] = "";
    ImGui::InputText(" ", str0, IM_ARRAYSIZE(str0));
    ImGui::Text("Escolha uma cor");
    const char* items[] = { "Vermelho", "Verde", "Azul", "Amarelo"};
    static int item_current = 0;
    ImGui::Combo("combo 3 (array)", &item_current, items, IM_ARRAYSIZE(items));
    ImGui::Text("Aperte o botão");
    if (ImGui::Button("Botão"))
          ImGui::OpenPopup("Modal");
    if (ImGui::BeginPopupModal("Modal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImVec4 imgCodes[] = {ImVec4(1.0f, 0.0f, 0.0f, 1.0f),ImVec4(0.0f, 1.0f, 0.0f, 1.0f),ImVec4(0.0f, 0.0f, 1.0f, 1.0f),ImVec4(1.0f, 1.0f, 0.3f, 1.0f)};
            ImGui::Text("Seu Texto no Modal");
            ImGui::TextColored(imgCodes[item_current], str0);
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }        
    // Window end
    ImGui::End();
  }
}