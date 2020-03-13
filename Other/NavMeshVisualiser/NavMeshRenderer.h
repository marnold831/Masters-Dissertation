#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLComputeShader.h"
#include "HeightMap.h"


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

		OGLMesh* navMesh;
		OGLShader* navShader;
		OGLComputeShader* computeshader;
		Camera* camera;
		HeightMap* heightmap;
		Particles* particles;

		GLuint computeBuffer;
	};
}

