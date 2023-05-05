export module TextureAtlas;

import <string>;
import <vector>;
import <map>;

export class TextureAtlas
{
	public:
		TextureAtlas(int size, int blockSize)
		{
			this->size = size;
			this->blockSize = blockSize;
			blockCount = size / blockSize;
			normalizedBlockSize = (float)blockSize / (float)size;
		}

		void createAtlas(std::vector<std::string> textureBlocks)
		{
			for (int i = 0; i < textureBlocks.size(); i++)
			{
				atlasLookUp[textureBlocks[i]] = i;
			}
		}

		std::vector<float> getUVCoordinates(std::string name)
		{
			int index = atlasLookUp[name];
			return getUVCoordinates(index);
		}

	private:
		std::vector<float> getUVCoordinates(int index)
		{
			std::vector<float> uvVector;

			float y = index / blockCount;
			float x = index - blockCount * y;

			y *= normalizedBlockSize;
			x *= normalizedBlockSize;

			uvVector.push_back(x);
			uvVector.push_back(y);

			uvVector.push_back(x);
			uvVector.push_back(y + normalizedBlockSize);

			uvVector.push_back(x + normalizedBlockSize);
			uvVector.push_back(y);

			uvVector.push_back(x + normalizedBlockSize);
			uvVector.push_back(y + normalizedBlockSize);

			return uvVector;
		}

		int size, blockSize, blockCount;
		float normalizedBlockSize;
		std::map<std::string, int> atlasLookUp;
};
