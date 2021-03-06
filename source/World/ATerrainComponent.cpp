#include <Nephilim/World/ATerrainComponent.h>

#include <Nephilim/Foundation/Math.h>
#include <Nephilim/Foundation/Image.h>

NEPHILIM_NS_BEGIN

static const float GRID_SIZE = 1.0f;
static const float HEIGHT_FACTOR = 0.1f;
static const float TEXTURE_INCREMENT = 0.2f;

bool ATerrainComponent::load(const String& heightmapFileName)
{
	surfaceTex.loadFromFile("grass.png");
	surfaceTex.setRepeated(true);
	//surfaceTex.generateMipMaps();

	
	heightmap.loadFromFile(heightmapFileName);

	heightmapRaw = new Uint8[heightmap.getSize().x * heightmap.getSize().y];

	int k = 0;
	for( Uint16 x = 0; x < heightmap.getSize().x; ++x )
	{		
		for( Uint16 y = 0; y < heightmap.getSize().y; ++y )
		{
			if(x < heightmap.getSize().x-1 && y < heightmap.getSize().y - 1)
			{
				geometry.vertices.push_back(vec3(x * GRID_SIZE + GRID_SIZE, heightmap.getPixel(x+1,y).r * HEIGHT_FACTOR, y * GRID_SIZE));
				geometry.vertices.push_back(vec3(x * GRID_SIZE + GRID_SIZE, heightmap.getPixel(x + 1, y + 1).r * HEIGHT_FACTOR, y * GRID_SIZE + GRID_SIZE));
				geometry.vertices.push_back(vec3(x * GRID_SIZE, heightmap.getPixel(x, y + 1).r * HEIGHT_FACTOR, y * GRID_SIZE + GRID_SIZE));

				geometry.texcoords0.push_back(vec2(static_cast<float>( x+1 ) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y ) / static_cast<float>( heightmap.getSize().y)));
				geometry.texcoords0.push_back(vec2(static_cast<float>( x+1 ) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y+1 ) / static_cast<float>( heightmap.getSize().y)));
				geometry.texcoords0.push_back(vec2(static_cast<float>( x ) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y ) / static_cast<float>( heightmap.getSize().y)));

				geometry.vertices.push_back(vec3(x * GRID_SIZE + GRID_SIZE, heightmap.getPixel(x + 1, y).r * HEIGHT_FACTOR, y * GRID_SIZE));
				geometry.vertices.push_back(vec3(x * GRID_SIZE, heightmap.getPixel(x, y + 1).r * HEIGHT_FACTOR, y * GRID_SIZE + GRID_SIZE));
				geometry.vertices.push_back(vec3(x * GRID_SIZE, heightmap.getPixel(x, y).r * HEIGHT_FACTOR, y * GRID_SIZE));

				geometry.texcoords0.push_back(vec2(static_cast<float>( x +1) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y ) / static_cast<float>( heightmap.getSize().y)));
				geometry.texcoords0.push_back(vec2(static_cast<float>( x ) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y +1) / static_cast<float>( heightmap.getSize().y)));
				geometry.texcoords0.push_back(vec2(static_cast<float>( x ) / static_cast<float>( heightmap.getSize().x), static_cast<float>( y ) / static_cast<float>( heightmap.getSize().y)));
			}


			heightmapRaw[k++] = heightmap.getPixel(x,y).r;
		}
	}

	// Rotate the terrain

	mat4 trf = mat4::rotatey(math::pi/2);
	for (int i = 0; i < geometry.vertices.size(); ++i)
	{
		geometry.vertices[i].x -= heightmap.getSize().x * GRID_SIZE / 2 - GRID_SIZE / 2;
		geometry.vertices[i].z -= heightmap.getSize().y * GRID_SIZE / 2 - GRID_SIZE / 2;
		//geometry.m_vertices[i].y -= 255 * HEIGHT_FACTOR  / 2;
		geometry.vertices[i] = (trf * vec4(geometry.vertices[i], 1.f)).xyz();

		geometry.vertices[i].z *= -1;
	}

	geometry.setAllColors(Color::White);
	geometry.scaleUV(100);
	geometry.generateNormals();

	return true;
}

NEPHILIM_NS_END