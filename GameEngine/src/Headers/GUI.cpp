#include "GUI.hpp"


void GUI::metricsWindow()
{
    static bool showMetrics = true;
    ImGui::Begin("Metrics", &showMetrics, ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground);

    std::string text = "Framerate: " + std::to_string(framerate);
    ImGui::Text(text.c_str());

    ImGui::End();
}

void GUI::lightWindow()
{
    ImGui::Begin("Lighting");

    ImGui::SeparatorText("Directional Lights");
    ImGui::BeginChild("Directional Lights", {0, static_cast<float>(112 * lightSys.dirLights.size()) });
    for (int i = 0; i < lightSys.dirLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("DirLight" + i, {0, 112});

        DirectionalLight& dirLight = lightSys.dirLights[i];
        glm::vec3& dir = dirLight.direction;
        glm::vec3& ambient = dirLight.ambient;
        glm::vec3& diffuse = dirLight.diffuse;
        glm::vec3& specular = dirLight.specular;
        glm::vec3& color = dirLight.color;

        ImGui::DragFloat3("Direction", &dir[0], 0.05f, -1.0f, 1.0f);
        ImGui::ColorEdit3("Ambient", &ambient[0]);
        ImGui::ColorEdit3("Diffuse", &diffuse[0]);
        ImGui::ColorEdit3("Specular", &specular[0]);
        ImGui::ColorEdit3("Color", &color[0]);

        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SeparatorText("Point Lights");
    ImGui::BeginChild("Point Lights", { 0, static_cast<float>(112 * lightSys.pointLights.size()) });
    for (int i = 0; i < lightSys.pointLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("PointLight" + i, { 0, 112 });

        PointLight& pointLight = lightSys.pointLights[i];
        glm::vec3& position = pointLight.position;
        glm::vec3& ambient = pointLight.ambient;
        glm::vec3& diffuse = pointLight.diffuse;
        glm::vec3& specular = pointLight.specular;
        glm::vec3& color = pointLight.color;

        ImGui::DragFloat3("Position", &position[0], 0.05f);
        ImGui::ColorEdit3("Ambient", &ambient[0]);
        ImGui::ColorEdit3("Diffuse", &diffuse[0]);
        ImGui::ColorEdit3("Specular", &specular[0]);
        ImGui::ColorEdit3("Color", &color[0]);

        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SeparatorText("Spot Lights");
    ImGui::BeginChild("Spot Lights", {0, static_cast<float>(180 * lightSys.SpotLights.size()) });
    for (int i = 0; i < lightSys.SpotLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("Spot" + i, {0, 180});

        SpotLight& pointLight = lightSys.SpotLights[i];
        glm::vec3& position = pointLight.position;
        glm::vec3& dir = pointLight.direcion;
        glm::vec3& ambient = pointLight.ambient;
        glm::vec3& diffuse = pointLight.diffuse;
        glm::vec3& specular = pointLight.specular;
        glm::vec3& color = pointLight.color;

        float& cutOff = pointLight.cutOff;
        float& outerCutOff = pointLight.outerCutOff;

        if (cutOff < outerCutOff) outerCutOff = cutOff - glm::radians(1.0f);

        ImGui::DragFloat3("Position", &position[0], 0.05f);
        ImGui::DragFloat3("Direction", &dir[0], 0.05f, -1.0f, 1.0f);

        float lastCufOff = cutOff;
        if (ImGui::DragFloat("CutOff(rad)", &cutOff, 0.0005f, 0.0f, glm::radians(180.0f))) {
            float delta = lastCufOff - cutOff;
            outerCutOff += delta;
        }
        ImGui::DragFloat("OuterCutOff(rad)", &outerCutOff, 0.005f, 0.0f, cutOff);

        ImGui::ColorEdit3("Ambient", &ambient[0]);
        ImGui::ColorEdit3("Diffuse", &diffuse[0]);
        ImGui::ColorEdit3("Specular", &specular[0]);
        ImGui::ColorEdit3("Color", &color[0]);

        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::End();
}

GUI::GUI(GLFWwindow* window, LightingSystem& lightSys) : lightSys(lightSys)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 420");
}

void GUI::update(float time) {
    ++frameNum;
    if (time - lastFrame >= 1) {
        framerate = frameNum;
        frameNum = 0;
        lastFrame = time;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    metricsWindow();
    lightWindow();
}

void GUI::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
