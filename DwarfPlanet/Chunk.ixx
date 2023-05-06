export module Chunk;

#include <glad/glad.h>
#include "stb_image.h"

import <iostream>;
import <vector>;
import <cmath>;
import glmModule;
import Shader;
import WorldData;
import TextureAtlas;

const float cubeVertexArray[24][3] =
{
	//top face
	-0.5f,  0.5f, -0.5f,  //0
	-0.5f,  0.5f,  0.5f,  //1
	 0.5f,  0.5f, -0.5f,  //2
	 0.5f,  0.5f,  0.5f,  //3

	 //bottom face
	-0.5f, -0.5f,  0.5f,  //4
	-0.5f, -0.5f, -0.5f,  //5
	 0.5f, -0.5f,  0.5f,  //6
	 0.5f, -0.5f, -0.5f,  //7

	//front face
	-0.5f, -0.5f, -0.5f,  //8
	-0.5f,  0.5f, -0.5f,  //9
	 0.5f, -0.5f, -0.5f,  //10
	 0.5f,  0.5f, -0.5f,  //11

	//back face
	 0.5f, -0.5f,  0.5f,  //12
	 0.5f,  0.5f,  0.5f,  //13
	-0.5f, -0.5f,  0.5f,  //14
	-0.5f,  0.5f,  0.5f,  //15

	//left face
	-0.5f, -0.5f,  0.5f,  //16
	-0.5f,  0.5f,  0.5f,  //17
	-0.5f, -0.5f, -0.5f,  //18
	-0.5f,  0.5f, -0.5f,  //19

	//right face
	 0.5f, -0.5f, -0.5f,  //20
	 0.5f,  0.5f, -0.5f,  //21
	 0.5f, -0.5f,  0.5f,  //22
	 0.5f,  0.5f,  0.5f,  //23
};

const float cubeIndexArray[6][4] =
{
	 0,  1,  2,  3, //Top Face
	 4,  5,  6,  7, //Bottom Face
	 8,  9, 10, 11, //Front Face
	12, 13, 14, 15, //Back Face
	16, 17, 18, 19, //Left Face
	20, 21, 22, 23, //Right Face
};

const Vec3 NeighborArray[6] =
{
	Vec3(0.0f, 1.0f, 0.0f),
	Vec3(0.0f, -1.0f, 0.0f),
	Vec3(0.0f, 0.0f, -1.0f),
	Vec3(0.0f, 0.0f, 1.0f),
	Vec3(-1.0f, 0.0f, 0.0f),
	Vec3(1.0f, 0.0f, 0.0f),
};

const std::vector<std::string> textureBlocks
{
	"dirt",
	"grass_side",
	"grass_top",
	"stone",
	"bedrock",
	"oak_log_side",
	"oak_log_top",
	"oak_leaves",
	"sand",
	"red_sand",
	"soul_sand",
	"clay",
	"snow_top",
	"mossy_cobblestone",
	"mycelium_side",
	"mycelium_top",
};

export struct MeshData
{
	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;
};

export struct ChunkID
{
	int x, z;

	ChunkID(int x = 0, int z = 0)
	{
		this->x = x;
		this->z = z;
	}

	bool operator<(const ChunkID& rhs) const
	{
		return x < rhs.x || (x == rhs.x && z < rhs.z);
	}
};

export class Chunk
{
	public:
		Chunk()
			: atlas(64, 16), data(0)
		{
			this->chunkId = ChunkID(0, 0);
			this->width = 0;
			this->height = 0;

			model = Mat4(1.0f);

			VAO = 0;
			EBO = 0;
			VBO = 0;
			texture = 0;

			indexIndex = 0;
			triangleIndiciesCount = 0;
		}

