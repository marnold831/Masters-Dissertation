#pragma once
#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"


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
		//TODO Implement the heigtmap class
		//HeightMap* heightmap;
		Camera*		camera;
	};
}

