#include "tp_maps_example/BuildExample.h"

#include "tp_maps/Map.h"
#include "tp_maps/controllers/FlatController.h"
#include "tp_maps/layers/Geometry3DLayer.h"
#include "tp_maps/layers/LinesLayer.h"

#include "tp_math_utils/Sphere.h"

#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace tp_maps_example
{

//##################################################################################################
void buildExample(tp_maps::Map* map)
{

  tp_maps::Geometry3DLayer* geometryLayer {nullptr};
  tp_maps::LinesLayer*      linesLayer    {nullptr};
  auto rotation = std::make_shared<int>(0);

  map->setBackgroundColor({0.5f, 0.5f, 0.5f});

  geometryLayer = new tp_maps::Geometry3DLayer();
  map->addLayer(geometryLayer);

  linesLayer = new tp_maps::LinesLayer();
  map->addLayer(linesLayer);

  std::vector<tp_maps::Geometry3D> geometry;
  auto& mesh = geometry.emplace_back().geometry;
  mesh = tp_math_utils::Sphere::octahedralClass1(6.0,
                                                 4,
                                                 GL_TRIANGLE_FAN,
                                                 GL_TRIANGLE_STRIP,
                                                 GL_TRIANGLES);
  mesh.calculateFaceNormals();

  geometryLayer->setGeometry(geometry);
  linesLayer->setLinesFromGeometry(geometry);

  auto controller = new tp_maps::FlatController(map);
  controller->setAllowRotation(false);
  controller->setVariableViewAngle(false);
  controller->setAllowTranslation(false);

  geometryLayer->animateCallbacks.addCallback([=](double t)
  {
    (*rotation)+=1;
    (*rotation)%=360;
    glm::mat4 m{1.0f};
    m = glm::rotate(m, glm::radians(float(*rotation)), glm::vec3(0.0, 1.0, 0.0));
    geometryLayer->setObjectMatrix(m);
    linesLayer->setObjectMatrix(m);
  });
}

}