		Chunk(ChunkID chunkId, int width, int height)
			: atlas(64, 16), data(height)
		{
			this->chunkId = chunkId;
			this->width = width;
			this->height = height;

			model = Mat4(1.0f);

			VAO = 0;
			EBO = 0;
			VBO = 0;
			texture = 0;

			indexIndex = 0;
			triangleIndiciesCount = 0;

			atlas.createAtlas(textureBlocks);
		}

		void init()
		{
			generateBuffers();
			generateTexture();
		}

		Vec3 getPosition()
		{
			return Vec3(chunkId.x * width, 0.0f, chunkId.z * width);
		}

		Mat4 getModel()
		{
			model = Mat4(1.0f);

			Vec3 position = getPosition();

			model = model.translate(position);

			return model;
		}

		void draw(Shader shader)
		{
			glBindTexture(GL_TEXTURE_2D, texture);

			shader.setUniform("model", getModel());

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, triangleIndiciesCount, GL_UNSIGNED_INT, 0);
		}

		MeshData getMeshData()
		{
			MeshData mesh;

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					for (int z = 0; z < width; z++)
					{
						float offsetPos[3] = { x, y, z };
						createVoxel(mesh, offsetPos);
					}
				}
			}

			return mesh;
		}

		void setMeshData()
		{
			MeshData mesh = getMeshData();

			triangleIndiciesCount = mesh.indexData.size();

			glBindVertexArray(VAO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indexData.size() * sizeof(float), &mesh.indexData.front(), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, mesh.vertexData.size() * sizeof(float), &mesh.vertexData.front(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}

		bool hasSolidVoxel(Vec3 position)
		{
			Vec3 worldPos = position + getPosition();
			return data.hasSolidVoxel(worldPos);
		}

		bool isOutsideChunk(Vec3 position)
		{
			int boundXZ = width / 2;
			int boundY = height - 1;

			int x = floor(position.x());
			int y = floor(position.y());
			int z = floor(position.z());

			if (x < -boundXZ || x > boundXZ - 1 ||
				z < -boundXZ || z > boundXZ - 1 ||
				y < 0 || y > boundY)
			{
				return true;
			}

			return false;
		}

	private:
		void createVoxel(MeshData& mesh, float offsetPos[3])
		{
			for (int i = 0; i < 6; i++)
			{
				Vec3 position = Vec3(offsetPos[0], offsetPos[1], offsetPos[2]);
				Vec3 neighborPos = position + NeighborArray[i];
				if (!hasSolidVoxel(neighborPos))
				{
					Vec3 worldPos = position + getPosition();
					BlockType blockType = data.getBlockType(data.getVoxel(worldPos));
					std::string textureName = blockType.getTextureName(i);
					std::vector<float> uvVector = atlas.getUVCoordinates(textureName);
					for (int j = 0; j < 4; j++)
					{
						int index = cubeIndexArray[i][j];
						for (int k = 0; k < 3; k++)
						{
							float vertex = cubeVertexArray[index][k] + offsetPos[k] + 0.5f;
							mesh.vertexData.push_back(vertex);
						}

						for (int l = 0; l < 2; l++)
						{
							float uv = uvVector[j * 2 + l];
							mesh.vertexData.push_back(uv);
						}
					}

					int indexArray[6] = { 0, 1, 2, 2, 1, 3 };
					for (int j = 0; j < 6; j++)
					{
						mesh.indexData.push_back(indexIndex + indexArray[j]);
					}
					indexIndex += 4;
				}
			}
		}

		void generateBuffers()
		{
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &EBO);

			glGenBuffers(1, &VBO);

			glBindVertexArray(0);
		}

		void generateTexture()
		{
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load("Assets/Textures/TextureAtlas.png", &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture image\n";
			}
			stbi_image_free(data);
		}

		Mat4 model;

		unsigned int VAO, EBO, VBO;
		int width, height;
		unsigned int texture;

		int indexIndex;
		unsigned int triangleIndiciesCount;

		TextureAtlas atlas;
		WorldData data;
		ChunkID chunkId;
};