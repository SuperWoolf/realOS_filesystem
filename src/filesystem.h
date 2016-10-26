#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <vector>
#include <iostream>
#include <string>

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
	int* allBlockNbrs;
	int nbrOfBlocks;
	int TakeFirstFreeBlockNbr();

    // Here you can add your own data structures
    struct node
    {
        std::string directoryName;//Dirname or file name
        unsigned int nbrOfKids;
        node** kids;
        node* parent;
        int blockNr;    //-5 if dir otherwhise 0-249 for blocks
    } root;
	node * currentDirectory;

	node allNodes[250];

public:
	FileSystem(int nbrOfBlocks);
    ~FileSystem();
	
    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */

	int cat(std::string filePath, std::string &fileContent);

	/* Saves the filesystem to an image on the disk */
	int CreateImageCd(std::string fileName);

	/* Restore a filesystem from an image on the disk */
	int RestoreImageCd(std::string fileName);

	/* Build an empty filesystem*/
	int Format(std::string &currentDir);

    /* This function creates a file in the filesystem */
	int create(std::string filePath, std::string fileContent);
    
    /* Creates a folder in the filesystem */
	int mkdir(std::string dirPath);

    /* Removes a file in the filesystem */
	int rmFile(std::string fileToRemove);

    /* Removes a folder in the filesystem */
	int rmDir(std::string dirToRemove);

    /* Function will move the current location to a specified location in the filesystem */
	int cd(std::string dirPath);

    /* This function will get all the files and folders in the specified folder */
	int ls(std::string dirPath, std::string &dirContent);

	/* This function will return the absolut path to the working directory*/


    /* Add your own member-functions if needed */
private:
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);							//Used by all													
	void CreateNewNode(std::string dirName, node* parent, int blockNr);								//mkdir

	int CheckKidsFindBlockNr(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//returns the blocknumber of a file
	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> &dirs, unsigned int index);	//cd
	int GetDirectory(node *&currentNode, std::vector<std::string> &dirs, unsigned int index);

	int DeleteNode(node *nodeToBeDeleted);
	int DeleteFileBlock(int fileBlockToDelete);
	int RemoveFolder(node *currentNode, int kidNbr);
	int RemoveFile(node *currentNode, int kidNbr, int fileBlock);

};

#endif // FILESYSTEM_H
