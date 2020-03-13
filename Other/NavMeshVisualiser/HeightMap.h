#pragma once
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"

#include <string>	
#include <iostream>
#include <fstream>

#define	RAW_HEIGHT 257	
#define RAW_WIDTH 257

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z	16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

namespace NCL {
	namespace Rendering {
		class HeightMap : public OGLMesh {
		public:
			HeightMap(std::string name);
			~HeightMap() {};

			void SetTexture(TextureBase* _texture) { texture = _texture; }
			TextureBase* GetTexture()const { return texture; }

		private:
			TextureBase* texture;
		};
	}
}