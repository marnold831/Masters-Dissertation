#include "HeightMap.h"

using namespace NCL;
using namespace Rendering;

HeightMap::HeightMap(std::string name)  {
	std::ifstream file(name.c_str(), std::ios::binary);

	if (!file)
		return;

	float numVertices = RAW_WIDTH * RAW_HEIGHT;

	vector<Vector3> vertices;
	vertices.resize(numVertices);
	vector<Vector2> textureCoords;
	textureCoords.resize(numVertices);
	
	unsigned char* data = new unsigned char[numVertices];
	file.read((char*)data, numVertices * sizeof(unsigned char));
	file.close();

	for (int x = 0; x < RAW_WIDTH; ++x) {
		for (int z = 0; z < RAW_HEIGHT; ++z) {

			int offset = (x * RAW_HEIGHT) + z;

			vertices[offset] = Vector3(x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
			textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}
	positions = vertices;
	texCoords = textureCoords;

	delete data;

	float numIndices = 0;
	indices.resize((RAW_WIDTH - 1) * (RAW_WIDTH - 1) * 6);
	for (int x = 0; x < RAW_WIDTH - 1; ++x) {
		for (int z = 0; z < RAW_HEIGHT - 1; ++z) {
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z+1);
			int d = (x *(RAW_WIDTH)) +(z + 1);
				
			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;
				
			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
			
		}
	}
}
