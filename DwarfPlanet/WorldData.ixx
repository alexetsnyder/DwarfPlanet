export module WorldData;

import <iostream>;
import <map>;
import <vector>;
import <string>;
import glmModule;
import Noise;

export struct BlockType
{
	std::string name;
	bool isSolid;

	std::string top_texture_name;
	std::string bottom_texture_name;
	std::string front_texture_name;
	std::string back_texture_name;
	std::string left_texture_name;
	std::string right_texture_name;

	BlockType(std::string name, bool isSolid, std::initializer_list<std::string> textureNameList)
	{
		this->name = name;
		this->isSolid = isSolid;
		std::vector<std::string> textureNameVct = { textureNameList };

		if (textureNameVct.size() == 6)
		{
			this->top_texture_name = textureNameVct[0];
			this->bottom_texture_name = textureNameVct[1];
			this->front_texture_name = textureNameVct[2];
			this->back_texture_name = textureNameVct[3];
			this->left_texture_name = textureNameVct[4];
			this->right_texture_name = textureNameVct[5];
		}
		else
		{
			this->top_texture_name = "";
			this->bottom_texture_name = "";
			this->front_texture_name = "";
			this->back_texture_name = "";
			this->left_texture_name = "";
			this->right_texture_name = "";
		}
	}

	std::string getTextureName(int face)
	{
		switch (face)
		{
		case 0:
			return top_texture_name;
		case 1:
			return bottom_texture_name;
		case 2:
			return front_texture_name;
		case 3:
			return back_texture_name;
		case 4:
			return left_texture_name;
		case 5:
			return right_texture_name;
		default:
			return top_texture_name;
		}
	}
};

export class WorldData
{
	public:
		WorldData(int chunkHeight)
			: noise(0, 0.03f)
		{
			this->worldSize = 4;
			this->chunkWidth = 4;
			this->chunkHeight = chunkHeight;
			this->dirtDepth = 2;

			if (getBlockTypeVct().empty())
			{
				getBlockTypeVct() =
				{
					BlockType("air", false, {}),
					BlockType("bedrock", true, {"bedrock", "bedrock", "bedrock", "bedrock", "bedrock", "bedrock"}),
					BlockType("stone", true, {"stone", "stone", "stone", "stone", "stone", "stone"}),
					BlockType("dirt", true, {"dirt", "dirt", "dirt", "dirt", "dirt", "dirt"}),
					BlockType("grass", true, {"grass_top", "dirt", "grass_side", "grass_side", "grass_side", "grass_side"}),
				};

				for (int i = 0; i < getBlockTypeVct().size(); i++)
				{
					BlockType block = getBlockTypeVct()[i];
					getBlockTypeLookUp()[block.name] = i;
				}
			}	
		}

		BlockType getBlockType(std::string name)
		{
			int index = getBlockId(name);
			return getBlockTypeVct()[index];
		}

		BlockType getBlockType(int index)
		{
			return getBlockTypeVct()[index];
		}

		int getBlockId(std::string name)
		{
			return getBlockTypeLookUp()[name];
		}

		bool hasSolidVoxel(Vec3& position)
		{
			if (isOutsideWorld(position))
			{
				return false;
			}

			int voxel = getVoxel(position);

			return getBlockType(voxel).isSolid;
		}

		bool isOutsideWorld(Vec3& position)
		{
			int bound = chunkWidth * worldSize / 2;
			if (position.y() < 0 || position.y() > chunkHeight - 1 ||
				position.x() < -bound || position.x() > bound - 1 ||
				position.z() < -bound || position.z() > bound - 1)
			{
				return true;
			}

			return false;
		}

		int getVoxel(Vec3& position)
		{
			int yPos = floor(position.y());

			if (yPos == 0)
			{
				return getBlockId("bedrock");
			}

			float noiseValue = noise.getNoise2D(position.x(), position.z());
			int height = floor(noise.remap(-1.0f, 1.0f, 20.0f, 60.0f, noiseValue));

			if (yPos > height)
			{
				return getBlockId("air");
			}
			else if (yPos == height)
			{
				return getBlockId("grass");
			}
			else if (yPos >= height - dirtDepth)
			{
				return getBlockId("dirt");
			}
			else
			{
				return getBlockId("stone");
			}

			return 0;
		}

	private:
		std::vector <BlockType>& getBlockTypeVct()
		{
			static std::vector<BlockType> blockTypeVct;
			return blockTypeVct;
		}

		std::map<std::string, int>& getBlockTypeLookUp()
		{
			static std::map<std::string, int> blockTypeLookUp;
			return blockTypeLookUp;
		}

		int chunkWidth, chunkHeight, dirtDepth, worldSize;
		Noise noise;
};

