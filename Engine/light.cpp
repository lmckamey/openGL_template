#include "light.h"
#include "camera.h"


Light::~Light()
{

}

void Light::Update()
{
	m_shader.Use();

	Camera* camera = m_scene->GetObject<Camera>("camera");
	glm::mat4 mxMVP = camera->GetProjection() * camera->GetView() * m_transform.GetMatrix44();
	m_shader.SetUniform("mxMVP", mxMVP);
	m_shader.SetUniform("color", diffuse);

}

void Light::Render()
{
	m_shader.Use();
	m_mesh.Render();
}
