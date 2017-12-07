#include "model.h"
#include "camera.h"


Model::~Model()
{
}

void Model::Update()
{
	m_shader.Use();
	
	std::string cameraID = (m_cameraID.empty()) ? "camera" : m_cameraID;
	Camera* camera = m_scene->GetObject<Camera>(cameraID);

	glm::mat4 mxView = camera->GetView();
	glm::mat4 mxMV = mxView * m_transform.GetMatrix44();
	m_shader.SetUniform("mxModelView", mxMV);

	glm::mat4 mxMVP = camera->GetProjection() * mxMV;
	m_shader.SetUniform("mxMVP", mxMVP);

	glm::mat3 mxNormal = glm::mat3(mxMV);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader.SetUniform("mxNormal", mxNormal);

}

void Model::Render()
{
	m_shader.Use();
	m_material.SetTextures();
	m_mesh.Render();

}
