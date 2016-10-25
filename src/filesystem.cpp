#include "filesystem.h"

FileSystem::FileSystem(int nbrOfBlocks) {
	this->root.parent = nullptr;
	this->root.kids = nullptr;
	this->root.blockNr = -5;
	this->root.nbrOfKids = 0;
	this->root.directoryName = "/";

	this->currentDirectory = &this->root;
	/*this->currentDirectory.parent = nullptr;
	this->currentDirectory.kids = nullptr;
	this->currentDirectory.blockNr = -5;
	this->currentDirectory.nbrOfKids = 0;
	this->currentDirectory.directoryName = '/';*/

	this->allBlockNbrs = new int[nbrOfBlocks];
	this->nbrOfBlocks = nbrOfBlocks;
	for (int i = 0; i < nbrOfBlocks; i++)
	{
		this->allBlockNbrs[i] = i;
	}
}

FileSystem::~FileSystem() {
	delete[] this->allBlockNbrs;
}

int FileSystem::TakeFirstFreeBlockNbr()
{
	int firstFreeBlock = -5;
	for (int i = 0; i < this->nbrOfBlocks; i++)
	{
		if (this->allBlockNbrs[i] == i)
		{
			firstFreeBlock = i;
			this->allBlockNbrs[i] = -5;//use number
			i = this->nbrOfBlocks;//break loop
		}
	}
	return firstFreeBlock;
}

void FileSystem::CreateNewNode(std::string dirName, node* parent, int blockNr)
{
	node *newNode = new node;
	newNode->directoryName = dirName;
	newNode->parent = parent;
	newNode->kids = nullptr;
	newNode->nbrOfKids = 0;
	newNode->blockNr = blockNr;

	if (parent->nbrOfKids == 0)
	{
		parent->kids = new node*[1];
		parent->kids[0] = newNode;
		parent->nbrOfKids = 1;
	}
	else
	{
		node ** temp = new node*[parent->nbrOfKids + 1];
		for (int i = 0; i < parent->nbrOfKids; ++i)
		{
			temp[i] = parent->kids[i];
		}
		temp[parent->nbrOfKids] = newNode;
		parent->kids = temp;
		parent->nbrOfKids++;
		temp = nullptr;

	}
	for (int i = 0; i < parent->nbrOfKids; i++)
		std::cout << parent->kids[i]->directoryName << std::endl;
	std::cout << parent->nbrOfKids << std::endl;
	return;
}


int FileSystem::ListDir(std::string dirPath)
{
	int result = -5;

	if (dirPath == "")
	{
		for (int i = 0; i < this->currentDirectory->nbrOfKids; i++)
		{
			std::cout << this->currentDirectory->kids[i]->directoryName << std::endl;
			result = 0;
		}
	}
	else
	{
		std::vector<std::string> dirs = ConvertDirPathToVector(dirPath);

		if (dirPath[0] == '/')
		{
			//Absolut path
			result = CheckKidsListDir(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		}
		else if (dirs[0] == "")
		{
			for (int i = 0; i < root.nbrOfKids; i++)
			{
				std::cout << root.kids[i]->directoryName << std::endl;
				result = 0;
			}
		}
		else
		{
			result = CheckKidsListDir(currentDirectory, dirs, 0);
		}
	}
	return result;
}

int FileSystem::CheckKidsMakeDir(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	if (dirs.size() -1 == index && currentNode->nbrOfKids == 0)
	{
		//skapa mapp
		std::cout << "skapa mapp 1" << std::endl;
		//Skapamappfunktion
		CreateNewNode(dirs[index], currentNode, -5);//send -5 if directory

		result = 0;
	}
	else
	{
		bool madeDir = false;
		for(unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					index = index +1;
					std::cout << "Dirs size index: " << dirs.size() << " Index: " << index << std::endl;
					result = CheckKidsMakeDir(currentNode->kids[k], dirs, index);
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
			else //in last folder
			{
				//om mappen redan finns
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					std::cout << "Kid found" << std::endl;
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
		}
		if (!madeDir)
		{
			std::cout << "Skapa mapp 2" << std::endl;
			CreateNewNode(dirs[index],currentNode,-5);//send -5 if directory
			result = 0;
		}
	}
	return result;
}

int FileSystem::CheckKidsListDir(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	for (int  i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)
		{
			for (int k = 0; i < currentNode->kids[i]->nbrOfKids; i++)
			{
				std::cout << currentNode->kids[i]->kids[k]->directoryName << std::endl;
			}
			i = currentNode->nbrOfKids;
			result = 0;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)
		{
			result = CheckKidsListDir(currentNode->kids[i], dirs, index++);
		}
	}
	return result;
}

int FileSystem::CheckKidsFindBlockNr(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)
	{
		
		result = -5;
	}
	else
	{
		for (unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					index = index + 1;
					std::cout << "Dirs size index: " << dirs.size() << " Index: " << index << std::endl;
					result = CheckKidsMakeDir(currentNode->kids[k], dirs, index);
					k = currentNode->nbrOfKids;//exit loop
					
				}
			}
			else //in last folder
			{
				//om mappen redan finns
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					std::cout << "Kid found" << std::endl;
					
					result = currentNode->kids[k]->blockNr;
					
					k = currentNode->nbrOfKids;//exit loop
				}
			}
		}
	}

	return result;
}

