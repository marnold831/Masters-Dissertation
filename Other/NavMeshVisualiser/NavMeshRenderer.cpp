#include "NavMeshRenderer.h"
#include "../../Common/Assets.h"
#include "../../Common/Camera.h"

#include "../../Common/Vector3.h"
#include "../../Common/Matrix4.h"

#include <fstream>
#include <iostream>
using namespace NCL;

NavMeshRenderer::NavMeshRenderer() : OGLRenderer(*Window::GetWindow())	{
	
	heightmap = new HeightMap(Assets::MESHDIR + "terrain.raw");
	heightmap->SetTexture(OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "Barren Reds.jpg"));

	computeshader = new OGLComputeShader("ComputeShader.glsl");
	GenerateComputeBuffer();

	navShader = new OGLShader("BasicVert.glsl", "BasicFrag.glsl");
	camera = new Camera();

	heightmap->UploadToGPU();

	camera->SetNearPlane(1.0f);
	camera->SetFarPlane(5000.0f);
	camera->SetPitch(-30.0f);
	camera->SetPosition(Vector3(0, 500, 0));
}

NavMeshRenderer::~NavMeshRenderer() {
	delete navMesh;
	delete navShader;
	delete camera;
}

void NavMeshRenderer::Update(float dt) {
	camera->UpdateCamera(dt);
}

void NavMeshRenderer::RenderFrame() {

	computeshader->Bind();
	glUniform4fv(glGetUniformLocation(computeshader->GetProgramID(), "positions"), (float*)& positions);
	glUniform1f(glGetUniformLocation(computeshader->GetProgramID(), "number"), 1.0f);
	computeshader->Execute(1);

	BindShader(navShader);
	BindTextureToShader(heightmap->GetTexture(), "mainTex", 0);
	float screenAspect = (float)currentWidth / (float)currentHeight;

	Matrix4 viewMatrix = camera->BuildViewMatrix();
	Matrix4 projMatrix = camera->BuildProjectionMatrix(screenAspect);
	Matrix4 modelMat = Matrix4();
	Matrix4 texture = Matrix4();

	int projLocation	= glGetUniformLocation(navShader->GetProgramID(), "projMatrix");
	int viewLocation	= glGetUniformLocation(navShader->GetProgramID(), "viewMatrix");
	int modelLocation	= glGetUniformLocation(navShader->GetProgramID(), "modelMatrix");
	int textureMatrix = glGetUniformLocation(navShader->GetProgramID(), "textureMatrix");
	//int hasTexLocation	= glGetUniformLocation(navShader->GetProgramID(), "hasTexture");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMat);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(textureMatrix, 1, false, (float*)&texture);

	//glUniform1i(hasTexLocation, 0);

	BindMesh(heightmap);
	DrawBoundMesh();
}

void NCL::NavMeshRenderer::GenerateComputeBuffer() {
	glGenBuffers(1, &computeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 1 * sizeof(float), NULL, GL_STATIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBuffer);
}
