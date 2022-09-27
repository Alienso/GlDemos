#include "MapGen.h"
#include <limits>
#include <glm/vec3.hpp>

#include <stdio.h>
#include <iostream>

using namespace std;

struct Vertex{
    glm::vec3 pos,norm;
};

MapGen::MapGen(int width_, int height_, float scale_, int octaves_, float persistance_, float lacunarity_,float amplitude_):width(width_),height(height_),scale(scale_),octaves(octaves_),persistance(persistance_),lacunarity(lacunarity_),amplitude(amplitude_){
}

vector<float> MapGen::generateChunk(vector<unsigned int>* ind,int coords[2]) {

	int i = 0;
	vector<float> vertices(width * height * 3 * 3); //xyz * pos+color+normal

	for (int i = 0; i < vertices.size(); i++)
		vertices[i] = 0;

	float* color = (float*)malloc(3 * sizeof(float));
	float* normal = (float*)malloc(3 * sizeof(float));

	int yOffset = coords[1] * height;
	int xOffset = coords[0] * width;
	for (int row = yOffset; row < yOffset + height; row++) {
		for (int col = xOffset; col < xOffset + width; col++) {
			vertices[i++] = (float)col ;
			float y = (noiseMap)[(53 + hash(row)) * 53 + hash(col)];
			vertices[i++] = y;
			vertices[i++] = (float)row;

			//getColorForHeight(y,color);
			vertices[i++] = color[0];
			vertices[i++] = color[1];
			vertices[i++] = color[2];

			i += 3;
		}
	}

	free(color);

	i = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width - 1; col++) {
			// 0.5 0.5 1 r g b n n n
			float p1[3] = { vertices[i],vertices[i + 1],vertices[i + 2] };
			float p2[3] = { vertices[i+9],vertices[i + 10],vertices[i + 11] };
			float p3[3] = { vertices[i+9*width],vertices[i + 9 * width + 1],vertices[i + 9 * width + 2] };

			float u[3] = { p2[0] - p1[0],p2[1] - p1[1],p2[2] - p1[2] };
			float v[3] = { p3[0] - p1[0],p3[1] - p1[1],p3[2] - p1[2] };

			i += 6;
			vertices[i++] = -(u[1]*v[2] - u[2]*v[1]);
			vertices[i++] = -(u[2]*v[0] - u[0]*v[2]);
			vertices[i++] = -(u[0]*v[1] - u[1]*v[0]);
		}
	}

	i = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (col == width - 1) {
				vertices[i + 6] = -vertices[i - 1];
				vertices[i + 7] = -vertices[i - 1];
				vertices[i + 8] = -vertices[i - 1];
				i += 9;
				continue;
			}
			i += 9;
		}
	}

	/*for (int i = 9 * (width+1); i < height ; i+=9*width) { //normals in last columns
		vertices[i] = vertices[i - 9];
		}*/
	ind->resize(0);
	for (int y = 0; y < height-1; y++)
	{
		unsigned int base = y * width;
		unsigned int top = base + width;
		for (int x = 0; x < (width - 1); x++)
		{
			ind->push_back(base + x);
			ind->push_back(top + x);
			ind->push_back(top + x + 1);

			ind->push_back(top + x + 1);
			ind->push_back(base + x + 1);
			ind->push_back(base + x);
		}
	}
	return vertices;
}

void MapGen::generateMap(float** verticesData, unsigned int** indices){
    generateNoiseMap();
    int verticesSize = width*height;
    //Vertex* vertices = new Vertex[verticesSize];

    //generate vertices
    int i=0,j=0;
    for (int row = -height/2; row < height/2; row++) {
        for (int col = -width/2; col < width/2; col++) {
            (*verticesData)[j++] = (float) col;
            float y = (noiseMap)[(53 + hash(row)) * 53 + hash(col)];
            (*verticesData)[j++] = y;
            (*verticesData)[j++] = (float) row;

            (*verticesData)[j++] = 0.0f;
            (*verticesData)[j++] = 0.0f;
            (*verticesData)[j++] = 0.0f;
            //TODO NORMS
            i++;
        }
    }

    //map to float[]
    /*int j = 0;
    for (int i = 0; i< verticesSize; i++){
        (*verticesData)[j++] = (vertices[i]).pos.x;
        (*verticesData)[j++] = (vertices[i]).pos.y;
        (*verticesData)[j++] = (vertices[i]).pos.z;

        (*verticesData)[j++] = 0.0f;//(vertices[i]).norm.x;
        (*verticesData)[j++] = 0.0f;//(vertices[i]).norm.y;
        (*verticesData)[j++] = 0.0f;//(vertices[i]).norm.z;
    }*/

    //generate indices
    int k=0;
    // |/ triangles
    for (int i=0;i<height-1;i++){
        for (int j=0;j<width-1;j++){
            (*indices)[k++] = i*width + j;
            (*indices)[k++] = i*width + j + 1;
            (*indices)[k++] = (i+1)*width + j;
        }
    }

    // /| triangles
    for (int i=0;i<height-1;i++){
        for (int j=0;j<width-1;j++){
            (*indices)[k++] = (i+1)*width + j;
            (*indices)[k++] = (i+1)*width + j + 1;
            (*indices)[k++] = i*width + j + 1;
        }
    }

    //free(vertices);
}

void MapGen::generateNoiseMap() {
	PerlinNoise perlin;

	float max = numeric_limits<float>::min();
	float min = numeric_limits<float>::max();

	for (int y = -height/2 ; y <  height/2; y++) {
		for (int x = -width/2 ; x < width/2; x++) {

			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++) {
				float sampleX = x * scale * frequency;
				float sampleY = y * scale * frequency;
				float perlinValue = perlin.noise((double)sampleX, (double)sampleY, 0) * 2 - 1;
				noiseHeight += perlinValue * amplitude;

				amplitude *= persistance;
				frequency *= lacunarity;
			}

			if (noiseHeight > max)
				max = noiseHeight;
			else if (noiseHeight < min)
				min = noiseHeight;

            (noiseMap)[(53 + hash(y)) * 53 + hash(x)] = noiseHeight;
		}
	}

	//normalize
	for (int y = - height; y < height; y++) {
		for (int x = -width ; x < width; x++) {
            (noiseMap)[(53 + hash(y)) * 53 + hash(x)] = (((noiseMap)[(53 + hash(y)) * 53 + hash(x)] - min) / (max - min))* amplitude;
		}
	}
}

uint32_t MapGen::hash(uint32_t a){
	a = (a + 0x7ed55d16) + (a << 12);
	a = (a ^ 0xc761c23c) ^ (a >> 19);
	a = (a + 0x165667b1) + (a << 5);
	a = (a + 0xd3a2646c) ^ (a << 9);
	a = (a + 0xfd7046c5) + (a << 3);
	a = (a ^ 0xb55a4f09) ^ (a >> 16);
	return a;
}