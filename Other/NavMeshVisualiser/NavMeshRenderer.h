#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLComputeShader.h"

#include "HeightMap.h"
#include "Particles.h"

#define NUMBER_PARTICLES 100


namespace NCL {
	class Camera;

	class NavMeshRenderer : public OGLRenderer
	{
	public:
		NavMeshRenderer();
		virtual ~NavMeshRenderer();
		void Update(float dt) override;
	protected:
		void RenderFrame()	override;
		void GenerateComputeBuffer();
		void SwapComputeBuffers();
		void RunGenerateShader();
		void SaveParticlesGenerated();
		void MoveParticles();

		GLuint defaultVAO;
		GLuint vao;

		GLuint vertexBuffer;
		GLuint computeBufferA;
		GLuint computeBufferB;
		
		OGLMesh* navMesh;

		OGLShader* navShader;
		OGLComputeShader* generateShader;
		OGLComputeShader* updateShader;

		Camera* camera;
		HeightMap* heightmap;
		Particles* particles;

		TextureBase* doge;

		

		int count;
		bool bufferBind;
	};
}

