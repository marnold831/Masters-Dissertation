#include "NavMeshRenderer.h"
#include "../../Common/Assets.h"
#include "../../Common/Camera.h"

#include "../../Common/Vector3.h"
#include "../../Common/Matrix4.h"

#include <fstream>
#include <iostream>
using namespace NCL;

NavMeshRenderer::NavMeshRenderer() : OGLRenderer(*Window::GetWindow()), count(0), bufferBind(false), dt(0.f)	{
	
	heightmap = new HeightMap(Assets::MESHDIR + "terrain.raw");
	heightmap->SetTexture(OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "Barren Reds.jpg"));

	particles = new Particles(NUMBER_PARTICLES);

	generateShader = new OGLComputeShader("SpawnComputeShader.glsl");
	updateShader = new OGLComputeShader("UpdateComputeShader.glsl");

	GenerateComputeBuffer();
	RunGenerateShader();
	SaveParticlesGenerated();

	navShader = new OGLShader("BasicVert.glsl", "BasicFrag.glsl", "RenderGeom.glsl");
	camera = new Camera();

	heightmap->UploadToGPU();

	camera->SetNearPlane(1.0f);
	camera->SetFarPlane(5000.0f);
	camera->SetPitch(0.0f);
	camera->SetPosition(Vector3(0, 0, 100));

	doge = OGLTexture::RGBATextureFromFilename(Assets::TEXTUREDIR + "doge.png");

	glGenVertexArrays(1, &defaultVAO);
	glBindVertexArray(defaultVAO);

	glEnable(GL_BLEND);
}

NavMeshRenderer::~NavMeshRenderer() {
	delete navMesh;
	delete navShader;
	delete camera;
}

void NavMeshRenderer::Update(float dt) {
	this->dt = dt*10;
	camera->UpdateCamera(dt);
	Vector3 cameraPos = camera->GetPosition();
	std::string cameraPosString = std::to_string(cameraPos.x) + ", "+ std::to_string(cameraPos.y) + ", " +  std::to_string(cameraPos.z);
	
	DrawString(cameraPosString, Vector2(10, 10));
}

void NavMeshRenderer::RenderFrame() {

	updateShader->Bind();

	int timeLocation = glGetUniformLocation(updateShader->GetProgramID(), "time");
	glUniform1f(timeLocation, dt);
	//std::cout << "dt: " << dt << std::endl;

	updateShader->Execute(NUMBER_PARTICLES);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//MoveParticles();

	if (bufferBind) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBufferA);
	}
	else {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBufferB);
	}

	BindShader(navShader);
	glBindVertexArray(defaultVAO);
	
	
	BindTextureToShader(doge, "mainTex", 0);
	float screenAspect = (float)currentWidth / (float)currentHeight;

	Matrix4 viewMatrix = camera->BuildViewMatrix();
	Matrix4 projMatrix = camera->BuildProjectionMatrix(screenAspect);
	Matrix4 modelMat = Matrix4();
	Matrix4 texture = Matrix4();

	int progID = navShader->GetProgramID();

	int projLocation	= glGetUniformLocation(progID, "projMatrix");
	int viewLocation	= glGetUniformLocation(progID, "viewMatrix");
	int modelLocation	= glGetUniformLocation(progID, "modelMatrix");
	int textureMatrix	= glGetUniformLocation(progID, "textureMatrix");
	
	glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMat);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(textureMatrix, 1, false, (float*)&texture);
	
	glDrawArrays(GL_POINTS, 0, NUMBER_PARTICLES);

	
	SwapComputeBuffers();
}

void NCL::NavMeshRenderer::GenerateComputeBuffer() {
	glGenBuffers(1, &positionBufferA);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBufferA);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBufferA);

	glGenBuffers(1, &positionBufferB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBufferB);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBufferB);

	glGenBuffers(1, &directionBufferA);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, directionBufferA);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, directionBufferA);

	glGenBuffers(1, &directionBufferB);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, directionBufferB);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, directionBufferB);


}

void NCL::NavMeshRenderer::SwapComputeBuffers() {
	if (count % 2 == 0) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBufferA);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, directionBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, directionBufferA);
		bufferBind = true;
	}
	else {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, positionBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positionBufferA);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, directionBufferB);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, directionBufferA);
		bufferBind = false;
	}
	count++;
}

void NCL::NavMeshRenderer::RunGenerateShader() {
	generateShader->Bind();
	generateShader->Execute(NUMBER_PARTICLES);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_COPY_READ_BUFFER, positionBufferA);
	glBindBuffer(GL_COPY_WRITE_BUFFER, positionBufferB);
	glBufferData(GL_COPY_WRITE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, NUMBER_PARTICLES * sizeof(Vector4));

	glBindBuffer(GL_COPY_READ_BUFFER, directionBufferA);
	glBindBuffer(GL_COPY_WRITE_BUFFER, directionBufferB);
	glBufferData(GL_COPY_WRITE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, NUMBER_PARTICLES * sizeof(Vector4));


}

void NCL::NavMeshRenderer::SaveParticlesGenerated() {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBufferA);
	//Vector4* ptr;
	ptr = (Vector4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, NUMBER_PARTICLES * sizeof(Vector4), GL_MAP_READ_BIT);
	vector<Vector4> temp;
	for (int i = 0; i < NUMBER_PARTICLES; ++i) {
		temp.push_back(ptr[i]);
		
	}
	particles->SetPositions(temp);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	
}

void NCL::NavMeshRenderer::MoveParticles() {
	Vector4 centrePos(0.f, 0.f, 0.f, 0.f);
	std::vector<Vector4> positions = particles->GetPositions();
	std::vector<Vector4> newPositions;
	
	for (auto p : positions) {
		Vector3 position(p.x, p.z, p.y);
		Vector3 direction = Vector3(0, 0, 0) - position;
		direction.Normalise();
		direction *= -0.1f;
		position = position + direction;
		newPositions.push_back(Vector4(position.x, position.y, position.z, 1));
	}
	particles->SetPositions(newPositions);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBufferA);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionBufferB);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUMBER_PARTICLES * sizeof(Vector4), (float*)particles->GetPositions().data(), GL_DYNAMIC_DRAW);
}
