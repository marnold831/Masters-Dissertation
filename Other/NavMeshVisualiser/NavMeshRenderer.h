#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLComputeShader.h"

#include "HeightMap.h"
#include "Particles.h"

#define NUMBER_PARTICLES 100//100,000


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
		void GenerateVertexBuffer();
		void SwapComputeBuffers();
		void RunGenerateShader();
		void SaveParticlesGenerated();
		

		GLuint defaultVAO;

		GLuint vertexBuffer;
		GLuint positionBufferA;
		GLuint positionBufferB;
		GLuint directionBufferA;
		GLuint directionBufferB;
		
		OGLMesh* navMesh;

		OGLShader* navShader;
		OGLComputeShader* generateShader;
		OGLComputeShader* updateShader;

		Camera* camera;
		HeightMap* heightmap;
		Particles* particles;

		TextureBase* doge;

		Vector4* ptr;

		int count;
		bool bufferBind;
		float dt;
	};
}

