#include "GUI.hpp"

void GUI::lightAndCamWindow()
{
    ImGui::Begin("Lighting and Cameras");
    ImGui::BeginTabBar("LightingAndCameras", {});
    if (ImGui::TabItemButton("Lighting")) lightingWindow = true;
    if (ImGui::TabItemButton("Cameras")) lightingWindow = false;
    ImGui::EndTabBar();
   
    lightingWindow ? lighting() : cameraWindow();

    ImGui::End();
}

void GUI::lighting()
{
    ImGui::SeparatorText("Directional Lights");
    ImGui::BeginChild("Directional Lights", { 0, static_cast<float>(130 * lightSys.dirLights.size()) });
    for (int i = 0; i < lightSys.dirLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("DirLight" + i, { 0, 125 });

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

    if (ImGui::Button("Add Directional Light")) {
        lightSys.addDirectionalLight(DirectionalLight{});
    }

    ImGui::EndChild();

    ImGui::SeparatorText("Point Lights");
    ImGui::BeginChild("Point Lights", { 0, static_cast<float>(120 * lightSys.pointLights.size()) });
    for (int i = 0; i < lightSys.pointLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("PointLight" + i, { 0, 120 });

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

    if (ImGui::Button("Add Point Light")) {
        lightSys.addPointLight(PointLight{});
    }

    ImGui::EndChild();

    ImGui::SeparatorText("Spot Lights");
    ImGui::BeginChild("Spot Lights", { 0, static_cast<float>(180 * lightSys.SpotLights.size()) });
    for (int i = 0; i < lightSys.SpotLights.size(); ++i) {
        if (i > 0) ImGui::Separator();
        ImGui::BeginChild("Spot" + i, { 0, 180 });

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

    if (ImGui::Button("Add Spot Light")) {
        lightSys.addSpotlLight(SpotLight{});
    }

    ImGui::EndChild();
}

void GUI::cameraWindow()
{
    bool camChanged = false;

    ImGui::BeginChild("Cameras");
    for (unsigned int i = 0; i < cameras.size(); ++i) {
        if (i > 0) ImGui::Separator();

        ImGui::BeginChild("Cam" + i, { 0, 110 });

        Camera& cam = cameras[i];

        glm::vec3& position = cam.Position;
        glm::vec3& rotation = cam.Front;
        bool useCam = i == usedCamIdx;

        ImGui::DragFloat3("Position", &position[0], 0.05f);
        ImGui::DragFloat3("Rotation", &rotation[0], 0.05f);
        ImGui::DragFloat("Speed", &cam.MovementSpeed, 0.25f, 1.0f);
        if (ImGui::Checkbox("UseCam", &useCam)) {
            camChanged = true;
            usedCamIdx = i;
        }

        if (cameras.size() > 1) {
            if (ImGui::Button("Delete")) {
                cameras.erase(cameras.begin() + i);
            }
        }

        ImGui::EndChild();
    }

    if (ImGui::Button("Add Camera")) {
        cameras.emplace_back(Camera());
    }

    ImGui::EndChild();

    if (usedCamIdx > cameras.size() - 1) {
        usedCamIdx = (cameras.size() - 1 >= 0) ? (cameras.size() - 1) : 0;
    }
}

void GUI::objectsWindow()
{
    ImGui::Begin("Objects");

    for (std::pair<std::string, std::vector<Transformations>&>& objs : objects) {
        ImGui::BeginChild(objs.first.c_str());
        ImGui::SeparatorText(std::string(objs.first + ": " + std::to_string(objs.second.size())).c_str());

        if (ImGui::Button("Add")) {
            objs.second.emplace_back(Transformations{});
        }

        for (unsigned int i = 0; i < objs.second.size(); ++i) {

            ImGui::BeginChild(std::string(objs.first + std::to_string(i)).c_str(), {0, 90});

            Transformations& trans = objs.second[i];

            ImGui::DragFloat3("Position", &trans.position[0], 0.05f);
            ImGui::DragFloat4("Rotation", &trans.rotation[0], 0.05f);
            ImGui::DragFloat3("Scale", &trans.scale[0], 0.025f);
            
            if (ImGui::Button("Delete")) {
                objs.second.erase(objs.second.begin() + i);
            }

            ImGui::EndChild();
            ImGui::Separator();
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

GUI::GUI(GLFWwindow* window, LightingSystem& lightSys, unsigned int& usedCamIdx, std::vector<Camera>& cameras, std::vector<std::pair<std::string, std::vector<Transformations>&>>& objects) : lightSys(lightSys), usedCamIdx(usedCamIdx), cameras(cameras), objects(objects)
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

    lightAndCamWindow();
    objectsWindow();
    ImGui::ShowMetricsWindow();
}

void GUI::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
