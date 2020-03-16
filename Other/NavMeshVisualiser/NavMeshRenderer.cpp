#include "NavMeshRenderer.h"
#include "../../Common/Assets.h"
#include "../../Common/Camera.h"

#include "../../Common/Vector3.h"
#include "../../Common/Matrix4.h"

#include <fstream>
#include <iostream>
using namespace NCL;

NavMeshRenderer::NavMeshRenderer() : OGLRenderer(*Window::GetWindow()), count(0), bufferBind(false)	{
	
	heightmap = new HeightMap(Assets::MESHDIR + "terrain.raw");
	heightmap->SetTexture(OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "Barren Reds.jpg"));
	particles = new Particles(100);
	computeshader = new OGLComputeShader("SpawnComputeShader.glsl");
	GenerateComputeBuffer();

	navShader = new OGLShader("BasicVert.glsl", "BasicFrag.glsl", "RenderGeom.glsl");
	camera = new Camera();

	heightmap->UploadToGPU();

	camera->SetNearPlane(1.0f);
	camera->SetFarPlane(5000.0f);
	camera->SetPitch(-30.0f);
	camera->SetPosition(Vector3(0, 500, 0));

	navMesh = new OGLMesh();
	//navMesh->SetTexture(OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "doge.png"));
	vector<Vector3> verts;
	verts.resize(100);
	navMesh->SetVertexPositions(verts);
	
	doge = OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "doge.png");

	glGenVertexArrays(1, &defaultVAO);
	glBindVertexArray(defaultVAO);
	

	
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
	
	//glUniform1f(glGetUniformLocation(computeshader->GetProgramID(), "number"), 1.0f);
	computeshader->Execute(100);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeBuffer);
	Vector4* ptr;
	ptr = (Vector4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 100 * sizeof(Vector4), GL_MAP_READ_BIT);
	vector<Vector4> temp;
	for (int i = 0; i < 100; ++i) {
		temp.push_back(ptr[i]);
		std::cout << "position " << i << ": " << ptr[i] << std::endl;
	}
	particles->SetPositions(temp);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/
	if (bufferBind) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBufferA);
	}
	else {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBufferB);
	}
	BindShader(navShader);
	glBindVertexArray(defaultVAO);
	
	BindTextureToShader(doge, "mainTex", 0);
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

	//BindMesh(navMesh);
	glDrawArrays(GL_POINTS, 0, 100);
	//DrawBoundMesh();

	SwapComputeBuffers();
}

void NCL::NavMeshRenderer::GenerateComputeBuffer() {
	glGenBuffers(1, &computeBufferA);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeBufferA);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 100 * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBufferA);

	glGenBuffers(1, &computeBufferB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeBufferB);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 100 * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, computeBufferB);



}

void NCL::NavMeshRenderer::SwapComputeBuffers() {
	if (count % 2 == 0) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, computeBufferA);
		bufferBind = true;
	}
	else {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, computeBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeBufferA);
		bufferBind = false;
	}
	count++;
}
