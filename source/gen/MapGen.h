#ifndef __MAPGEN__
#define __MAPGEN__

#include <vector>
#include <map>
#include <unordered_map>

#include "PerlinNoise.h"

using namespace std;

class MapGen {

public:
	MapGen(int width, int height, float scale, int octaves, float persistance, float lacunarity,float amplitude);
	vector<float> generateChunk(vector<unsigned int>* ind,int coords[2]);
	void generateNoiseMap();
    void generateMap(float** verticesData, unsigned int** indicesData);
private:
	int width;
	int height;
	float scale;
	int octaves;
	float persistance;
	float lacunarity;
	float amplitude;
	unordered_map<int, float> noiseMap;
	uint32_t hash(uint32_t a);
};

#endif