std::vector<std::string> FileSystem::ConvertDirPathToVector(std::string dirPath)
{
	std::vector<std::string> dirs;
	std::string temp = "";
	int asd = dirPath.length();
	for (unsigned int i = 0; i <= dirPath.length(); i++)
	{

		if (dirPath[i] == '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else if (i == dirPath.length() && dirPath[i] != '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else
		{
			temp += dirPath[i];
		}
	}
	return dirs;
}

int FileSystem::CheckKidsMakeFile(node *currentNode, std::vector<std::string> dirs, unsigned int index, int blockNr)
{
	int result = -5;
	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)
	{
		//skapa mapp
		std::cout << "skapa fil 1" << std::endl;
		//Skapamappfunktion
		CreateNewNode(dirs[index], currentNode, blockNr);

		result = 0;
	}
	else
	{
		bool madeDir = false;
		for (unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					index = index + 1;
					std::cout << "Dirs size index: " << dirs.size() << " Index: " << index << std::endl;
					result = CheckKidsMakeFile(currentNode->kids[k], dirs, index, blockNr);
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
			else //in last folder
			{
				//om noden redan finns
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					std::cout << "Kid found" << std::endl;
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
		}
		if (!madeDir)
		{
			std::cout << "Skapa mapp 2" << std::endl;
			CreateNewNode(dirs[index], currentNode, blockNr);
			result = 0;
		}
	}
	return result;
}

int FileSystem::CreateFile(std::string filePath, std::string fileContent)
{
	int result = -5;
	int blockNr = this->TakeFirstFreeBlockNbr();

	std::vector<std::string> dirs;
	//std::string temp = "";
	std::cout << "Dir path lenght: " << filePath.length() << std::endl;
	

	dirs = ConvertDirPathToVector(filePath);

	if (filePath[0] == '/')
	{
		//Absolute path
		result = CheckKidsMakeFile(&root, dirs, 1, blockNr);		//Sending 1 as index because 0 is "/"
		std::cout << "Check kids result: " << result << std::endl;
	}
	else
	{
		result = CheckKidsMakeFile(this->currentDirectory, dirs, 0, blockNr);
		std::cout << "Check kids result: " << result << std::endl;
	}

	if (result != -5)
	{
		this->mMemblockDevice.writeBlock(blockNr, fileContent);
	}

	return result;
}

std::string FileSystem::GetFileContent(std::string filePath)
{
	std::string content = "File not found";
	int result = -5;
	std::vector<std::string> dirs;
	dirs = ConvertDirPathToVector(filePath);

	if (filePath[0] == '/')
	{
		//Absolut path
		result = CheckKidsFindBlockNr(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		std::cout << "Check kids result: " << result << std::endl;
	}
	else
	{
		result = CheckKidsFindBlockNr(this->currentDirectory, dirs, 0);
		std::cout << "Check kids result: " << result << std::endl;
	}

	if (result != -5)
	{
		content = this->mMemblockDevice.readBlock(result).toString();
	}

	return content;
}


int FileSystem::MakeDirectory(std::string dirPath)
{
	std::vector<std::string> dirs;
	//std::string temp = "";
	std::cout << "Dir path lenght: " << dirPath.length() << std::endl;
	int result = -5;

	dirs = ConvertDirPathToVector(dirPath);

	if(dirPath[0] == '/')
	{
		//Absolut path
		result = CheckKidsMakeDir(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		std::cout << "Check kids result: " << result << std::endl;
	}
	else
	{
		result = CheckKidsMakeDir(this->currentDirectory, dirs, 0);
		std::cout << "Check kids result: " <<  result << std::endl;
	}
	return result;
}

int FileSystem::RemoveFolder(std::string dirPath, std::string currentDir)
{
	std::vector<std::string> dirs;
	std::cout << "removing directory" << std::endl;
	int result = -5;

	dirs = ConvertDirPathToVector(dirPath);


	return result;
}



int FileSystem::SetCurrentDirByPath(node *currentNode, std::vector<std::string> dirs, unsigned int index)//moves the "currentDir" pointer to requested destination
{
	int result = -5;
	for (int i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)//last folder in searchpath
		{
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];
			i = currentNode->nbrOfKids;//exit loop
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)//search for kids, if found use function recursively
		{
			result = SetCurrentDirByPath(currentNode->kids[i], dirs, ++index);
		}
	}
	return result;
}

int FileSystem::GoToDirectory(std::string dirPath)//CD
{
	std::vector<std::string> dirs;
	std::cout << "CD called" << std::endl;
	int result = -5;

	dirs = ConvertDirPathToVector(dirPath);

	
	if (dirPath[0] == '/')
	{
		//Absolut path
		result = SetCurrentDirByPath(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		std::cout << "Absolute path: " << result << std::endl;
	}
	else
	{
		result = SetCurrentDirByPath(this->currentDirectory, dirs, 0);
		std::cout << "Path: " << result << std::endl;
	}

	return result;
}