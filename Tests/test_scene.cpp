//
// Created by jr on 2/12/24.
//
#include <catch2/catch_test_macros.hpp>
#include "Scene.h"
#include "Camera.h"
#include "STL.h"

TEST_CASE("Scene Management", "[Scene]") {
    AScene scene("main");
    ACamera cam;
    cam.SetActive(true);
    scene.AddGameObject<ACamera>(cam);

    auto mainCam = AScene::GetActiveCamera(scene.GetContext());
    REQUIRE(mainCam != nullptr);
}