export module WorldGeneration;

import <map>;
import <vector>;
import Chunk;
import Shader;

export struct BlockType;

export class WorldGeneration
{
	public:
		WorldGeneration()
		{
			viewDistanceInChunks = 5;
		}

		bool ContainsChunk(ChunkID chunkId)
		{
			return activeChunkMap.count(chunkId);
		}

		void generateWorld()
		{
			int worldSize = 4;

			for (int x = -worldSize / 2; x < worldSize / 2; x++)
			{
				for (int z = -worldSize / 2; z < worldSize / 2; z++)
				{
					ChunkID chunkId(x, z);

					Chunk chunk(chunkId, 16, 128);
					chunk.init();
					chunk.setMeshData();

					activeChunkMap[chunkId] = chunk;
				}
			}
		}

		void draw(Shader shader)
		{
			for (auto chunkPair : activeChunkMap)
			{
				chunkPair.second.draw(shader);
			}
		}

	private:
		std::map<ChunkID, Chunk> activeChunkMap;

		int viewDistanceInChunks;
};
